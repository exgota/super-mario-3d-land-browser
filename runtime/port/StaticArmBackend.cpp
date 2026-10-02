#include "StaticArmBackend.h"
#include "NativeTiming.h"
#include <algorithm>
#include <cstring>
#include <dlfcn.h>
#include <iostream>
#include <stdexcept>
#include "core/hle/kernel/svc.h"
#include "core/memory.h"

namespace Port {
namespace {
StaticArmBackend& backend(Context* context) {
    return *static_cast<StaticArmBackend*>(context->user);
}
void chargeBlock(Context* context, u32 address, u32 instructions, u64 ticks) {
    backend(context).ChargeBlock(address, instructions, ticks);
}
template<class T> T loadSymbol(void* library, const char* name) {
    auto* pointer = dlsym(library, name);
    if (!pointer) throw std::runtime_error(std::string("missing static library symbol: ") + name);
    return reinterpret_cast<T>(pointer);
}
}

const Host StaticArmBackend::callbacks{Read8, Read16, Read32, Write8, Write16, Write32, RefuseInterpretation, Lookup};

StaticArmBackend::StaticArmBackend(Core::System& system, Memory::MemorySystem& memory_, u32 id,
                                 std::shared_ptr<Core::Timing::Timer> timer_, const std::filesystem::path& path)
    : ARM_Interface(id, std::move(timer_)), memory(memory_),
      svc(std::make_unique<Kernel::SVCContext>(system)), callback_pages(1 << 20, nullptr) {
    library = dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL);
    if (!library) throw std::runtime_error(dlerror());
    if (*loadSymbol<const u32*>(library, "recomp_abi") != RECOMP_ABI)
        throw std::runtime_error("static CPU ABI disagreement");
    entries = loadSymbol<const Entry*>(library, "recomp_entries");
    entry_count = *loadSymbol<const u32*>(library, "recomp_entry_count");
    for (u32 i = 1; i < entry_count; ++i)
        if (entries[i - 1].address >= entries[i].address) throw std::runtime_error("invalid static entry table");
    *loadSymbol<NativeBlockTimingCallback*>(library, "native_block_timing_callback") = chargeBlock;
    context.host = &callbacks;
    context.user = this;
    context.read_pages = context.write_pages = callback_pages.data();
    context.vfp = floating_registers.data();
    context.fpscr = &fpscr;
    page_table = memory.GetCurrentPageTable();
}
StaticArmBackend::~StaticArmBackend() {
    std::cerr << "static CPU " << GetID() << " executed " << instructions_executed << " guest instructions; interpreter/JIT fallbacks 0\n";
    if (library) dlclose(library);
}

void StaticArmBackend::Run() {
    if (break_flag) return;
    reschedule = false;
    // Every generated instruction is resumable. Stock instruction costs advance the
    // platform timer, and its downcount stops dispatch at the next instruction.
    context.budget = 1000000;
    while (!reschedule && timer->GetDowncount() > 0) {
        auto code = FindCode(context.r[15] | context.thumb);
        if (!code) throw std::runtime_error("missing static CPU entry at " + std::to_string(context.r[15]));
        const auto before_pc = context.r[15];
        const auto before_count = instructions_executed;
        context.exit = EXIT_NONE;
        context.depth = 0;
        code(&context);
        if (context.exit == EXIT_SVC) {
            svc->CallSVC(context.svc);
            timer->AddTicks(supervisor_ticks);
            supervisor_ticks = 0;
        }
        if (context.exit == EXIT_BUDGET || reschedule || timer->GetDowncount() <= 0) return;
        if (context.r[15] == before_pc && instructions_executed == before_count)
            throw std::runtime_error("static CPU dispatch made no progress");
    }
}
void StaticArmBackend::Step() { throw std::runtime_error("static CPU cannot single-step a translated basic block"); }
void StaticArmBackend::ChargeBlock(u32, u32 instructions, u64 ticks) {
    instructions_executed += instructions;
    if (ticks & (u64(1) << 63)) supervisor_ticks += ticks & ~(u64(1) << 63);
    else timer->AddTicks(ticks);
    if (timer->GetDowncount() <= 0) context.budget = 0;
}
void StaticArmBackend::ClearInstructionCache() {}
void StaticArmBackend::InvalidateCacheRange(u32, std::size_t) {}
void StaticArmBackend::ClearExclusiveState() { context.exclusive = 0; }
void StaticArmBackend::SetPageTable(const std::shared_ptr<Memory::PageTable>& table) { page_table = table; }
std::shared_ptr<Memory::PageTable> StaticArmBackend::GetPageTable() const { return page_table; }
void StaticArmBackend::SetPC(u32 value) { context.r[15] = value; }
u32 StaticArmBackend::GetPC() const { return context.r[15]; }
u32 StaticArmBackend::GetReg(int index) const { return context.r[index]; }
void StaticArmBackend::SetReg(int index, u32 value) { context.r[index] = value; }
u32 StaticArmBackend::GetVFPReg(int index) const { return floating_registers[index]; }
void StaticArmBackend::SetVFPReg(int index, u32 value) { floating_registers[index] = value; }
u32 StaticArmBackend::GetVFPSystemReg(VFPSystemRegister reg) const {
    if (reg == VFP_FPSCR) return fpscr;
    if (reg == VFP_FPEXC) return fpexc;
    throw std::runtime_error("unsupported VFP system register");
}
void StaticArmBackend::SetVFPSystemReg(VFPSystemRegister reg, u32 value) {
    if (reg == VFP_FPSCR) fpscr = value;
    else if (reg == VFP_FPEXC) fpexc = value;
    else throw std::runtime_error("unsupported VFP system register");
}
u32 StaticArmBackend::GetCPSR() const {
    return cpsr_control | u32(context.n) << 31 | u32(context.z) << 30 | u32(context.c) << 29 |
        u32(context.v) << 28 | u32(context.q) << 27 | u32(context.ge) << 16 | u32(context.thumb) << 5;
}
void StaticArmBackend::SetCPSR(u32 value) {
    cpsr_control = value & ~0xF80F0020u;
    context.n = value >> 31; context.z = (value >> 30) & 1;
    context.c = (value >> 29) & 1; context.v = (value >> 28) & 1;
    context.q = (value >> 27) & 1; context.ge = (value >> 16) & 15; context.thumb = (value >> 5) & 1;
}
u32 StaticArmBackend::GetCP15Register(CP15Register reg) const { return coprocessor_registers[reg]; }
void StaticArmBackend::SetCP15Register(CP15Register reg, u32 value) {
    coprocessor_registers[reg] = value;
    if (reg == CP15_THREAD_URO) context.tls = value;
}
void StaticArmBackend::SaveContext(ThreadContext& output) {
    std::copy_n(context.r, 16, output.cpu_registers.begin());
    output.cpsr = GetCPSR(); output.fpu_registers = floating_registers;
    output.fpscr = fpscr; output.fpexc = fpexc;
}
void StaticArmBackend::LoadContext(const ThreadContext& input) {
    std::copy_n(input.cpu_registers.begin(), 16, context.r);
    SetCPSR(input.cpsr); floating_registers = input.fpu_registers;
    fpscr = input.fpscr; fpexc = input.fpexc;
    ClearExclusiveState();
}
void StaticArmBackend::PrepareReschedule() { reschedule = true; context.budget = 0; }
u8 StaticArmBackend::Read8(Context* c, u32 a) { return backend(c).memory.Read8(a); }
u16 StaticArmBackend::Read16(Context* c, u32 a) { return backend(c).memory.Read16(a); }
u32 StaticArmBackend::Read32(Context* c, u32 a) { return backend(c).memory.Read32(a); }
void StaticArmBackend::Write8(Context* c, u32 a, u8 v) { backend(c).memory.Write8(a, v); }
void StaticArmBackend::Write16(Context* c, u32 a, u16 v) { backend(c).memory.Write16(a, v); }
void StaticArmBackend::Write32(Context* c, u32 a, u32 v) { backend(c).memory.Write32(a, v); }
void StaticArmBackend::RefuseInterpretation(Context*, u32 address, u32 opcode) {
    throw std::runtime_error("static CPU interpreter fallback refused at " + std::to_string(address) + " opcode " + std::to_string(opcode));
}
Code StaticArmBackend::Lookup(Context* c, u32 a) { return backend(c).FindCode(a); }
Code StaticArmBackend::FindCode(u32 address) const {
    auto* end = entries + entry_count;
    auto* found = std::lower_bound(entries, end, address, [](const Entry& entry, u32 value) { return entry.address < value; });
    return found != end && found->address == address ? found->code : nullptr;
}
}
