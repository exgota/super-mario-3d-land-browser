#include "GuestExecutionObservation.h"
#include <algorithm>
#include <charconv>
#include <cstdlib>
#include <filesystem>
#include <fcntl.h>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <unistd.h>
#include "core/memory.h"

namespace Port {
namespace {
constexpr std::uint64_t FooterReserve = 4096, AddressExtent = std::uint64_t(1) << 32;
constexpr std::uint64_t MaximumCallbacks = 32, MaximumEntryRecords = 128, MaximumRetainedRecords = 16384;
const char* Boolean(bool value) { return value ? "true" : "false"; }
std::string Quote(std::string_view text) {
    std::ostringstream result;
    result << '"';
    for (unsigned char byte : text) {
        if (byte == '"' || byte == '\\') result << '\\' << char(byte);
        else if (byte < 32 || byte >= 127) {
            constexpr char digits[] = "0123456789abcdef";
            result << "\\u00" << digits[byte >> 4] << digits[byte & 15];
        } else result << char(byte);
    }
    result << '"';
    return result.str();
}
std::uint64_t Parse(const char* text, std::uint64_t maximum, const char* name) {
    std::string_view input(text);
    int base = 10;
    if (input.starts_with("0x") || input.starts_with("0X")) { base = 16; input.remove_prefix(2); }
    std::uint64_t value = 0;
    const auto converted = std::from_chars(input.data(), input.data() + input.size(), value, base);
    if (input.empty() || converted.ec != std::errc() || converted.ptr != input.data() + input.size() || value > maximum)
        throw std::runtime_error(std::string("invalid guest execution observation ") + name);
    return value;
}
template<class T> void Array(std::ostream& stream, const T& values, std::size_t count) {
    stream << '[';
    for (std::size_t index = 0; index < count; ++index) stream << (index ? "," : "") << +values[index];
    stream << ']';
}
void EmitSnapshot(std::ostream& stream, const GuestExecutionObservation::Snapshot& state) {
    const auto& machine = state.machine;
    stream << "{\"registers\":"; Array(stream, machine.registers, machine.registers.size());
    stream << ",\"floating_registers\":"; Array(stream, machine.floating_registers, machine.floating_registers.size());
    stream << ",\"coprocessor_registers\":"; Array(stream, state.coprocessor_registers, state.coprocessor_count);
    stream << ",\"condition_fields_n_z_c_v_q_thumb_ge\":"; Array(stream, state.condition_fields, state.condition_fields.size());
    stream << ",\"cpsr\":" << machine.cpsr << ",\"cpsr_control\":" << state.cpsr_control
           << ",\"fpscr\":" << machine.fpscr << ",\"fpexc\":" << machine.fpexc << ",\"tls\":" << machine.tls
           << ",\"exclusive\":" << machine.exclusive << ",\"exclusive_address\":" << machine.exclusive_address
           << ",\"timer_ticks\":" << machine.timer_ticks << ",\"timer_downcount\":" << machine.timer_downcount
           << ",\"scheduled_instructions\":" << machine.scheduled_instructions << ",\"budget\":" << state.budget
           << ",\"exit\":" << state.exit << ",\"supervisor_call\":" << state.supervisor_call
           << ",\"dispatch_depth\":" << state.dispatch_depth << ",\"reschedule\":" << Boolean(state.reschedule)
           << ",\"break_requested\":" << Boolean(state.break_requested)
           << ",\"callback_matches_backend\":" << Boolean(state.callback_matches_backend)
           << ",\"actual_table_identity\":" << state.actual_table_identity
           << ",\"backend_table_identity\":" << state.backend_table_identity
           << ",\"tables_match\":" << Boolean(state.actual_table_identity && state.actual_table_identity == state.backend_table_identity)
           << ",\"writer_observer_enabled\":" << Boolean(state.writer_enabled)
           << ",\"writer_context_identity\":" << state.writer_context
           << ",\"writer_charge_generation\":" << state.writer_generation
           << ",\"writer_callback_ordinal\":" << state.writer_callback_ordinal
           << ",\"writer_charge_valid\":" << Boolean(state.writer_charge_valid) << '}';
}
void EmitBytes(std::ostream& stream, const GuestExecutionObservation::RamBytes& bytes) {
    stream << "{\"address\":" << bytes.address << ",\"width\":" << bytes.width
           << ",\"valid\":" << Boolean(bytes.valid) << ",\"reason\":" << Quote(bytes.reason) << ",\"bytes\":";
    if (bytes.valid) Array(stream, bytes.bytes, bytes.width); else stream << "null";
    stream << ",\"value\":";
    if (bytes.valid) stream << bytes.value; else stream << "null";
    stream << '}';
}
bool SameMapping(const GuestExecutionObservation::RamBytes& first, const GuestExecutionObservation::RamBytes& second) {
    return first.valid && second.valid && first.address == second.address && first.width == second.width && first.locations == second.locations;
}
}

std::shared_ptr<GuestExecutionObservation> GuestExecutionObservation::FromEnvironment() {
    const char* path = std::getenv("ROOT_PORT_GUEST_EXECUTION_PATH");
    const char* trigger = std::getenv("ROOT_PORT_GUEST_EXECUTION_TRIGGER");
    const char* pre = std::getenv("ROOT_PORT_GUEST_EXECUTION_PREHISTORY");
    const char* post = std::getenv("ROOT_PORT_GUEST_EXECUTION_POSTHISTORY");
    const char* windows = std::getenv("ROOT_PORT_GUEST_EXECUTION_WINDOW_LIMIT");
    const char* events = std::getenv("ROOT_PORT_GUEST_EXECUTION_EVENT_LIMIT");
    const char* bytes = std::getenv("ROOT_PORT_GUEST_EXECUTION_BYTE_LIMIT");
    if (!path && !trigger && !pre && !post && !windows && !events && !bytes) return nullptr;
    if (!path || !*path || !trigger) throw std::runtime_error("guest execution observation needs path and tagged trigger PC");
    const auto selected = Parse(trigger, UINT32_MAX, "trigger PC");
    if (!(selected & 1) && (selected & 3)) throw std::runtime_error("guest execution trigger ARM PC is misaligned");
    const auto prehistory = pre ? Parse(pre, 256, "prehistory") : 16;
    const auto posthistory = post ? Parse(post, 512, "posthistory") : 32;
    const auto window_limit = windows ? Parse(windows, 64, "window limit") : 1;
    const auto event_limit = events ? Parse(events, 100000, "event limit") : 20000;
    const auto byte_limit = bytes ? Parse(bytes, 64 * 1024 * 1024, "byte limit") : 16 * 1024 * 1024;
    if (!window_limit || !event_limit || byte_limit < 8192)
        throw std::runtime_error("guest execution observation needs positive window/event limits and at least 8192 bytes");
    return std::shared_ptr<GuestExecutionObservation>(new GuestExecutionObservation(
        path, static_cast<std::uint32_t>(selected), prehistory, posthistory, window_limit, event_limit, byte_limit));
}

GuestExecutionObservation::GuestExecutionObservation(const char* path, std::uint32_t trigger_,
    std::uint64_t prehistory, std::uint64_t posthistory, std::uint64_t windows, std::uint64_t events, std::uint64_t bytes)
    : trigger(trigger_), prehistory_limit(prehistory), posthistory_limit(posthistory), window_limit(windows), event_limit(events), byte_limit(bytes) {
    const auto filename = std::filesystem::absolute(path).lexically_normal();
    for (auto parent = filename.parent_path(); !parent.empty(); parent = parent.parent_path()) {
        if (!std::filesystem::is_directory(parent) || std::filesystem::is_symlink(parent))
            throw std::runtime_error("guest execution output needs existing ordinary parent directories");
        if (parent == parent.root_path()) break;
    }
    const int descriptor = open(filename.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW, 0600);
    if (descriptor < 0) throw std::runtime_error("cannot exclusively create guest execution output");
    output = fdopen(descriptor, "w");
    if (!output) { close(descriptor); throw std::runtime_error("cannot open guest execution output stream"); }
    try {
        std::ostringstream record;
        record << "{\"type\":\"configuration\",\"version\":1,\"trigger_tagged_pc\":" << trigger
               << ",\"prehistory_charge_entries\":" << prehistory_limit << ",\"posthistory_charge_entries\":" << posthistory_limit
               << ",\"window_limit\":" << window_limit << ",\"event_limit\":" << event_limit << ",\"byte_limit\":" << byte_limit
               << ",\"footer_reserve\":" << FooterReserve << ",\"callbacks_per_charge_limit\":" << MaximumCallbacks
               << ",\"records_per_charge_limit\":" << MaximumEntryRecords << ",\"retained_record_limit\":" << MaximumRetainedRecords
               << ",\"context_limit\":8,\"table_identity_limit\":4096,\"byte_order\":\"little_endian\",\"writer_output_path\":";
        const char* writer = std::getenv("ROOT_PORT_GUEST_WRITE_PATH");
        if (writer) record << Quote(writer); else record << "null";
        record << ",\"coverage\":[\"charge_entry_not_retirement\",\"callbacks_not_full_instruction_completion\","
                  "\"no_hle_or_direct_memory_visibility\",\"sampled_mappings_only_no_global_remapping_hook\","
                  "\"no_saved_register_ancestry\",\"no_exclusive_store_success_inference\"]}\n";
        WriteRecord(record.str());
    } catch (...) {
        try { Finish(false); } catch (...) {}
        std::fclose(output); output = nullptr; throw;
    }
}
GuestExecutionObservation::~GuestExecutionObservation() {
    if (!finished) { try { Finish(false); } catch (...) {} }
    if (output) std::fclose(output);
}
GuestExecutionObservation::Charge::~Charge() { if (accounting) accounting->records -= records.size(); }
bool GuestExecutionObservation::Collecting() const noexcept {
    std::lock_guard<std::mutex> lock(mutex);
    return collecting && !finished && !failure;
}
void GuestExecutionObservation::MarkIncomplete(const char* reason) noexcept {
    std::lock_guard<std::mutex> lock(mutex);
    if (!failure) failure = reason;
    ++omitted_records;
}
[[noreturn]] void GuestExecutionObservation::Fail(const char* reason) {
    if (!failure) failure = reason;
    throw std::runtime_error(std::string("guest execution observation failed: ") + failure);
}
std::uint64_t GuestExecutionObservation::RegisterContext(std::uint32_t processor, const void* backend,
    const void* translated_context, std::uint64_t writer_context) {
    std::lock_guard<std::mutex> lock(mutex);
    if (contexts.size() == 8) Fail("context_limit_exceeded");
    BackendContext state; state.processor = processor; contexts.push_back(state);
    std::ostringstream record;
    record << "{\"type\":\"context_registration\",\"context_identity\":" << contexts.size() << ",\"processor\":" << processor
           << ",\"backend_identity\":\"" << backend << "\",\"translated_context_identity\":\"" << translated_context
           << "\",\"writer_context_identity\":" << writer_context << ",\"identity_scope\":\"this_shared_observer_invocation\"}\n";
    WriteRecord(record.str());
    return contexts.size();
}
GuestExecutionObservation::BackendContext& GuestExecutionObservation::Context(std::uint64_t identity) {
    if (!identity || identity > contexts.size()) Fail("unknown_backend_context");
    return contexts[identity - 1];
}
std::uint64_t GuestExecutionObservation::TableIdentity(const std::shared_ptr<Memory::PageTable>& table) {
    if (!table) return 0;
    for (std::size_t index = 0; index < tables.size(); ++index)
        if (tables[index].object == table.get() && !tables[index].owner.owner_before(table) &&
            !table.owner_before(tables[index].owner)) return index + 1;
    if (tables.size() == 4096) Fail("table_identity_limit_exceeded");
    tables.push_back({table, table.get()});
    return tables.size();
}
GuestExecutionObservation::Snapshot GuestExecutionObservation::SealSnapshot(const Snapshot& state) {
    auto sealed = state;
    sealed.actual_table_identity = TableIdentity(state.actual_table);
    sealed.backend_table_identity = TableIdentity(state.backend_table);
    sealed.actual_table.reset(); sealed.backend_table.reset();
    return sealed;
}
GuestExecutionObservation::RamBytes GuestExecutionObservation::ReadRam(const std::shared_ptr<Memory::PageTable>& table,
    std::uint32_t address, std::uint32_t width) noexcept {
    RamBytes result; result.address = address; result.width = width;
    if (!width || width > 4 || std::uint64_t(address) + width > AddressExtent) { result.reason = "address_extent"; return result; }
    if (!table) { result.reason = "no_current_page_table"; return result; }
    auto& pointers = table->GetPointerArray();
    for (std::uint32_t index = 0; index < width; ++index) {
        const auto byte_address = address + index;
        const auto page = byte_address >> Memory::CITRA_PAGE_BITS;
        if (table->attributes[page] != Memory::PageType::Memory) { result.reason = "not_direct_memory"; return result; }
        if (!pointers[page]) { result.reason = "null_page_pointer"; return result; }
        result.locations[index] = pointers[page] + (byte_address & Memory::CITRA_PAGE_MASK);
    }
    for (std::uint32_t index = 0; index < width; ++index) {
        result.bytes[index] = *result.locations[index];
        result.value |= std::uint32_t(result.bytes[index]) << (8 * index);
    }
    result.valid = true; result.reason = "valid"; return result;
}
GuestExecutionObservation::RamBytes GuestExecutionObservation::ReadInstruction(
    const std::shared_ptr<Memory::PageTable>& table, std::uint32_t address, const Snapshot& state) noexcept {
    const bool thumb = (state.machine.cpsr & 0x20) != 0;
    if (!thumb && (address & 3)) { RamBytes result; result.address = address; result.width = 4; result.reason = "misaligned_arm_address"; return result; }
    return ReadRam(table, thumb ? address & ~std::uint32_t(1) : address, 4);
}
void GuestExecutionObservation::Add(const std::shared_ptr<Charge>& charge, Record record) {
    if (charge->records.size() >= MaximumEntryRecords) Fail("entry_record_limit_exceeded");
    if (accounting->records >= MaximumRetainedRecords) Fail("retained_record_limit_exceeded");
    charge->records.push_back(std::move(record)); ++accounting->records;
}
void GuestExecutionObservation::DetectTableChange(std::uint64_t identity, const Snapshot& state) {
    auto& context = Context(identity);
    if (context.table_seen && context.actual_table_identity != state.actual_table_identity) {
        ++context.segment; context.charge_valid = false;
        if (context.current) {
            Record record; record.kind = "boundary"; record.reason = "actual_page_table_identity_changed";
            record.order = ++observation_order; record.context = identity; record.charge = context.current->identity;
            record.segment = context.segment; record.before = state;
            Add(context.current, std::move(record));
        }
    }
    context.actual_table_identity = state.actual_table_identity;
    context.table_seen = true;
}
void GuestExecutionObservation::BeginCharge(std::uint64_t identity, std::uint32_t address, std::uint32_t span,
    std::uint64_t ticks, const Snapshot& entry) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!collecting || finished || failure) return;
    auto& context = Context(identity);
    const auto sealed = SealSnapshot(entry);
    DetectTableChange(identity, sealed);
    if (context.current) {
        Record boundary; boundary.order = ++observation_order; boundary.context = identity;
        boundary.charge = context.current->identity; boundary.segment = context.segment;
        boundary.reason = "next_charge_entry_observed"; boundary.before = sealed;
        boundary.instruction_before = ReadInstruction(entry.actual_table, context.current->address, context.current->records.front().before);
        const auto& previous = context.current->records.front().instruction_before;
        if (!SameMapping(previous, boundary.instruction_before)) {
            ++context.segment; boundary.segment = context.segment; boundary.reason = "sampled_instruction_mapping_unavailable_or_changed";
        }
        Add(context.current, std::move(boundary));
    }
    if (window_open && post_remaining == 0) {
        closing_boundary_available = true; closing_boundary = sealed;
        closing_context = identity; closing_order = ++observation_order; closing_address = address; closing_span = span;
        closing_instruction = ReadInstruction(entry.actual_table, address, entry);
        CloseWindow("selected_posthistory_boundary", true);
    }
    if (!collecting) return;
    auto charge = std::make_shared<Charge>(); charge->accounting = accounting;
    charge->identity = ++charge_order; charge->context = identity; charge->generation = ++context.generation;
    charge->segment = context.segment; charge->address = address; charge->span = span; charge->supplied_ticks = ticks;
    context.current = charge; context.charge_valid = false;
    Record record; record.kind = "charge_entry"; record.order = ++observation_order;
    record.context = identity; record.charge = charge->identity; record.segment = context.segment; record.before = sealed;
    record.instruction_before = ReadInstruction(entry.actual_table, address, entry);
    Add(charge, std::move(record));
    const bool thumb = (entry.machine.cpsr & 0x20) != 0;
    const bool matches = thumb == bool(trigger & 1) && (address | std::uint32_t(thumb)) == trigger;
    if (matches) ++trigger_observations;
    if (window_open) {
        window.push_back(charge); --post_remaining;
        if (matches) ++overlapping_triggers;
    } else if (matches) {
        window.assign(history.begin(), history.end()); window.push_back(charge);
        window_open = true; ++windows_started; post_remaining = posthistory_limit;
        trigger_charge = charge->identity; trigger_order = charge->records.front().order; evictions_at_trigger = history_evictions;
        closing_boundary_available = false;
    }
    history.push_back(charge);
    while (history.size() > prehistory_limit) { history.pop_front(); ++history_evictions; }
}
void GuestExecutionObservation::Admission(std::uint64_t identity, bool admitted, bool raised,
    const Snapshot& after, const char* exception) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!collecting || finished || failure) return;
    auto& context = Context(identity);
    if (!context.current || context.current->admission_recorded) Fail("admission_without_matching_charge");
    Record record; record.kind = "admission"; record.order = ++observation_order; record.context = identity;
    record.charge = context.current->identity; record.segment = context.segment; record.admitted = admitted; record.raised = raised;
    record.after = SealSnapshot(after);
    if (exception) { const auto text = std::string_view(exception); record.exception = text.substr(0, 1024); record.exception_text_truncated = text.size() > 1024; }
    Add(context.current, std::move(record));
    context.current->admission_recorded = true; context.current->admitted = admitted;
    context.charge_valid = admitted && !raised;
}
GuestExecutionObservation::MemoryToken GuestExecutionObservation::BeginMemory(std::uint64_t identity, bool write,
    std::uint32_t address, std::uint32_t width, std::uint32_t value, const Snapshot& before) {
    std::lock_guard<std::mutex> lock(mutex);
    MemoryToken token;
    if (!collecting || finished || failure) return token;
    auto& context = Context(identity);
    if (!context.current) { ++outside_charge_callbacks; return token; }
    if (context.current->callbacks == MaximumCallbacks) Fail("callback_limit_exceeded");
    token.collected = true; token.write = write; token.context = identity; token.charge = context.current->identity;
    token.ordinal = ++context.current->callbacks; token.begin_order = ++observation_order;
    token.before = SealSnapshot(before); DetectTableChange(identity, token.before);
    token.segment = context.segment; token.address = address; token.width = width; token.supplied_value = value;
    token.associated = context.charge_valid && before.callback_matches_backend;
    token.instruction_before = ReadInstruction(before.actual_table, context.current->address, context.current->records.front().before);
    token.destination_before = ReadRam(before.actual_table, address, width);
    return token;
}
void GuestExecutionObservation::CompleteMemory(const MemoryToken& token, bool returned, std::uint32_t value,
    const Snapshot& after, const char* exception) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!token.collected) return;
    if (finished || failure) Fail("memory_record_after_failure_or_footer");
    auto& context = Context(token.context);
    if (!context.current || context.current->identity != token.charge) Fail("memory_callback_charge_changed");
    Record record; record.kind = "memory_callback"; record.order = token.begin_order; record.end_order = ++observation_order;
    record.context = token.context; record.charge = token.charge; record.segment = token.segment; record.ordinal = token.ordinal;
    record.address = token.address; record.width = token.width; record.supplied_value = token.supplied_value;
    record.returned_value = value; record.write = token.write; record.returned = returned; record.raised = !returned;
    record.before = token.before; record.after = SealSnapshot(after);
    record.instruction_before = token.instruction_before;
    record.instruction_after = ReadInstruction(after.actual_table, context.current->address, context.current->records.front().before);
    record.destination_before = token.destination_before; record.destination_after = ReadRam(after.actual_table, token.address, token.width);
    record.associated = token.associated && context.charge_valid && after.callback_matches_backend &&
                        token.segment == context.segment && token.before.actual_table_identity == record.after.actual_table_identity;
    if (!SameMapping(record.instruction_before, record.instruction_after) ||
        !SameMapping(record.destination_before, record.destination_after)) {
        record.reason = "sampled_callback_mapping_unavailable_or_changed"; record.associated = false;
        ++context.segment; context.charge_valid = false;
    }
    if (exception) { const auto text = std::string_view(exception); record.exception = text.substr(0, 1024); record.exception_text_truncated = text.size() > 1024; }
    if (!returned) ++failed_memory_operations;
    Add(context.current, std::move(record)); DetectTableChange(token.context, SealSnapshot(after));
}
void GuestExecutionObservation::Boundary(std::uint64_t identity, const char* reason, const Snapshot& state, bool invalidate) {
    std::lock_guard<std::mutex> lock(mutex);
    if (!collecting || finished || failure) return;
    auto& context = Context(identity);
    const auto sealed = SealSnapshot(state); DetectTableChange(identity, sealed);
    if (invalidate) { ++context.segment; context.charge_valid = false; }
    if (!context.current) { ++outside_charge_boundaries; return; }
    Record record; record.kind = "boundary"; record.reason = reason; record.order = ++observation_order;
    record.context = identity; record.charge = context.current->identity; record.segment = context.segment; record.before = sealed;
    record.instruction_before = ReadInstruction(state.actual_table, context.current->address, context.current->records.front().before);
    Add(context.current, std::move(record));
}
std::string GuestExecutionObservation::Serialize(const Record& record, const Charge& charge, const char* scope) const {
    std::ostringstream stream;
    stream << "{\"type\":" << Quote(record.kind) << ",\"observation_order\":" << record.order
           << ",\"completion_observation_order\":" << record.end_order << ",\"window\":" << windows_started
           << ",\"scope\":" << Quote(scope) << ",\"context_identity\":" << record.context
           << ",\"processor\":" << contexts[record.context - 1].processor << ",\"charge_identity\":" << charge.identity
           << ",\"charge_generation\":" << charge.generation << ",\"segment\":" << record.segment
           << ",\"raw_charged_address\":" << charge.address << ",\"span\":" << charge.span
           << ",\"supplied_ticks\":" << charge.supplied_ticks << ",\"admission_recorded_by_window_emission\":" << Boolean(charge.admission_recorded)
           << ",\"charge_admitted_by_window_emission\":" << Boolean(charge.admitted) << ",\"reason\":" << Quote(record.reason)
           << ",\"callback_ordinal\":" << record.ordinal << ",\"matching_callback_charge\":" << Boolean(record.associated)
           << ",\"single_instruction_charge_admitted\":" << Boolean(charge.span == 1 && charge.admitted)
           << ",\"instruction_set_at_charge\":" << Quote((charge.records.front().before.machine.cpsr & 0x20) ? "thumb" : "arm")
           << ",\"precise_pre_instruction_context_candidate\":" << Boolean(charge.span == 1 && charge.admitted && record.associated &&
                charge.segment == record.segment && record.instruction_before.valid && record.before.actual_table_identity &&
                record.before.actual_table_identity == charge.records.front().before.actual_table_identity &&
                record.before.actual_table_identity == record.before.backend_table_identity &&
                record.after.actual_table_identity == record.before.actual_table_identity &&
                record.after.actual_table_identity == record.after.backend_table_identity &&
                SameMapping(record.instruction_before, record.instruction_after) &&
                charge.records.front().before.actual_table_identity == charge.records.front().before.backend_table_identity)
           << ",\"architectural_instruction_completion\":false,\"before\":";
    if (std::string_view(record.kind) != "admission") EmitSnapshot(stream, record.before); else stream << "null";
    stream << ",\"after\":";
    if (std::string_view(record.kind) == "admission" || std::string_view(record.kind) == "memory_callback") EmitSnapshot(stream, record.after); else stream << "null";
    stream << ",\"instruction_ram_prefix_before\":"; EmitBytes(stream, record.instruction_before);
    stream << ",\"instruction_ram_prefix_after\":"; EmitBytes(stream, record.instruction_after);
    stream << ",\"destination_ram_before\":"; EmitBytes(stream, record.destination_before);
    stream << ",\"destination_ram_after\":"; EmitBytes(stream, record.destination_after);
    stream << ",\"operation\":";
    if (std::string_view(record.kind) == "memory_callback") stream << Quote(record.write ? "write" : "read"); else stream << "null";
    stream << ",\"address\":" << record.address
           << ",\"width\":" << record.width << ",\"supplied_value\":" << record.supplied_value
           << ",\"actual_returned_value\":";
    if (record.returned && !record.write) stream << record.returned_value; else stream << "null";
    stream << ",\"operation_returned\":" << Boolean(record.returned) << ",\"raised\":" << Boolean(record.raised)
           << ",\"admitted\":" << Boolean(record.admitted) << ",\"exception\":";
    if (record.exception.empty()) stream << "null"; else stream << Quote(record.exception);
    stream << ",\"exception_text_truncated\":" << Boolean(record.exception_text_truncated)
           << ",\"instruction_mapping_stable_at_samples\":" << Boolean(SameMapping(record.instruction_before, record.instruction_after))
           << ",\"destination_mapping_stable_at_samples\":" << Boolean(SameMapping(record.destination_before, record.destination_after)) << "}\n";
    return stream.str();
}
void GuestExecutionObservation::CloseWindow(const char* reason, bool extent_complete) {
    for (const auto& charge : window) if (!charge->admission_recorded) Fail("selected_charge_missing_admission_record");
    std::ostringstream begin;
    begin << "{\"type\":\"window_begin\",\"window\":" << windows_started << ",\"trigger_charge\":" << trigger_charge
          << ",\"trigger_observation_order\":" << trigger_order << ",\"history_evictions_before_trigger\":" << evictions_at_trigger
          << ",\"prehistory_entries\":" << std::count_if(window.begin(), window.end(), [&](const auto& c) { return c->identity < trigger_charge; }) << "}\n";
    WriteRecord(begin.str());
    struct Selected { const Record* record; const Charge* charge; };
    std::vector<Selected> selected;
    for (const auto& charge : window) for (const auto& record : charge->records) selected.push_back({&record, charge.get()});
    std::sort(selected.begin(), selected.end(), [](const auto& a, const auto& b) { return a.record->order < b.record->order; });
    for (const auto& item : selected) WriteRecord(Serialize(*item.record, *item.charge,
        item.charge->identity < trigger_charge ? "prehistory" : item.charge->identity == trigger_charge ? "trigger" : "posthistory"));
    std::ostringstream end;
    end << "{\"type\":\"window_end\",\"window\":" << windows_started << ",\"extent_complete\":" << Boolean(extent_complete)
        << ",\"reason\":" << Quote(reason) << ",\"posthistory_entries_remaining\":" << post_remaining
        << ",\"records\":" << selected.size() << ",\"closing_boundary_available\":" << Boolean(closing_boundary_available)
        << ",\"closing_boundary_context_identity\":" << closing_context << ",\"closing_boundary_observation_order\":" << closing_order
        << ",\"next_raw_charged_address\":" << closing_address << ",\"next_charge_span\":" << closing_span
        << ",\"next_charge_admission_observed\":false,\"closing_boundary_snapshot\":";
    if (closing_boundary_available) EmitSnapshot(end, closing_boundary); else end << "null";
    end << ",\"next_instruction_ram_prefix\":";
    if (closing_boundary_available) EmitBytes(end, closing_instruction); else end << "null";
    end << ",\"architectural_instruction_completion\":false}\n";
    WriteRecord(end.str());
    if (extent_complete) { ++windows_completed; last_completed_window_charge = window.back()->identity; }
    window.clear(); window_open = false;
    if (windows_completed == window_limit) {
        collecting = false; history.clear(); for (auto& context : contexts) { context.current.reset(); context.charge_valid = false; }
    }
}
void GuestExecutionObservation::WriteRecord(const std::string& record) {
    if (output_records >= event_limit) Fail("event_limit_exceeded");
    const auto framed = std::string("{\"output_sequence\":") + std::to_string(output_records) + ',' + record.substr(1);
    if (framed.size() > byte_limit - FooterReserve || bytes_written > byte_limit - FooterReserve - framed.size()) Fail("byte_limit_exceeded");
    const auto written = std::fwrite(framed.data(), 1, framed.size(), output); bytes_written += written;
    interrupted = written != framed.size();
    if (interrupted || std::fflush(output) != 0) Fail("output_io_failure");
    ++output_records;
}
bool GuestExecutionObservation::Finish(bool capture_succeeded) {
    std::lock_guard<std::mutex> lock(mutex);
    if (finished) return complete;
    const bool truncated = window_open;
    if (window_open && !failure) {
        try { CloseWindow("capture_ended_before_next_selected_boundary", false); }
        catch (...) { if (!failure) failure = "window_output_failure"; ++omitted_records; }
    }
    complete = capture_succeeded && !failure && !truncated && !failed_memory_operations;
    std::ostringstream footer;
    if (interrupted) footer << '\n';
    footer << "{\"type\":\"footer\",\"output_sequence\":" << output_records << ",\"complete\":" << Boolean(complete) << ",\"capture_succeeded\":" << Boolean(capture_succeeded)
           << ",\"reason\":" << Quote(failure ? failure : truncated ? "posthistory_truncated_by_capture" : failed_memory_operations ? "memory_operation_failure" : !capture_succeeded ? "capture_failed" : "complete_declared_scope")
           << ",\"contexts\":" << contexts.size() << ",\"charges_observed\":" << charge_order << ",\"windows_started\":" << windows_started
           << ",\"windows_completed\":" << windows_completed << ",\"zero_observed_windows\":" << Boolean(windows_started == 0)
           << ",\"disarmed_after_window_limit\":" << Boolean(!collecting) << ",\"later_triggers_not_observed\":" << Boolean(!collecting)
           << ",\"posthistory_truncated\":" << Boolean(truncated) << ",\"history_evictions\":" << history_evictions
           << ",\"unselected_retained_charge_entries_at_finish\":" << history.size()
           << ",\"last_completed_window_charge\":" << last_completed_window_charge
           << ",\"charged_entries_after_last_completed_window\":" << (last_completed_window_charge ? charge_order - last_completed_window_charge : charge_order)
           << ",\"last_observation_order\":" << observation_order
           << ",\"trigger_observations\":" << trigger_observations << ",\"overlapping_triggers\":" << overlapping_triggers
           << ",\"output_records\":" << output_records << ",\"omitted_records\":" << omitted_records
           << ",\"outside_charge_callbacks\":" << outside_charge_callbacks << ",\"outside_charge_boundaries\":" << outside_charge_boundaries
           << ",\"failed_memory_operations\":" << failed_memory_operations << ",\"bytes_before_footer\":" << bytes_written
           << ",\"coverage_complete_for_all_guest_execution\":false,\"architectural_instruction_completion\":false}\n";
    const auto record = footer.str(); finished = true;
    if (record.size() > byte_limit - bytes_written) { complete = false; Fail("footer_byte_limit_exceeded"); }
    const auto written = std::fwrite(record.data(), 1, record.size(), output); bytes_written += written;
    if (written != record.size() || std::fflush(output) != 0) { complete = false; Fail("footer_io_failure"); }
    return complete;
}
}
