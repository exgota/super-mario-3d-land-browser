#include "GuestWriteObservation.h"
#include <charconv>
#include <cstdlib>
#include <fcntl.h>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unistd.h>
#include "core/memory.h"

namespace Port {
namespace {
constexpr std::uint64_t AddressExtent = std::uint64_t(1) << 32;
constexpr std::uint64_t FooterReserve = 2048;
constexpr std::uint64_t DefaultEventLimit = 4096, MaximumEventLimit = 100000;
constexpr std::uint64_t DefaultByteLimit = 16 * 1024 * 1024, MaximumByteLimit = 64 * 1024 * 1024;

std::uint64_t ParseUnsigned(const char* text, std::uint64_t maximum, const char* name) {
    std::string_view input(text);
    int base = 10;
    if (input.starts_with("0x") || input.starts_with("0X")) { base = 16; input.remove_prefix(2); }
    std::uint64_t value = 0;
    const auto result = std::from_chars(input.data(), input.data() + input.size(), value, base);
    if (input.empty() || result.ec != std::errc() || result.ptr != input.data() + input.size() || value > maximum)
        throw std::runtime_error(std::string("invalid guest write observation ") + name);
    return value;
}

// A wrapping store remains relevant on either side of the address-space limit,
// but its destination read is invalid and cannot confirm committed bytes.
bool Overlaps(std::uint32_t address, std::uint32_t width, std::uint32_t selected) noexcept {
    const std::uint64_t start = address, end = start + width;
    const std::uint64_t selected_start = selected, selected_end = selected_start + 4;
    return (start < selected_end && selected_start < end) ||
           (end > AddressExtent && selected_start < end - AddressExtent);
}

bool SameMapping(const GuestWriteObservation::RamRead& first,
                 const GuestWriteObservation::RamRead& second) noexcept {
    return first.valid && second.valid && first.address == second.address && first.width == second.width &&
           first.locations == second.locations;
}
bool SameRead(const GuestWriteObservation::RamRead& first,
              const GuestWriteObservation::RamRead& second) noexcept {
    return SameMapping(first, second) && first.bytes == second.bytes;
}
const char* Boolean(bool value) { return value ? "true" : "false"; }
void EmitRead(std::ostream& output, const GuestWriteObservation::RamRead& read) {
    output << "{\"address\":" << read.address << ",\"width\":" << read.width
           << ",\"valid\":" << Boolean(read.valid) << ",\"reason\":\"" << read.reason << "\",\"value\":";
    if (!read.valid) { output << "null,\"bytes\":null}"; return; }
    output << read.value << ",\"bytes\":[";
    for (std::uint32_t index = 0; index < read.width; ++index)
        output << (index ? "," : "") << unsigned(read.bytes[index]);
    output << "]}";
}
void EmitRootSlot(std::ostream& output, const GuestWriteObservation::RootSlotState& state) {
    output << "{\"root\":"; EmitRead(output, state.root);
    output << ",\"root_recheck\":"; EmitRead(output, state.root_recheck);
    output << ",\"slot_address_valid\":" << Boolean(state.slot_address_valid)
           << ",\"slot_reason\":\"" << state.slot_reason << "\",\"slot\":";
    if (state.slot_address_valid) EmitRead(output, state.slot); else output << "null";
    output << ",\"coherent\":" << Boolean(state.coherent) << "}";
}
void EmitContext(std::ostream& output, const GuestWriteObservation::MachineContext& context) {
    output << "{\"registers\":[";
    for (std::size_t index = 0; index < context.registers.size(); ++index)
        output << (index ? "," : "") << context.registers[index];
    output << "],\"cpsr\":" << context.cpsr << ",\"floating_registers\":[";
    for (std::size_t index = 0; index < context.floating_registers.size(); ++index)
        output << (index ? "," : "") << context.floating_registers[index];
    output << "],\"fpscr\":" << context.fpscr << ",\"fpexc\":" << context.fpexc
           << ",\"tls\":" << context.tls << ",\"exclusive\":" << context.exclusive
           << ",\"exclusive_address\":" << context.exclusive_address
           << ",\"timer_ticks\":" << context.timer_ticks << ",\"timer_downcount\":" << context.timer_downcount
           << ",\"scheduled_instructions\":" << context.scheduled_instructions << "}";
}
}

std::shared_ptr<GuestWriteObservation> GuestWriteObservation::FromEnvironment() {
    const char* filename = std::getenv("ROOT_PORT_GUEST_WRITE_PATH");
    const char* root = std::getenv("ROOT_PORT_GUEST_WRITE_ROOT");
    const char* offset = std::getenv("ROOT_PORT_GUEST_WRITE_SLOT_OFFSET");
    const char* events = std::getenv("ROOT_PORT_GUEST_WRITE_EVENT_LIMIT");
    const char* bytes = std::getenv("ROOT_PORT_GUEST_WRITE_BYTE_LIMIT");
    if (!filename && !root && !offset && !events && !bytes) return nullptr;
    if (!filename || !*filename || !root || !offset)
        throw std::runtime_error("guest write observation needs path, root and slot offset");
    const auto root_address = ParseUnsigned(root, UINT32_MAX, "root address");
    if ((root_address & 3) || root_address + 4 > AddressExtent)
        throw std::runtime_error("guest write observation root must be an aligned complete word");
    const auto slot_offset = ParseUnsigned(offset, UINT32_MAX, "slot offset");
    const auto event_limit = events ? ParseUnsigned(events, MaximumEventLimit, "event limit") : DefaultEventLimit;
    const auto byte_limit = bytes ? ParseUnsigned(bytes, MaximumByteLimit, "byte limit") : DefaultByteLimit;
    if (!event_limit || byte_limit < 8192)
        throw std::runtime_error("guest write observation limits need at least one event and 8192 bytes");
    return std::shared_ptr<GuestWriteObservation>(new GuestWriteObservation(
        filename, static_cast<std::uint32_t>(root_address), static_cast<std::uint32_t>(slot_offset),
        event_limit, byte_limit));
}

GuestWriteObservation::GuestWriteObservation(const char* filename, std::uint32_t root_address_,
                                           std::uint32_t slot_offset_, std::uint64_t event_limit_,
                                           std::uint64_t byte_limit_)
    : root_address(root_address_), slot_offset(slot_offset_), event_limit(event_limit_), byte_limit(byte_limit_) {
    const int descriptor = open(filename, O_WRONLY | O_CREAT | O_EXCL, 0600);
    if (descriptor < 0) throw std::runtime_error("cannot exclusively create guest write observation output");
    output = fdopen(descriptor, "w");
    if (!output) { close(descriptor); throw std::runtime_error("cannot open guest write observation stream"); }
    try {
        std::ostringstream record;
        record << "{\"type\":\"configuration\",\"version\":1,\"root_address\":" << root_address
               << ",\"slot_offset\":" << slot_offset << ",\"event_limit\":" << event_limit
               << ",\"byte_limit\":" << byte_limit << ",\"byte_order\":\"little_endian\"}\n";
        WriteRecord(record.str());
    } catch (...) {
        try { Finish(false); } catch (...) {}
        std::fclose(output); output = nullptr;
        throw;
    }
}
GuestWriteObservation::~GuestWriteObservation() {
    if (!finished) { try { Finish(false); } catch (...) {} }
    if (output) std::fclose(output);
}

GuestWriteObservation::RamRead GuestWriteObservation::ReadRam(
    const std::shared_ptr<Memory::PageTable>& table, std::uint32_t address, std::uint32_t width) noexcept {
    RamRead result;
    result.address = address; result.width = width;
    if (!width || width > result.bytes.size() || std::uint64_t(address) + width > AddressExtent) {
        result.reason = "address_extent"; return result;
    }
    if (!table) { result.reason = "no_current_page_table"; return result; }
    auto& pointers = table->GetPointerArray();
    // Validate every byte before dereferencing any byte, including page crossings.
    for (std::uint32_t index = 0; index < width; ++index) {
        const std::uint32_t byte_address = address + index;
        const auto page = byte_address >> Memory::CITRA_PAGE_BITS;
        if (table->attributes[page] != Memory::PageType::Memory) {
            result.reason = "not_direct_memory"; return result;
        }
        if (!pointers[page]) { result.reason = "null_page_pointer"; return result; }
        result.locations[index] = pointers[page] + (byte_address & Memory::CITRA_PAGE_MASK);
    }
    for (std::uint32_t index = 0; index < width; ++index) {
        result.bytes[index] = *result.locations[index];
        result.value |= std::uint32_t(result.bytes[index]) << (index * 8);
    }
    result.valid = true; result.reason = "valid";
    return result;
}

GuestWriteObservation::RootSlotState GuestWriteObservation::ReadRootSlot(
    const std::shared_ptr<Memory::PageTable>& table) const noexcept {
    RootSlotState result;
    result.root = ReadRam(table, root_address, 4);
    if (result.root.valid) {
        const std::uint64_t slot_address = std::uint64_t(result.root.value) + slot_offset;
        if (!result.root.value) result.slot_reason = "null_root";
        else if (slot_address + 4 > AddressExtent) result.slot_reason = "slot_address_extent";
        else {
            result.slot_address_valid = true; result.slot_reason = "valid_address";
            result.slot = ReadRam(table, static_cast<std::uint32_t>(slot_address), 4);
        }
    }
    result.root_recheck = ReadRam(table, root_address, 4);
    result.coherent = SameRead(result.root, result.root_recheck) && result.slot_address_valid && result.slot.valid;
    return result;
}

GuestWriteObservation::RamRead GuestWriteObservation::ReadInstruction(
    const std::shared_ptr<Memory::PageTable>& table, const InstructionContext& instruction) noexcept {
    if (!instruction.valid) { RamRead result; result.reason = "no_charge_context"; return result; }
    const bool thumb = (instruction.entry.cpsr & 0x20) != 0;
    if (!thumb && (instruction.address & 3u)) {
        RamRead result; result.address = instruction.address; result.width = 4;
        result.reason = "instruction_alignment"; return result;
    }
    // Four raw bytes are a prefix only. Thumb instruction length is not decoded.
    return ReadRam(table, thumb ? instruction.address & ~std::uint32_t(1) : instruction.address, 4);
}

GuestWriteObservation::PendingWrite GuestWriteObservation::BeginWrite(
    const std::shared_ptr<Memory::PageTable>& table, std::uint32_t address, std::uint32_t width,
    std::uint32_t value, const InstructionContext& instruction, bool backend_table_matches) noexcept {
    PendingWrite result;
    result.callback_sequence = ++callbacks;
    result.table = table; result.address = address; result.width = width; result.value = value;
    result.backend_table_matches = backend_table_matches;
    result.before = ReadRootSlot(table);
    if (!SameRead(result.before.root, result.before.root_recheck)) ++unavailable_root_callbacks;
    if (!result.before.slot_address_valid || !result.before.slot.valid) ++unavailable_slot_callbacks;
    result.root_overlap = Overlaps(address, width, root_address);
    result.slot_overlap = result.before.slot_address_valid && Overlaps(address, width, result.before.slot.address);
    result.relevant = result.root_overlap || result.slot_overlap;
    if (result.relevant) {
        result.destination_before = ReadRam(table, address, width);
        result.instruction_before = ReadInstruction(table, instruction);
    }
    return result;
}

void GuestWriteObservation::CompleteWrite(
    const PendingWrite& pending, const std::shared_ptr<Memory::PageTable>& table,
    std::uint32_t processor, const InstructionContext& instruction, bool matching_callback,
    const MachineContext& callback_entry, const MachineContext& after_store, bool write_returned,
    bool backend_table_matches) {
    if (!pending.relevant) return;
    const auto destination_after = ReadRam(table, pending.address, pending.width);
    const auto after = ReadRootSlot(table);
    RamRead previous_slot_after;
    if (pending.before.slot_address_valid) previous_slot_after = ReadRam(table, pending.before.slot.address, 4);
    const auto instruction_after = ReadInstruction(table, instruction);
    const bool same_table = pending.table == table;
    const bool instruction_table_matches = instruction.valid && instruction.table == pending.table;
    const bool precise_context = instruction.valid && instruction.span == 1 && matching_callback &&
                                 instruction_table_matches && instruction.backend_table_matches &&
                                 pending.backend_table_matches && backend_table_matches && same_table;
    const bool destination_mapping_stable = same_table && SameMapping(pending.destination_before, destination_after);
    const bool destination_matches = destination_after.valid && destination_after.value == pending.value;
    const bool committed_bytes = write_returned && destination_mapping_stable && destination_matches;
    const bool same_value = committed_bytes && pending.destination_before.value == pending.value;
    const bool root_mapping_stable = same_table && SameMapping(pending.before.root, after.root);
    const bool previous_slot_mapping_stable = same_table && SameMapping(pending.before.slot, previous_slot_after);
    const bool coherent_join = write_returned && same_table && pending.backend_table_matches && backend_table_matches &&
                               root_mapping_stable && previous_slot_mapping_stable && pending.before.coherent &&
                               after.coherent && destination_mapping_stable;
    if (!write_returned) ++failed_writes;
    if (finished || failure) { ++omitted_events; Fail(finished ? "write_after_footer" : failure); }
    if (events >= event_limit) { ++omitted_events; Fail("event_limit_exceeded"); }
    std::ostringstream record;
    record << "{\"type\":\"write\",\"sequence\":" << events + 1
           << ",\"callback_sequence\":" << pending.callback_sequence << ",\"processor\":" << processor
           << ",\"context_identity\":" << instruction.context_identity << ",\"address\":" << pending.address
           << ",\"width\":" << pending.width << ",\"supplied_value\":" << pending.value
           << ",\"root_overlap\":" << Boolean(pending.root_overlap) << ",\"slot_overlap\":" << Boolean(pending.slot_overlap)
           << ",\"write_returned\":" << Boolean(write_returned) << ",\"destination_matches_supplied_bytes\":";
    if (destination_after.valid) record << Boolean(destination_matches); else record << "null";
    record << ",\"committed_bytes_observed\":" << Boolean(committed_bytes)
           << ",\"same_value_store_observed\":" << Boolean(same_value)
           << ",\"same_actual_page_table\":" << Boolean(same_table)
           << ",\"backend_table_matches_callback_entry\":" << Boolean(pending.backend_table_matches)
           << ",\"backend_table_matches_after_store\":" << Boolean(backend_table_matches)
           << ",\"root_mapping_stable\":" << Boolean(root_mapping_stable)
           << ",\"previous_slot_mapping_stable\":" << Boolean(previous_slot_mapping_stable)
           << ",\"destination_mapping_stable\":" << Boolean(destination_mapping_stable)
           << ",\"coherent_memory_join\":" << Boolean(coherent_join) << ",\"before\":";
    EmitRootSlot(record, pending.before);
    record << ",\"after\":"; EmitRootSlot(record, after);
    record << ",\"previous_slot_after\":";
    if (pending.before.slot_address_valid) EmitRead(record, previous_slot_after); else record << "null";
    record << ",\"destination_before\":"; EmitRead(record, pending.destination_before);
    record << ",\"destination_after\":"; EmitRead(record, destination_after);
    record << ",\"instruction\":{\"charge_context_valid\":" << Boolean(instruction.valid)
           << ",\"timing_address\":" << instruction.address << ",\"span\":" << instruction.span
           << ",\"generation\":" << instruction.generation << ",\"callback_ordinal\":" << instruction.callback_ordinal
           << ",\"instruction_set\":";
    if (instruction.valid) record << "\"" << ((instruction.entry.cpsr & 0x20) ? "thumb" : "arm") << "\"";
    else record << "null";
    record << ",\"matching_callback\":" << Boolean(matching_callback)
           << ",\"backend_table_matches_charge_entry\":" << Boolean(instruction.backend_table_matches)
           << ",\"charge_table_matches_callback_entry\":" << Boolean(instruction_table_matches)
           << ",\"precise_pre_instruction_context\":" << Boolean(precise_context) << ",\"precise_pc_candidate\":";
    if (precise_context && pending.instruction_before.valid) record << pending.instruction_before.address; else record << "null";
    record << ",\"raw_prefix_at_callback_entry\":"; EmitRead(record, pending.instruction_before);
    record << ",\"raw_prefix_after_store\":"; EmitRead(record, instruction_after);
    record << ",\"charge_entry_context\":";
    if (instruction.valid) EmitContext(record, instruction.entry); else record << "null";
    record << "},\"callback_entry_context\":"; EmitContext(record, callback_entry);
    record << ",\"after_store_context\":"; EmitContext(record, after_store);
    record << ",\"architectural_instruction_completion\":false}\n";
    try { WriteRecord(record.str()); }
    catch (...) { ++omitted_events; throw; }
    ++events;
    if (committed_bytes) ++confirmed_bytes;
    if (same_value) ++same_value_stores;
    if (!coherent_join || !pending.instruction_before.valid || !instruction_after.valid) ++unavailable_events;
    if (!precise_context) ++imprecise_events;
}

void GuestWriteObservation::Fail(const char* reason) {
    if (!failure) failure = reason;
    throw std::runtime_error(std::string("guest write observation failed: ") + failure);
}
void GuestWriteObservation::WriteRecord(const std::string& record) {
    if (record.size() > byte_limit - FooterReserve || bytes_written > byte_limit - FooterReserve - record.size())
        Fail("byte_limit_exceeded");
    const auto written = std::fwrite(record.data(), 1, record.size(), output);
    bytes_written += written;
    record_interrupted = written != record.size();
    if (written != record.size() || std::fflush(output) != 0) Fail("output_io_failure");
}
bool GuestWriteObservation::Finish(bool capture_succeeded) {
    if (finished) return complete;
    complete = capture_succeeded && !failure && !failed_writes;
    std::ostringstream footer;
    if (record_interrupted) footer << '\n';
    footer << "{\"type\":\"footer\",\"complete\":" << Boolean(complete)
           << ",\"capture_succeeded\":" << Boolean(capture_succeeded) << ",\"reason\":\""
           << (failure ? failure : failed_writes ? "memory_write_failure" : capture_succeeded ? "complete" : "capture_failed")
           << "\",\"contexts\":" << contexts << ",\"callbacks\":" << callbacks << ",\"events\":" << events
           << ",\"omitted_events\":" << omitted_events << ",\"confirmed_destination_byte_events\":" << confirmed_bytes
           << ",\"same_value_store_events\":" << same_value_stores << ",\"unavailable_evidence_events\":" << unavailable_events
           << ",\"imprecise_instruction_events\":" << imprecise_events << ",\"failed_memory_writes\":" << failed_writes
           << ",\"unavailable_root_callbacks\":" << unavailable_root_callbacks
           << ",\"unavailable_slot_callbacks\":" << unavailable_slot_callbacks
           << ",\"bytes_before_footer\":" << bytes_written << "}\n";
    const auto record = footer.str();
    finished = true;
    if (bytes_written > byte_limit || record.size() > byte_limit - bytes_written) {
        complete = false; Fail("footer_byte_limit_exceeded");
    }
    const auto written = std::fwrite(record.data(), 1, record.size(), output);
    bytes_written += written;
    if (written != record.size() || std::fflush(output) != 0) {
        complete = false; Fail("footer_io_failure");
    }
    return complete;
}
}
