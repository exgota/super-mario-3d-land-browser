// Bounded passive observations of externally selected guest execution windows.
#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <memory>
#include <mutex>
#include <string>
#include <vector>
#include "GuestWriteObservation.h"
#include "core/arm/skyeye_common/arm_regformat.h"

namespace Port {
class GuestExecutionObservation {
public:
    struct Snapshot {
        GuestWriteObservation::MachineContext machine;
        std::array<std::uint32_t, CP15_REGISTER_COUNT> coprocessor_registers{};
        std::uint32_t coprocessor_count = 0, cpsr_control = 0;
        std::array<std::uint8_t, 7> condition_fields{};
        std::int32_t budget = 0;
        std::uint32_t exit = 0, supervisor_call = 0, dispatch_depth = 0;
        bool reschedule = false, break_requested = false, callback_matches_backend = false;
        bool writer_enabled = false, writer_charge_valid = false;
        std::uint64_t writer_context = 0, writer_generation = 0, writer_callback_ordinal = 0;
        std::shared_ptr<Memory::PageTable> actual_table, backend_table;
        std::uint64_t actual_table_identity = 0, backend_table_identity = 0;
    };
    struct RamBytes {
        std::uint32_t address = 0, width = 0, value = 0;
        std::array<std::uint8_t, 4> bytes{};
        std::array<const std::uint8_t*, 4> locations{};
        bool valid = false;
        const char* reason = "not_requested";
    };
    struct MemoryToken {
        bool collected = false, write = false, associated = false;
        std::uint64_t context = 0, charge = 0, ordinal = 0, begin_order = 0, segment = 0;
        std::uint32_t address = 0, width = 0, supplied_value = 0;
        Snapshot before;
        RamBytes instruction_before, destination_before;
    };

    static std::shared_ptr<GuestExecutionObservation> FromEnvironment();
    ~GuestExecutionObservation();
    std::uint64_t RegisterContext(std::uint32_t processor, const void* backend,
                                  const void* translated_context, std::uint64_t writer_context);
    bool Collecting() const noexcept;
    void BeginCharge(std::uint64_t context, std::uint32_t address, std::uint32_t span,
                     std::uint64_t supplied_ticks, const Snapshot& entry);
    void Admission(std::uint64_t context, bool admitted, bool raised,
                   const Snapshot& after, const char* exception = nullptr);
    MemoryToken BeginMemory(std::uint64_t context, bool write, std::uint32_t address,
                            std::uint32_t width, std::uint32_t supplied_value, const Snapshot& before);
    void CompleteMemory(const MemoryToken& token, bool returned, std::uint32_t returned_value,
                        const Snapshot& after, const char* exception = nullptr);
    void Boundary(std::uint64_t context, const char* reason, const Snapshot& state,
                  bool invalidate_charge = true);
    void MarkIncomplete(const char* reason) noexcept;
    bool Finish(bool capture_succeeded);

private:
    struct Record {
        const char* kind = "boundary";
        const char* reason = "observed_boundary";
        std::uint64_t order = 0, end_order = 0, context = 0, charge = 0, segment = 0, ordinal = 0;
        std::uint32_t address = 0, width = 0, supplied_value = 0, returned_value = 0;
        bool admitted = false, raised = false, write = false, associated = false, returned = false;
        bool exception_text_truncated = false;
        Snapshot before, after;
        RamBytes instruction_before, instruction_after, destination_before, destination_after;
        std::string exception;
    };
    struct Accounting { std::uint64_t records = 0; };
    struct Charge {
        ~Charge();
        std::shared_ptr<Accounting> accounting;
        std::uint64_t identity = 0, context = 0, generation = 0, segment = 0;
        std::uint64_t supplied_ticks = 0;
        std::uint32_t address = 0, span = 0, callbacks = 0;
        bool admission_recorded = false, admitted = false;
        std::deque<Record> records;
    };
    struct BackendContext {
        std::uint32_t processor = 0;
        std::uint64_t generation = 0, segment = 0, actual_table_identity = 0;
        bool charge_valid = false;
        bool table_seen = false;
        std::shared_ptr<Charge> current;
    };
    struct TableIdentityRecord {
        std::weak_ptr<Memory::PageTable> owner;
        const Memory::PageTable* object = nullptr;
    };
    GuestExecutionObservation(const char* path, std::uint32_t trigger,
                              std::uint64_t prehistory, std::uint64_t posthistory,
                              std::uint64_t windows, std::uint64_t events, std::uint64_t bytes);
    static RamBytes ReadRam(const std::shared_ptr<Memory::PageTable>& table,
                            std::uint32_t address, std::uint32_t width) noexcept;
    static RamBytes ReadInstruction(const std::shared_ptr<Memory::PageTable>& table,
                                    std::uint32_t address, const Snapshot& state) noexcept;
    Snapshot SealSnapshot(const Snapshot& state);
    std::uint64_t TableIdentity(const std::shared_ptr<Memory::PageTable>& table);
    BackendContext& Context(std::uint64_t context);
    void Add(const std::shared_ptr<Charge>& charge, Record record);
    void DetectTableChange(std::uint64_t context, const Snapshot& state);
    void CloseWindow(const char* reason, bool extent_complete);
    std::string Serialize(const Record& record, const Charge& charge, const char* scope) const;
    void WriteRecord(const std::string& record);
    [[noreturn]] void Fail(const char* reason);
    std::FILE* output = nullptr;
    std::uint32_t trigger;
    std::uint64_t prehistory_limit, posthistory_limit, window_limit, event_limit, byte_limit;
    std::uint64_t bytes_written = 0, output_records = 0, observation_order = 0, charge_order = 0;
    std::uint64_t windows_started = 0, windows_completed = 0, post_remaining = 0;
    std::uint64_t history_evictions = 0, trigger_observations = 0, overlapping_triggers = 0;
    std::uint64_t omitted_records = 0, outside_charge_callbacks = 0, outside_charge_boundaries = 0;
    std::uint64_t failed_memory_operations = 0;
    std::uint64_t trigger_charge = 0, trigger_order = 0, evictions_at_trigger = 0;
    std::uint64_t last_completed_window_charge = 0;
    Snapshot closing_boundary;
    RamBytes closing_instruction;
    std::uint64_t closing_context = 0, closing_order = 0;
    std::uint32_t closing_address = 0, closing_span = 0;
    bool closing_boundary_available = false;
    std::shared_ptr<Accounting> accounting = std::make_shared<Accounting>();
    std::vector<BackendContext> contexts;
    std::vector<TableIdentityRecord> tables;
    std::deque<std::shared_ptr<Charge>> history;
    std::vector<std::shared_ptr<Charge>> window;
    mutable std::mutex mutex;
    const char* failure = nullptr;
    bool collecting = true, window_open = false, finished = false, complete = false, interrupted = false;
};
}
