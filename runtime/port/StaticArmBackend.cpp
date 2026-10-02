#include "StaticArmBackend.h"
#include "NativeTiming.h"
#include <algorithm>
#include <cstring>
#ifndef ROOT_PORT_STATIC_MODULE_ONLY
#include <dlfcn.h>
#endif
#include <iostream>
#include <limits>
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
void stopDispatch(Context* context) { context->exit = EXIT_BUDGET; }
#ifndef ROOT_PORT_STATIC_MODULE_ONLY
template<class T> T loadSymbol(void* library, const char* name) {
    auto* pointer = dlsym(library, name);
    if (!pointer) throw std::runtime_error(std::string("missing static library symbol: ") + name);
    return reinterpret_cast<T>(pointer);
}
void closeNativeLibrary(void* library) { dlclose(library); }
#endif
}

const Host StaticArmBackend::callbacks{Read8, Read16, Read32, Write8, Write16, Write32, RefuseInterpretation, Lookup};

StaticArmBackend::StaticArmBackend(Core::System& system, Memory::MemorySystem& memory_, u32 id,
                                 std::shared_ptr<Core::Timing::Timer> timer_, const std::filesystem::path& path,
                                 std::shared_ptr<GuestMemoryTrace> trace_)
    : ARM_Interface(id, std::move(timer_)), memory(memory_),
      callback_pages(1 << 20, nullptr), trace(std::move(trace_)) {
#ifdef ROOT_PORT_STATIC_MODULE_ONLY
    (void)system;
    (void)path;
    throw std::runtime_error("native library loading is unavailable in the static-module build");
#else
    std::unique_ptr<void, decltype(&closeNativeLibrary)> loaded(
        dlopen(path.c_str(), RTLD_NOW | RTLD_LOCAL), closeNativeLibrary);
    if (!loaded) {
        const auto* error = dlerror();
        throw std::runtime_error(error ? error : "cannot load static CPU native library");
    }
    const TranslatedFunctionModule module{
        *loadSymbol<const u32*>(loaded.get(), "recomp_abi"),
        *loadSymbol<const u32*>(loaded.get(), "native_timing_revision"),
        loadSymbol<const Entry*>(loaded.get(), "recomp_entries"),
        *loadSymbol<const u32*>(loaded.get(), "recomp_entry_count"),
        loadSymbol<NativeBlockTimingCallback*>(loaded.get(), "native_block_timing_callback")
    };
    InitializeModule(system, module, path.parent_path() / "block_schedule.bin");
    library = loaded.release();
#endif
}

StaticArmBackend::StaticArmBackend(Core::System& system, Memory::MemorySystem& memory_, u32 id,
                                 std::shared_ptr<Core::Timing::Timer> timer_, const TranslatedFunctionModule& module,
                                 const std::filesystem::path& schedule_path,
                                 std::shared_ptr<GuestMemoryTrace> trace_)
    : ARM_Interface(id, std::move(timer_)), memory(memory_),
      callback_pages(1 << 20, nullptr), trace(std::move(trace_)) {
    InitializeModule(system, module, schedule_path);
}

void StaticArmBackend::InitializeModule(Core::System& system, const TranslatedFunctionModule& module,
                                       const std::filesystem::path& schedule_path) {
    module.Validate();
    schedule = std::make_unique<NativeBlockSchedule>(schedule_path);
    svc = std::make_unique<Kernel::SVCContext>(system);
    entries = module.entries;
    entry_count = module.entry_count;
    context.host = &callbacks;
    context.user = this;
    context.read_pages = context.write_pages = callback_pages.data();
    context.vfp = floating_registers.data();
    context.fpscr = &fpscr;
    page_table = memory.GetCurrentPageTable();
    *module.timing_callback = chargeBlock;
}
StaticArmBackend::~StaticArmBackend() {
    std::cerr << "static CPU " << GetID() << " executed " << InstructionsExecuted() << " guest instructions; interpreter/JIT fallbacks 0\n";
#ifndef ROOT_PORT_STATIC_MODULE_ONLY
    if (library) closeNativeLibrary(library);
#endif
}

void StaticArmBackend::Run() {
    if (break_flag || timer->GetDowncount() <= 0) return;
    if (cpsr_control & 0x0600FE00u)
        throw std::runtime_error("native scheduling does not support CPSR IT or big-endian state");
    reschedule = false;
    schedule->BeginRun();
    // The stock CPU exposes accumulated completed-block ticks at SVC and Run return.
    // A linked forward block or return-stack hit can cross an expired slice.
    context.budget = std::numeric_limits<int32_t>::max();
    for (;;) {
        if (!schedule->ResolveBoundary(context, context.r[15] | context.thumb, timer->GetDowncount(), reschedule)) {
            timer->AddTicks(schedule->TakePendingTicks());
            return;
        }
        auto code = FindCode(context.r[15] | context.thumb);
        if (!code) throw std::runtime_error("missing static CPU entry at " + std::to_string(context.r[15]));
        const auto before_pc = context.r[15];
        const auto before_count = InstructionsExecuted();
        context.exit = EXIT_NONE;
        context.depth = 0;
        code(&context);
        if (context.exit == EXIT_SVC) {
            timer->AddTicks(schedule->TakePendingTicks());
            svc->CallSVC(context.svc);
            if (!schedule->Complete(context, context.r[15] | context.thumb, timer->GetDowncount(), reschedule)) {
                timer->AddTicks(schedule->TakePendingTicks());
                return;
            }
        }
        if (context.exit == EXIT_BUDGET) {
            timer->AddTicks(schedule->TakePendingTicks());
            return;
        }
        if (reschedule) throw std::runtime_error("native reschedule outside a completed supervisor block");
        if (context.r[15] == before_pc && InstructionsExecuted() == before_count)
            throw std::runtime_error("static CPU dispatch made no progress");
    }
}
void StaticArmBackend::Step() { throw std::runtime_error("static CPU cannot single-step a translated basic block"); }
void StaticArmBackend::ChargeBlock(u32 address, u32 instructions, u64) {
    current_instruction = address;
    if (instructions > 1 && address == 0x0010766C)
        schedule->ChargePriorityReplacement(context, instructions, timer->GetDowncount(), reschedule);
    else
        schedule->BeforeInstruction(context, address, instructions, timer->GetDowncount(), reschedule);
}
void StaticArmBackend::ClearInstructionCache() { schedule->ClearVisited(); }
void StaticArmBackend::InvalidateCacheRange(u32, std::size_t) {
    // Guest code remains immutable. Dynamic executable writes require regeneration.
    schedule->ClearVisited();
}
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
void StaticArmBackend::Write32(Context* c, u32 a, u32 v) {
    auto& cpu = backend(c);
    if (cpu.trace) cpu.trace->RecordWrite(cpu.GetID(), cpu.current_instruction, a, v, *c);
    cpu.memory.Write32(a, v);
}
void StaticArmBackend::RefuseInterpretation(Context*, u32 address, u32 opcode) {
    throw std::runtime_error("static CPU interpreter fallback refused at " + std::to_string(address) + " opcode " + std::to_string(opcode));
}
Code StaticArmBackend::Lookup(Context* c, u32 a) {
    auto& cpu = backend(c);
    if (!cpu.schedule->ResolveBoundary(*c, a, cpu.timer->GetDowncount(), cpu.reschedule)) return stopDispatch;
    return cpu.FindCode(a);
}
Code StaticArmBackend::FindCode(u32 address) const {
    auto* end = entries + entry_count;
    auto* found = std::lower_bound(entries, end, address, [](const Entry& entry, u32 value) { return entry.address < value; });
    return found != end && found->address == address ? found->code : nullptr;
}
}
