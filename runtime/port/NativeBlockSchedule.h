// Scheduling metadata is derived before linking. This runtime never decodes ARM.
#pragma once
#include <array>
#include <cstdint>
#include <filesystem>
#include <unordered_set>
#include <vector>
#include "recomp.h"

namespace Port {
class NativeBlockSchedule {
public:
    explicit NativeBlockSchedule(const std::filesystem::path& path);
    void BeginRun();
    bool BeforeInstruction(Context& context, std::uint32_t address,
                           std::uint32_t count, std::int64_t downcount, bool halted);
    bool ContinueInstruction() {
        if (!active || remaining == 0) return false;
        --remaining;
        return true;
    }
    bool Complete(Context& context, std::uint32_t next_address,
                  std::int64_t downcount, bool halted);
    bool ResolveBoundary(Context& context, std::uint32_t next_address,
                         std::int64_t downcount, bool halted);
    void ChargePriorityReplacement(Context& context, std::uint32_t count,
                                   std::int64_t downcount, bool halted);
    std::uint64_t TakePendingTicks();
    std::uint64_t Instructions() const {
        return instructions + (active ? active->instruction_count - remaining : 0);
    }
    void ClearVisited();
private:
    struct Record {
        std::uint32_t address, mode, end_address, condition, failed_address;
        std::uint32_t pass_cycles, failure_cycles, instruction_count, flags, push_address;
        std::uint64_t end_descriptor, failed_descriptor, push_descriptor;
        std::uint32_t failure_instruction_count, node_offset, node_count;
        // The serialized reserved word must be zero. Reuse that storage for
        // runtime visitation without changing the record or scheduler layout.
        mutable std::uint32_t reserved;
        std::uint64_t Key() const { return (std::uint64_t(mode) << 32) | address; }
    };
    struct Terminal {
        std::uint32_t kind, condition;
        std::uint64_t next;
        std::uint32_t then_index, else_index, interpreter_count, reserved;
    };
    struct ReturnEntry { std::uint64_t descriptor = 0; bool direct = false; };
    const Record& Find(std::uint32_t address, std::uint32_t mode) const;
    bool TerminalPermits(const Record& record, std::uint32_t index, const Context& context,
                         std::uint64_t next, std::int64_t downcount, bool halted);
    static bool Condition(const Context& context, std::uint32_t condition);
    static std::uint64_t Descriptor(const Context& context, std::uint32_t address);
    std::vector<Record> records;
    std::vector<Terminal> terminals;
    std::unordered_set<std::uint64_t> visited;
    std::array<ReturnEntry, 8> return_stack{};
    std::uint32_t return_pointer = 0;
    const Record* active = nullptr;
    std::uint32_t remaining = 0;
    std::uint64_t pending_ticks = 0, instructions = 0;
};
}
