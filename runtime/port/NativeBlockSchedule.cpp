#include "NativeBlockSchedule.h"
#include <algorithm>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>

namespace Port {
namespace {
constexpr std::uint32_t ModeMask = 0x07F70000;
constexpr std::uint32_t Unsupported = 0x80000000;
constexpr char OriginalHash[] = "e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64";
std::uint32_t Read32(std::istream& input) {
    unsigned char bytes[4];
    if (!input.read(reinterpret_cast<char*>(bytes), 4)) throw std::runtime_error("truncated block schedule");
    return std::uint32_t(bytes[0]) | std::uint32_t(bytes[1]) << 8 |
           std::uint32_t(bytes[2]) << 16 | std::uint32_t(bytes[3]) << 24;
}
std::uint64_t Read64(std::istream& input) {
    const auto low = Read32(input), high = Read32(input);
    return low | (std::uint64_t(high) << 32);
}
}

NativeBlockSchedule::NativeBlockSchedule(const std::filesystem::path& path) {
    std::ifstream input(path, std::ios::binary);
    char magic[8], hash[64];
    if (!input.read(magic, 8) || std::string(magic, 8) != "SM3DBS02")
        throw std::runtime_error("invalid block schedule signature");
    const auto version = Read32(input), record_count = Read32(input), terminal_count = Read32(input);
    if (version != 2 || Read32(input) != 80 || Read32(input) != 32 ||
        record_count == 0 || record_count > 4000000 || terminal_count > 16000000)
        throw std::runtime_error("unsupported block schedule extent or revision");
    if (!input.read(hash, 64) || std::string(hash, 64) != OriginalHash || Read32(input) != 0)
        throw std::runtime_error("block schedule original-image identity differs");
    const auto expected_size = 96ULL + 80ULL * record_count + 32ULL * terminal_count;
    if (std::filesystem::file_size(path) != expected_size)
        throw std::runtime_error("block schedule file extent differs");
    records.reserve(record_count);
    for (std::uint32_t index = 0; index < record_count; ++index) {
        Record record{};
        // Read fields explicitly. Object padding and host endianness are irrelevant.
        record.address = Read32(input); record.mode = Read32(input);
        record.end_address = Read32(input); record.condition = Read32(input);
        record.failed_address = Read32(input); record.pass_cycles = Read32(input);
        record.failure_cycles = Read32(input); record.instruction_count = Read32(input);
        record.flags = Read32(input); record.push_address = Read32(input);
        record.end_descriptor = Read64(input); record.failed_descriptor = Read64(input);
        record.push_descriptor = Read64(input); record.failure_instruction_count = Read32(input);
        record.node_offset = Read32(input); record.node_count = Read32(input); record.reserved = Read32(input);
        if (record.mode & ~ModeMask || record.condition > 15 || record.reserved ||
            (!(record.flags & Unsupported) && record.instruction_count == 0) || record.node_count == 0 || record.node_count > 64 ||
            std::uint64_t(record.node_offset) + record.node_count > terminal_count ||
            (index && records.back().Key() >= record.Key()))
            throw std::runtime_error("invalid or unsorted block schedule record");
        records.push_back(record);
    }
    terminals.reserve(terminal_count);
    for (std::uint32_t index = 0; index < terminal_count; ++index) {
        Terminal terminal{};
        terminal.kind = Read32(input); terminal.condition = Read32(input); terminal.next = Read64(input);
        terminal.then_index = Read32(input); terminal.else_index = Read32(input);
        terminal.interpreter_count = Read32(input); terminal.reserved = Read32(input);
        if (terminal.kind > 11 || (terminal.kind == 8 && terminal.condition > 15) || terminal.reserved)
            throw std::runtime_error("invalid block schedule terminal");
        terminals.push_back(terminal);
    }
    for (const auto& record : records) {
        for (std::uint32_t index = 0; index < record.node_count; ++index) {
            const auto& terminal = terminals[record.node_offset + index];
            // Children must follow parents. This also excludes cycles.
            const auto valid_child = [&](std::uint32_t child) { return child > index && child < record.node_count; };
            if (((terminal.kind == 8 || terminal.kind == 9) && !valid_child(terminal.then_index)) ||
                ((terminal.kind == 6 || terminal.kind == 8 || terminal.kind == 9) && !valid_child(terminal.else_index)))
                throw std::runtime_error("invalid block schedule terminal edge");
        }
    }
}

void NativeBlockSchedule::BeginRun() {
    if (active || pending_ticks) throw std::runtime_error("unfinished previous native scheduling slice");
    return_stack = {}; return_pointer = 0;
}
void NativeBlockSchedule::ClearVisited() { visited.clear(); }
std::uint64_t NativeBlockSchedule::TakePendingTicks() {
    const auto ticks = pending_ticks; pending_ticks = 0; return ticks;
}
std::uint64_t NativeBlockSchedule::Descriptor(const Context& context, std::uint32_t address) {
    return std::uint64_t(*context.fpscr & ModeMask) << 32 |
           std::uint64_t(address & 1) << 32 | (address & ~std::uint32_t(1));
}
const NativeBlockSchedule::Record& NativeBlockSchedule::Find(std::uint32_t address, std::uint32_t mode) const {
    const auto key = (std::uint64_t(mode) << 32) | address;
    // Validate indices against this table. Reused object or allocation addresses
    // cannot retain a descriptor or pointer from another module.
    static thread_local std::array<std::uint32_t, 1024> cached_indices{};
    auto& cached_index = cached_indices[((address >> 1) ^ (address >> 11) ^ (mode >> 16)) & 1023];
    const Record* found;
    if (cached_index < records.size() && records[cached_index].Key() == key) {
        found = &records[cached_index];
    } else {
        const auto position = std::lower_bound(records.begin(), records.end(), key,
            [](const Record& record, std::uint64_t target) { return record.Key() < target; });
        if (position == records.end() || position->Key() != key)
            throw std::runtime_error("missing native scheduling descriptor at " + std::to_string(address));
        cached_index = static_cast<std::uint32_t>(position - records.begin());
        found = &*position;
    }
    if (found->flags & Unsupported)
        throw std::runtime_error("unsupported native scheduling terminal at " + std::to_string(address));
    return *found;
}
bool NativeBlockSchedule::Condition(const Context& context, std::uint32_t condition) {
    switch (condition) {
    case 0: return context.z; case 1: return !context.z;
    case 2: return context.c; case 3: return !context.c;
    case 4: return context.n; case 5: return !context.n;
    case 6: return context.v; case 7: return !context.v;
    case 8: return context.c && !context.z; case 9: return !context.c || context.z;
    case 10: return context.n == context.v; case 11: return context.n != context.v;
    case 12: return !context.z && context.n == context.v; case 13: return context.z || context.n != context.v;
    case 14: return true;
    default: throw std::runtime_error("unsupported scheduling condition NV");
    }
}
bool NativeBlockSchedule::TerminalPermits(const Record& record, std::uint32_t index,
                                        const Context& context, std::uint64_t next,
                                        std::int64_t downcount, bool halted) {
    const auto& terminal = terminals[record.node_offset + index];
    const bool time_remaining = downcount > static_cast<std::int64_t>(pending_ticks);
    switch (terminal.kind) {
    case 1: // Checked links test cycles even when their target is already linked.
    case 2:
        if (terminal.next != next) throw std::runtime_error("native branch disagrees with scheduling terminal");
        return (terminal.kind == 2 && visited.contains(next)) ||
               (time_remaining && (visited.contains(next) || !halted));
    case 3: {
        const auto entry = return_stack[return_pointer];
        return_pointer = (return_pointer + 7) & 7;
        return (entry.direct && entry.descriptor == next) || (time_remaining && !halted);
    }
    case 4: case 5: return time_remaining && !halted;
    case 6:
        return !halted && TerminalPermits(record, terminal.else_index, context, next, downcount, halted);
    case 8:
        return TerminalPermits(record, Condition(context, terminal.condition) ? terminal.then_index : terminal.else_index,
                               context, next, downcount, halted);
    default: throw std::runtime_error("unsupported native scheduling terminal node");
    }
}
bool NativeBlockSchedule::Complete(Context& context, std::uint32_t next_address,
                                   std::int64_t downcount, bool halted) {
    if (!active) return downcount > static_cast<std::int64_t>(pending_ticks) && !halted;
    if (remaining) throw std::runtime_error("native control left a stock block before its last instruction");
    const auto* completed = active; active = nullptr;
    instructions += completed->instruction_count;
    pending_ticks += completed->pass_cycles;
    return TerminalPermits(*completed, 0, context, Descriptor(context, next_address), downcount, halted);
}
bool NativeBlockSchedule::BeforeInstruction(Context& context, std::uint32_t address,
                                           std::uint32_t count, std::int64_t downcount, bool halted) {
    const auto tagged = address | context.thumb;
    if (!ResolveBoundary(context, tagged, downcount, halted)) return false;
    if (!active) {
        const auto& record = Find(tagged, *context.fpscr & ModeMask);
        visited.insert(Descriptor(context, tagged));
        if (!Condition(context, record.condition)) {
            if (!record.failed_address || record.failure_instruction_count == 0)
                throw std::runtime_error("missing conditional-failure scheduling path");
            pending_ticks += record.failure_cycles;
            instructions += record.failure_instruction_count;
            context.thumb = record.failed_address & 1;
            context.r[15] = record.failed_address & ~std::uint32_t(1);
            context.exit = downcount > static_cast<std::int64_t>(pending_ticks) &&
                           (visited.contains(record.failed_descriptor) || !halted) ? EXIT_UNWIND : EXIT_BUDGET;
            return false;
        }
        active = &record; remaining = record.instruction_count;
        if (record.push_descriptor) {
            return_pointer = (return_pointer + 1) & 7;
            return_stack[return_pointer] = {record.push_descriptor, visited.contains(record.push_descriptor)};
        }
    }
    if (count != 1 || count > remaining)
        throw std::runtime_error("atomic source replacement needs a resumable scheduling adapter at " + std::to_string(address));
    --remaining;
    return true;
}
bool NativeBlockSchedule::ResolveBoundary(Context& context, std::uint32_t next_address,
                                          std::int64_t downcount, bool halted) {
    if (active && remaining == 0 && !Complete(context, next_address, downcount, halted)) {
        context.r[15] = next_address & ~std::uint32_t(1); context.thumb = next_address & 1;
        context.exit = EXIT_BUDGET; return false;
    }
    return true;
}
void NativeBlockSchedule::ChargePriorityReplacement(Context& context, std::uint32_t count,
                                                   std::int64_t downcount, bool halted) {
    // This is scheduling metadata for the one certified scalar source binding.
    // The unchanged C++ computes the result afterward. No original opcode is read.
    // A slice ending inside this atomic adapter is explicitly unsupported.
    Context shadow = context;
    const auto input = context.r[0];
    const auto before = Instructions();
    auto address = std::uint32_t(0x0010766C);
    for (;;) {
        shadow.exit = EXIT_NONE;
        if (!BeforeInstruction(shadow, address, 1, downcount, halted)) {
            if (shadow.exit == EXIT_UNWIND) { address = shadow.r[15]; continue; }
            if (Instructions() == before) {
                context.r[15] = shadow.r[15]; context.exit = shadow.exit; return;
            }
            throw std::runtime_error("priority source replacement crossed a scheduling slice; resumable adapter required");
        }
        if (address == 0x0010766C || address == 0x0010767C) {
            const auto operand = address == 0x0010766C ? 32u : 24u;
            const auto result = input - operand;
            shadow.n = result >> 31; shadow.z = result == 0; shadow.c = input >= operand;
            shadow.v = ((input ^ operand) & (input ^ result)) >> 31;
        }
        if (address == 0x00107678 || address == 0x00107690) break;
        address = address == 0x00107670 && static_cast<std::int32_t>(input) < 32 ? 0x0010767C : address + 4;
    }
    if (Instructions() - before != count || remaining)
        throw std::runtime_error("priority source scheduling path extent differs");
    context.n = shadow.n; context.z = shadow.z; context.c = shadow.c; context.v = shadow.v;
}
}
