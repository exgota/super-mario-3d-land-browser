// Passive observations of runtime-selected guest virtual RAM writes.
#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>

namespace Memory { struct PageTable; }

namespace Port {
class GuestWriteObservation {
public:
    struct MachineContext {
        std::array<std::uint32_t, 16> registers{};
        std::array<std::uint32_t, 64> floating_registers{};
        std::uint32_t cpsr = 0, fpscr = 0, fpexc = 0, tls = 0;
        std::uint32_t exclusive_address = 0, exclusive = 0;
        std::uint64_t timer_ticks = 0, scheduled_instructions = 0;
        std::int64_t timer_downcount = 0;
    };
    struct InstructionContext {
        bool valid = false;
        std::uint32_t address = 0, span = 0;
        std::uint64_t generation = 0, context_identity = 0, callback_ordinal = 0;
        bool backend_table_matches = false;
        std::shared_ptr<Memory::PageTable> table;
        MachineContext entry;
    };
    struct RamRead {
        bool valid = false;
        const char* reason = "not_requested";
        std::uint32_t address = 0, width = 0, value = 0;
        std::array<std::uint8_t, 4> bytes{};
        // Host pointers are used only for mapping comparisons, never emitted.
        std::array<const std::uint8_t*, 4> locations{};
    };
    struct RootSlotState {
        RamRead root;
        RamRead root_recheck;
        bool slot_address_valid = false, coherent = false;
        const char* slot_reason = "root_unavailable";
        RamRead slot;
    };
    struct PendingWrite {
        bool relevant = false, root_overlap = false, slot_overlap = false;
        bool backend_table_matches = false;
        std::uint32_t address = 0, width = 0, value = 0;
        std::uint64_t callback_sequence = 0;
        std::shared_ptr<Memory::PageTable> table;
        RootSlotState before;
        RamRead destination_before, instruction_before;
    };

    static std::shared_ptr<GuestWriteObservation> FromEnvironment();
    ~GuestWriteObservation();
    std::uint64_t RegisterContext() noexcept { return ++contexts; }
    PendingWrite BeginWrite(const std::shared_ptr<Memory::PageTable>& table,
                            std::uint32_t address, std::uint32_t width, std::uint32_t value,
                            const InstructionContext& instruction, bool backend_table_matches) noexcept;
    void CompleteWrite(const PendingWrite& pending, const std::shared_ptr<Memory::PageTable>& table,
                       std::uint32_t processor, const InstructionContext& instruction,
                       bool matching_callback, const MachineContext& callback_entry,
                       const MachineContext& after_store, bool write_returned,
                       bool backend_table_matches);
    // A complete footer means no observation records were lost. Per-field
    // validity and byte equality remain separate evidence requirements.
    bool Finish(bool capture_succeeded);

private:
    GuestWriteObservation(const char* filename, std::uint32_t root_address,
                          std::uint32_t slot_offset, std::uint64_t event_limit,
                          std::uint64_t byte_limit);
    static RamRead ReadRam(const std::shared_ptr<Memory::PageTable>& table,
                           std::uint32_t address, std::uint32_t width) noexcept;
    RootSlotState ReadRootSlot(const std::shared_ptr<Memory::PageTable>& table) const noexcept;
    static RamRead ReadInstruction(const std::shared_ptr<Memory::PageTable>& table,
                                  const InstructionContext& instruction) noexcept;
    void WriteRecord(const std::string& record);
    void Fail(const char* reason);
    std::FILE* output = nullptr;
    std::uint32_t root_address, slot_offset;
    std::uint64_t event_limit, byte_limit, bytes_written = 0;
    std::uint64_t callbacks = 0, events = 0, omitted_events = 0;
    std::uint64_t contexts = 0;
    std::uint64_t unavailable_root_callbacks = 0, unavailable_slot_callbacks = 0;
    std::uint64_t confirmed_bytes = 0, same_value_stores = 0, unavailable_events = 0;
    std::uint64_t imprecise_events = 0, failed_writes = 0;
    const char* failure = nullptr;
    bool finished = false, complete = false, record_interrupted = false;
};
}
