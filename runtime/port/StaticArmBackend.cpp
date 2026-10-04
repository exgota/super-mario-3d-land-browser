#include "StaticArmBackend.h"
#include "NativeTiming.h"
#include <algorithm>
#include <cstring>
#include <exception>
#ifndef ROOT_PORT_STATIC_MODULE_ONLY
#include <dlfcn.h>
#endif
#include <iostream>
#include <limits>
#include <stdexcept>
#include <type_traits>
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
                                 std::shared_ptr<GuestMemoryTrace> trace_,
                                 std::shared_ptr<GuestWriteObservation> write_observation_,
                                 std::shared_ptr<GuestExecutionObservation> execution_observation_)
    : ARM_Interface(id, std::move(timer_)), memory(memory_),
      callback_pages(1 << 20, nullptr), trace(std::move(trace_)),
      write_observation(std::move(write_observation_)), execution_observation(std::move(execution_observation_)) {
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
                                 std::shared_ptr<GuestMemoryTrace> trace_,
                                 std::shared_ptr<GuestWriteObservation> write_observation_,
                                 std::shared_ptr<GuestExecutionObservation> execution_observation_)
    : ARM_Interface(id, std::move(timer_)), memory(memory_),
      callback_pages(1 << 20, nullptr), trace(std::move(trace_)),
      write_observation(std::move(write_observation_)), execution_observation(std::move(execution_observation_)) {
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
    if (write_observation) instruction_context.context_identity = write_observation->RegisterContext();
    if (execution_observation) execution_context_identity = execution_observation->RegisterContext(
        GetID(), this, &context, write_observation ? instruction_context.context_identity : 0);
    *module.timing_callback = chargeBlock;
}
StaticArmBackend::~StaticArmBackend() {
    std::cerr << "static CPU " << GetID() << " executed " << InstructionsExecuted() << " guest instructions; interpreter/JIT fallbacks 0\n";
#ifndef ROOT_PORT_STATIC_MODULE_ONLY
    if (library) closeNativeLibrary(library);
#endif
}

void StaticArmBackend::RefreshMemoryPages() {
    context.read_pages = context.write_pages = callback_pages.data();
    if (trace || write_observation || execution_observation) return;
    const auto current_page_table = memory.GetCurrentPageTable();
    if (current_page_table) {
        // This is the live array. Remapping, watchpoints and rasterizer cache
        // invalidation update its cells rather than a copied page snapshot.
        // Non-Memory pages remain null and use the existing host callbacks.
        context.read_pages = context.write_pages = current_page_table->GetPointerArray().data();
    }
}

void StaticArmBackend::Run() {
    ExecutionBoundary("run_entry");
    if (write_observation) instruction_context.valid = false;
    if (break_flag || timer->GetDowncount() <= 0) { ExecutionBoundary("run_not_entered"); return; }
    if (cpsr_control & 0x0600FE00u)
        throw std::runtime_error("native scheduling does not support CPSR IT or big-endian state");
    RefreshMemoryPages();
    reschedule = false;
    schedule->BeginRun();
    // The stock CPU exposes accumulated completed-block ticks at SVC and Run return.
    // A linked forward block or return-stack hit can cross an expired slice.
    context.budget = std::numeric_limits<int32_t>::max();
    for (;;) {
        if (!schedule->ResolveBoundary(context, context.r[15] | context.thumb, timer->GetDowncount(), reschedule)) {
            timer->AddTicks(schedule->TakePendingTicks());
            ExecutionBoundary("run_return_scheduling_boundary");
            return;
        }
        auto code = FindCode(context.r[15] | context.thumb);
        if (!code) throw std::runtime_error("missing static CPU entry at " + std::to_string(context.r[15]));
        const auto before_pc = context.r[15];
        const auto before_count = InstructionsExecuted();
        context.exit = EXIT_NONE;
        context.depth = 0;
        try { code(&context); }
        catch (...) {
            try { ExecutionBoundary("translated_dispatch_exception"); } catch (...) {}
            throw;
        }
        ExecutionBoundary("translated_dispatch_return_observed", false);
        if (context.exit == EXIT_SVC) {
            if (write_observation) instruction_context.valid = false;
            timer->AddTicks(schedule->TakePendingTicks());
            ExecutionBoundary("supervisor_call_entry_visibility_gap");
            try { svc->CallSVC(context.svc); }
            catch (...) {
                try { ExecutionBoundary("supervisor_call_exception_visibility_gap"); } catch (...) {}
                throw;
            }
            RefreshMemoryPages();
            ExecutionBoundary("supervisor_call_return_visibility_gap");
            if (!schedule->Complete(context, context.r[15] | context.thumb, timer->GetDowncount(), reschedule)) {
                timer->AddTicks(schedule->TakePendingTicks());
                ExecutionBoundary("run_return_after_supervisor_boundary");
                return;
            }
        }
        if (context.exit == EXIT_BUDGET) {
            timer->AddTicks(schedule->TakePendingTicks());
            ExecutionBoundary("run_return_budget_yield");
            return;
        }
        if (reschedule) throw std::runtime_error("native reschedule outside a completed supervisor block");
        if (context.r[15] == before_pc && InstructionsExecuted() == before_count)
            throw std::runtime_error("static CPU dispatch made no progress");
    }
}
void StaticArmBackend::Step() { ExecutionBoundary("unsupported_single_step"); throw std::runtime_error("static CPU cannot single-step a translated basic block"); }
void StaticArmBackend::ChargeBlock(u32 address, u32 instructions, u64 ticks) {
    current_instruction = address;
    if (!trace && !write_observation && !execution_observation && instructions == 1 &&
        schedule->ContinueInstruction()) return;
    if (write_observation) {
        instruction_context.valid = false;
        instruction_context.address = address;
        instruction_context.span = instructions;
        instruction_context.callback_ordinal = 0;
        ++instruction_context.generation;
        instruction_context.entry = ObservationContext();
        instruction_context.table = memory.GetCurrentPageTable();
        instruction_context.backend_table_matches = instruction_context.table == page_table;
    }
    const bool observing = execution_observation && execution_observation->Collecting();
    if (observing) {
        try { execution_observation->BeginCharge(execution_context_identity, address, instructions, ticks, ExecutionContext()); }
        catch (...) { execution_observation->MarkIncomplete("charge_entry_record_failure"); throw; }
    }
    const auto before = observing ? InstructionsExecuted() : 0;
    bool admitted = false;
    try {
        if (instructions > 1 && address == 0x0010766C) {
            schedule->ChargePriorityReplacement(context, instructions, timer->GetDowncount(), reschedule);
            if (write_observation) instruction_context.valid = instructions != 0;
            admitted = observing && instructions != 0 && InstructionsExecuted() - before == instructions;
        } else {
            admitted = schedule->BeforeInstruction(context, address, instructions, timer->GetDowncount(), reschedule);
            if (write_observation) instruction_context.valid = admitted;
        }
    } catch (const std::exception& exception) {
        if (observing) {
            try { execution_observation->Admission(execution_context_identity, false, true, ExecutionContext(), exception.what()); }
            catch (...) { execution_observation->MarkIncomplete("admission_exception_record_failure"); }
        }
        throw;
    } catch (...) {
        if (observing) {
            try { execution_observation->Admission(execution_context_identity, false, true, ExecutionContext(), "non_standard_exception"); }
            catch (...) { execution_observation->MarkIncomplete("admission_exception_record_failure"); }
        }
        throw;
    }
    if (observing) {
        try { execution_observation->Admission(execution_context_identity, admitted, false, ExecutionContext()); }
        catch (...) { execution_observation->MarkIncomplete("admission_record_failure"); throw; }
    }
}
void StaticArmBackend::ClearInstructionCache() { ExecutionBoundary("instruction_cache_invalidation_visibility_gap"); schedule->ClearVisited(); }
void StaticArmBackend::InvalidateCacheRange(u32, std::size_t) {
    // Guest code remains immutable. Dynamic executable writes require regeneration.
    ExecutionBoundary("cache_range_invalidation_visibility_gap");
    schedule->ClearVisited();
}
void StaticArmBackend::ClearExclusiveState() { ExecutionBoundary("clear_exclusive_before"); context.exclusive = 0; ExecutionBoundary("clear_exclusive_after"); }
void StaticArmBackend::SetPageTable(const std::shared_ptr<Memory::PageTable>& table) {
    ExecutionBoundary("set_backend_page_table_before"); page_table = table; ExecutionBoundary("set_backend_page_table_after");
}
std::shared_ptr<Memory::PageTable> StaticArmBackend::GetPageTable() const { return page_table; }
void StaticArmBackend::SetPC(u32 value) {
    ExecutionBoundary("set_program_counter_before");
    if (write_observation) instruction_context.valid = false;
    context.r[15] = value;
    ExecutionBoundary("set_program_counter_after");
}
u32 StaticArmBackend::GetPC() const { return context.r[15]; }
u32 StaticArmBackend::GetReg(int index) const { return context.r[index]; }
void StaticArmBackend::SetReg(int index, u32 value) { ExecutionBoundary("set_register_before"); context.r[index] = value; ExecutionBoundary("set_register_after"); }
u32 StaticArmBackend::GetVFPReg(int index) const { return floating_registers[index]; }
void StaticArmBackend::SetVFPReg(int index, u32 value) { ExecutionBoundary("set_floating_register_before"); floating_registers[index] = value; ExecutionBoundary("set_floating_register_after"); }
u32 StaticArmBackend::GetVFPSystemReg(VFPSystemRegister reg) const {
    if (reg == VFP_FPSCR) return fpscr;
    if (reg == VFP_FPEXC) return fpexc;
    throw std::runtime_error("unsupported VFP system register");
}
void StaticArmBackend::SetVFPSystemReg(VFPSystemRegister reg, u32 value) {
    ExecutionBoundary("set_floating_system_register_before");
    if (reg == VFP_FPSCR) fpscr = value;
    else if (reg == VFP_FPEXC) fpexc = value;
    else throw std::runtime_error("unsupported VFP system register");
    ExecutionBoundary("set_floating_system_register_after");
}
u32 StaticArmBackend::GetCPSR() const {
    return cpsr_control | u32(context.n) << 31 | u32(context.z) << 30 | u32(context.c) << 29 |
        u32(context.v) << 28 | u32(context.q) << 27 | u32(context.ge) << 16 | u32(context.thumb) << 5;
}
void StaticArmBackend::SetCPSR(u32 value) {
    ExecutionBoundary("set_status_register_before");
    cpsr_control = value & ~0xF80F0020u;
    context.n = value >> 31; context.z = (value >> 30) & 1;
    context.c = (value >> 29) & 1; context.v = (value >> 28) & 1;
    context.q = (value >> 27) & 1; context.ge = (value >> 16) & 15; context.thumb = (value >> 5) & 1;
    ExecutionBoundary("set_status_register_after");
}
u32 StaticArmBackend::GetCP15Register(CP15Register reg) const { return coprocessor_registers[reg]; }
void StaticArmBackend::SetCP15Register(CP15Register reg, u32 value) {
    ExecutionBoundary("set_coprocessor_register_before");
    coprocessor_registers[reg] = value;
    if (reg == CP15_THREAD_URO) context.tls = value;
    ExecutionBoundary("set_coprocessor_register_after");
}
void StaticArmBackend::SaveContext(ThreadContext& output) {
    std::copy_n(context.r, 16, output.cpu_registers.begin());
    output.cpsr = GetCPSR(); output.fpu_registers = floating_registers;
    output.fpscr = fpscr; output.fpexc = fpexc;
}
void StaticArmBackend::LoadContext(const ThreadContext& input) {
    ExecutionBoundary("load_context_before");
    if (write_observation) instruction_context.valid = false;
    std::copy_n(input.cpu_registers.begin(), 16, context.r);
    SetCPSR(input.cpsr); floating_registers = input.fpu_registers;
    fpscr = input.fpscr; fpexc = input.fpexc;
    ClearExclusiveState();
    ExecutionBoundary("load_context_after");
}
void StaticArmBackend::PrepareReschedule() { ExecutionBoundary("reschedule_request_before"); reschedule = true; context.budget = 0; ExecutionBoundary("reschedule_request_after"); }
u8 StaticArmBackend::Read8(Context* c, u32 a) { auto& cpu = backend(c); return cpu.ObserveRead(c, a, 1, [&] { return cpu.memory.Read8(a); }); }
u16 StaticArmBackend::Read16(Context* c, u32 a) { auto& cpu = backend(c); return cpu.ObserveRead(c, a, 2, [&] { return cpu.memory.Read16(a); }); }
u32 StaticArmBackend::Read32(Context* c, u32 a) { auto& cpu = backend(c); return cpu.ObserveRead(c, a, 4, [&] { return cpu.memory.Read32(a); }); }
GuestWriteObservation::MachineContext StaticArmBackend::ObservationContext() const {
    GuestWriteObservation::MachineContext result;
    std::copy_n(context.r, 16, result.registers.begin());
    result.floating_registers = floating_registers;
    result.cpsr = GetCPSR(); result.fpscr = fpscr; result.fpexc = fpexc;
    result.tls = context.tls; result.exclusive = context.exclusive;
    result.exclusive_address = context.exclusive_address;
    result.timer_ticks = timer->GetTicks(); result.timer_downcount = timer->GetDowncount();
    result.scheduled_instructions = InstructionsExecuted();
    return result;
}
GuestExecutionObservation::Snapshot StaticArmBackend::ExecutionContext(Context* callback) const {
    GuestExecutionObservation::Snapshot result;
    result.machine = ObservationContext();
    static_assert(std::is_same_v<decltype(coprocessor_registers), decltype(result.coprocessor_registers)>);
    std::copy(coprocessor_registers.begin(), coprocessor_registers.end(), result.coprocessor_registers.begin());
    result.coprocessor_count = CP15_REGISTER_COUNT;
    result.cpsr_control = cpsr_control;
    result.condition_fields = {context.n, context.z, context.c, context.v, context.q, context.thumb, context.ge};
    result.budget = context.budget; result.exit = context.exit; result.supervisor_call = context.svc;
    result.dispatch_depth = context.depth; result.reschedule = reschedule; result.break_requested = break_flag;
    result.callback_matches_backend = callback == &context;
    result.writer_enabled = bool(write_observation);
    result.writer_context = instruction_context.context_identity;
    result.writer_generation = instruction_context.generation;
    result.writer_callback_ordinal = instruction_context.callback_ordinal;
    result.writer_charge_valid = instruction_context.valid;
    result.actual_table = memory.GetCurrentPageTable(); result.backend_table = page_table;
    return result;
}
void StaticArmBackend::ExecutionBoundary(const char* reason, bool invalidate) {
    if (execution_observation && execution_observation->Collecting()) {
        try { execution_observation->Boundary(execution_context_identity, reason, ExecutionContext(), invalidate); }
        catch (...) { execution_observation->MarkIncomplete("boundary_record_failure"); throw; }
    }
}
template<class ReadOperation> u32 StaticArmBackend::ObserveRead(
    Context* callback, u32 address, u32 width, ReadOperation&& operation) {
    if (!execution_observation || !execution_observation->Collecting()) return operation();
    GuestExecutionObservation::MemoryToken token;
    std::exception_ptr observation_failure;
    try { token = execution_observation->BeginMemory(execution_context_identity, false, address, width, 0, ExecutionContext(callback)); }
    catch (...) {
        execution_observation->MarkIncomplete("memory_begin_record_failure"); observation_failure = std::current_exception();
    }
    u32 result;
    try { result = operation(); }
    catch (const std::exception& exception) {
        try { execution_observation->CompleteMemory(token, false, 0, ExecutionContext(callback), exception.what()); }
        catch (...) { execution_observation->MarkIncomplete("memory_exception_record_failure"); }
        throw;
    } catch (...) {
        try { execution_observation->CompleteMemory(token, false, 0, ExecutionContext(callback), "non_standard_exception"); }
        catch (...) { execution_observation->MarkIncomplete("memory_exception_record_failure"); }
        throw;
    }
    try { execution_observation->CompleteMemory(token, true, result, ExecutionContext(callback)); }
    catch (...) {
        execution_observation->MarkIncomplete("memory_completion_record_failure");
        if (!observation_failure) observation_failure = std::current_exception();
    }
    if (observation_failure) std::rethrow_exception(observation_failure);
    return result;
}
template<class WriteOperation> void StaticArmBackend::ObserveExecutionWrite(
    Context* callback, u32 address, u32 width, u32 value, WriteOperation&& operation) {
    if (!execution_observation || !execution_observation->Collecting()) { operation(); return; }
    GuestExecutionObservation::MemoryToken token;
    std::exception_ptr observation_failure;
    try { token = execution_observation->BeginMemory(execution_context_identity, true, address, width, value, ExecutionContext(callback)); }
    catch (...) {
        execution_observation->MarkIncomplete("memory_begin_record_failure"); observation_failure = std::current_exception();
    }
    try { operation(); }
    catch (const std::exception& exception) {
        try { execution_observation->CompleteMemory(token, false, 0, ExecutionContext(callback), exception.what()); }
        catch (...) { execution_observation->MarkIncomplete("memory_exception_record_failure"); }
        throw;
    } catch (...) {
        try { execution_observation->CompleteMemory(token, false, 0, ExecutionContext(callback), "non_standard_exception"); }
        catch (...) { execution_observation->MarkIncomplete("memory_exception_record_failure"); }
        throw;
    }
    try { execution_observation->CompleteMemory(token, true, 0, ExecutionContext(callback)); }
    catch (...) {
        execution_observation->MarkIncomplete("memory_completion_record_failure");
        if (!observation_failure) observation_failure = std::current_exception();
    }
    if (observation_failure) std::rethrow_exception(observation_failure);
}
template<class WriteOperation> void StaticArmBackend::ObserveWrite(
    Context* callback_context, u32 address, u32 width, u32 value, WriteOperation&& operation) {
    if (!write_observation) { operation(); return; }
    ++instruction_context.callback_ordinal;
    const auto instruction = instruction_context;
    const auto callback_entry = ObservationContext();
    const auto actual_table = memory.GetCurrentPageTable();
    const auto pending = write_observation->BeginWrite(actual_table, address, width, value,
                                                      instruction, actual_table == page_table);
    if (!pending.relevant) { operation(); return; }
    const auto matches = [&] {
        return callback_context == &context && instruction.valid && instruction_context.valid &&
               instruction.generation == instruction_context.generation &&
               instruction.callback_ordinal == instruction_context.callback_ordinal &&
               instruction.address == current_instruction;
    };
    try {
        operation();
    } catch (...) {
        const auto after_store = ObservationContext();
        const auto after_table = memory.GetCurrentPageTable();
        try {
            write_observation->CompleteWrite(pending, after_table, GetID(), instruction, matches(),
                                              callback_entry, after_store, false, after_table == page_table);
        } catch (...) {}
        throw;
    }
    const auto after_store = ObservationContext();
    const auto after_table = memory.GetCurrentPageTable();
    write_observation->CompleteWrite(pending, after_table, GetID(), instruction, matches(),
                                      callback_entry, after_store, true, after_table == page_table);
}
void StaticArmBackend::Write8(Context* c, u32 a, u8 v) {
    auto& cpu = backend(c);
    cpu.ObserveWrite(c, a, 1, v, [&] { cpu.ObserveExecutionWrite(c, a, 1, v, [&] { cpu.memory.Write8(a, v); }); });
}
void StaticArmBackend::Write16(Context* c, u32 a, u16 v) {
    auto& cpu = backend(c);
    cpu.ObserveWrite(c, a, 2, v, [&] { cpu.ObserveExecutionWrite(c, a, 2, v, [&] { cpu.memory.Write16(a, v); }); });
}
void StaticArmBackend::Write32(Context* c, u32 a, u32 v) {
    auto& cpu = backend(c);
    if (cpu.trace) cpu.trace->RecordWrite(cpu.GetID(), cpu.current_instruction, a, v, *c);
    cpu.ObserveWrite(c, a, 4, v, [&] { cpu.ObserveExecutionWrite(c, a, 4, v, [&] { cpu.memory.Write32(a, v); }); });
}
void StaticArmBackend::RefuseInterpretation(Context* callback, u32 address, u32 opcode) {
    auto& cpu = backend(callback);
    try { cpu.ExecutionBoundary("unsupported_interpreter_request"); }
    catch (...) { if (cpu.execution_observation) cpu.execution_observation->MarkIncomplete("unsupported_visibility_record_failure"); }
    throw std::runtime_error("static CPU interpreter fallback refused at " + std::to_string(address) + " opcode " + std::to_string(opcode));
}
Code StaticArmBackend::Lookup(Context* c, u32 a) {
    auto& cpu = backend(c);
    if (!cpu.schedule->ResolveBoundary(*c, a, cpu.timer->GetDowncount(), cpu.reschedule)) {
        cpu.ExecutionBoundary("lookup_scheduling_yield"); return stopDispatch;
    }
    cpu.ExecutionBoundary("lookup_boundary_observed", false);
    return cpu.FindCode(a);
}
Code StaticArmBackend::FindCode(u32 address) const {
    static thread_local std::array<u32, 1024> cached_indices{};
    auto& cached_index = cached_indices[((address >> 1) ^ (address >> 11)) & 1023];
    if (cached_index < entry_count && entries[cached_index].address == address)
        return entries[cached_index].code;
    auto* end = entries + entry_count;
    auto* found = std::lower_bound(entries, end, address, [](const Entry& entry, u32 value) { return entry.address < value; });
    if (found == end || found->address != address) return nullptr;
    cached_index = static_cast<u32>(found - entries);
    return found->code;
}
}
