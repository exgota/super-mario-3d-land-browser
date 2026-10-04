// SPDX-License-Identifier: GPL-2.0-or-later
#include "GameplaySession.h"
#include "browser/BrowserWebGlDisplaySurface.h"
#include <array>
#include <atomic>
#include <charconv>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fcntl.h>
#include <limits>
#include <mutex>
#include <sstream>
#include <stdexcept>
#include <string_view>
#include <system_error>
#include <unistd.h>
#include "core/memory.h"
#include "video_core/pica/pica_core.h"
#include "video_core/renderer_software/renderer_software.h"

namespace {
using Clock = std::chrono::steady_clock;
constexpr std::uint32_t MaximumPresentations = 60000;
constexpr std::uint64_t MaximumRgbaBytes = 4096ull * 4096 * 4;
constexpr std::uint64_t MaximumFramebufferBytes = 16ull * 1024 * 1024;
constexpr std::uint64_t MaximumFinalPayloadBytes = 2 * (MaximumRgbaBytes + MaximumFramebufferBytes);
constexpr std::size_t MaximumConfigurationBytes = 4096, MaximumOutcomeBytes = 16384;
enum class SessionControl : std::uint32_t { Inactive, Active, StopRequested };
std::atomic<SessionControl> control{SessionControl::Inactive};
std::atomic<bool> selected{};
std::mutex session_mutex;

struct SessionState {
    std::filesystem::path directory;
    bool initialized = false, finished = false, failed = false, complete = false;
    bool top_screen_submitted = false, terminal_boundary_observed = false;
    bool stop_observed = false, limit_reached = false, final_frame_exported = false;
    std::uint32_t presentation_limit = 0, presentations = 0, payload_files_written = 0;
    std::uint64_t gsp_commands = 0, pica_lists = 0, pica_bytes = 0, buffer_swaps = 0;
    std::uint64_t vblanks = 0, hardware_register_writes = 0, color_fills = 0;
    std::uint64_t presentation_callbacks = 0, submission_ticks = 0, last_presentation_ticks = 0;
    std::uint64_t final_ticks = 0, final_renderer_frame = 0, payload_bytes_written = 0;
    std::uint64_t interval_samples = 0, interval_total_ns = 0, interval_min_ns = 0, interval_max_ns = 0;
    std::uint64_t first_presentation_elapsed_ns = 0, export_elapsed_ns = 0;
    Clock::time_point started, last_presentation;
    std::string outcome = "running", screen_metadata;
    const char* failure_reason = nullptr;
} session;

const char* Boolean(bool value) { return value ? "true" : "false"; }
std::string Quote(std::string_view text) {
    std::ostringstream output;
    output << '"';
    for (unsigned char byte : text) {
        if (byte == '"' || byte == '\\') output << '\\' << char(byte);
        else if (byte < 32 || byte >= 127) {
            constexpr char digits[] = "0123456789abcdef";
            output << "\\u00" << digits[byte >> 4] << digits[byte & 15];
        } else output << char(byte);
    }
    output << '"';
    return output.str();
}
void MarkFailure(const char* reason) {
    session.failed = true; session.complete = false; session.failure_reason = reason;
    control.store(SessionControl::Inactive, std::memory_order_release);
}
[[noreturn]] void Fail(const char* reason) {
    MarkFailure(reason);
    throw std::runtime_error(std::string("gameplay session failed: ") + reason);
}
void Add(std::uint64_t& count, std::uint64_t amount = 1) {
    if (amount > std::numeric_limits<std::uint64_t>::max() - count) Fail("scalar_counter_overflow");
    count += amount;
}
bool Recording() { return session.initialized && !session.finished && !session.failed && !session.complete; }
std::uint64_t Nanoseconds(Clock::time_point start, Clock::time_point end) {
    const auto count = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (count < 0) Fail("nonmonotonic_host_diagnostic_clock");
    return static_cast<std::uint64_t>(count);
}
void ValidateDirectory(const std::filesystem::path& directory) {
    for (auto current = directory; !current.empty(); current = current.parent_path()) {
        if (!std::filesystem::is_directory(current) || std::filesystem::is_symlink(current))
            throw std::runtime_error("gameplay session needs existing ordinary output directories");
        if (current == current.root_path()) break;
    }
}
void WriteExclusive(const std::string& name, const std::uint8_t* bytes, std::uint64_t size) {
    if (!bytes || size == 0 || size > MaximumRgbaBytes)
        throw std::runtime_error("invalid gameplay session output extent");
    const auto path = session.directory / name;
    const int descriptor = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_NOFOLLOW, 0600);
    if (descriptor < 0) throw std::runtime_error("cannot exclusively create gameplay session output");
    auto* stream = fdopen(descriptor, "wb");
    if (!stream) { close(descriptor); throw std::runtime_error("cannot open gameplay session output stream"); }
    const auto written = std::fwrite(bytes, 1, static_cast<std::size_t>(size), stream);
    const auto closed = std::fclose(stream);
    if (written != size || closed != 0) throw std::runtime_error("cannot finish gameplay session output");
}
void WriteJson(const std::string& name, const std::string& text, std::size_t maximum) {
    if (text.size() > maximum) throw std::runtime_error("gameplay session metadata exceeds its finite bound");
    WriteExclusive(name, reinterpret_cast<const std::uint8_t*>(text.data()), text.size());
}
void RefuseConcurrentObservers() {
    constexpr const char* names[] = {
        "ROOT_PORT_CAPTURE_PRESENTATION", "ROOT_PORT_CAPTURE_BEGIN_PRESENTATION", "ROOT_PORT_STATE_PLAN",
        "ROOT_PORT_MEMORY_TRACE_PATH", "ROOT_PORT_MEMORY_TRACE_VALUES",
        "ROOT_PORT_GUEST_WRITE_PATH", "ROOT_PORT_GUEST_WRITE_ROOT", "ROOT_PORT_GUEST_WRITE_SLOT_OFFSET",
        "ROOT_PORT_GUEST_WRITE_EVENT_LIMIT", "ROOT_PORT_GUEST_WRITE_BYTE_LIMIT",
        "ROOT_PORT_GUEST_EXECUTION_PATH", "ROOT_PORT_GUEST_EXECUTION_TRIGGER", "ROOT_PORT_GUEST_EXECUTION_PREHISTORY",
        "ROOT_PORT_GUEST_EXECUTION_POSTHISTORY", "ROOT_PORT_GUEST_EXECUTION_WINDOW_LIMIT",
        "ROOT_PORT_GUEST_EXECUTION_EVENT_LIMIT", "ROOT_PORT_GUEST_EXECUTION_BYTE_LIMIT",
        "ROOT_PORT_GUEST_EXECUTION_MINIMUM_TICKS"};
    for (const auto* name : names) if (std::getenv(name))
        throw std::runtime_error(std::string("gameplay session refuses concurrent observation selection: ") + name);
}
std::uint32_t ParseLimit(const char* text) {
    const std::string_view value(text);
    std::uint32_t count = 0;
    const auto parsed = std::from_chars(value.data(), value.data() + value.size(), count, 10);
    if (value.empty() || parsed.ec != std::errc() || parsed.ptr != value.data() + value.size() ||
        count == 0 || count > MaximumPresentations)
        throw std::runtime_error("gameplay session presentations must be a positive decimal in 1..60000");
    return count;
}
void ExportFinalScreens(const VideoCore::RendererBase& renderer, const Pica::PicaCore& pica,
                        Memory::MemorySystem& memory) {
    const auto* software = dynamic_cast<const SwRenderer::RendererSoftware*>(&renderer);
    if (!software) throw std::runtime_error("gameplay final export requires the software renderer");
    Port::FlushBrowserWebGlDisplaySurfaces();
    const_cast<SwRenderer::RendererSoftware*>(software)->PrepareRenderTarget();
    struct ScreenExport {
        const std::uint8_t* rgba = nullptr;
        const std::uint8_t* framebuffer = nullptr;
        std::uint64_t rgba_bytes = 0, framebuffer_bytes = 0;
        std::string rgba_name, framebuffer_name;
    };
    std::array<ScreenExport, 2> screens;
    std::ostringstream metadata;
    metadata << '[';
    for (std::size_t index = 0; index < screens.size(); ++index) {
        const std::uint32_t screen_id = index == 0 ? 0 : 2;
        const auto& info = software->Screen(static_cast<VideoCore::ScreenId>(screen_id));
        const auto& framebuffer = pica.regs.framebuffer_config[index];
        const auto& fill = index == 0 ? pica.regs_lcd.color_fill_top : pica.regs_lcd.color_fill_bottom;
        const std::uint32_t address = framebuffer.active_fb == 0 ? framebuffer.address_left1 : framebuffer.address_left2;
        const std::uint32_t right_address = framebuffer.active_fb == 0 ? framebuffer.address_right1 : framebuffer.address_right2;
        const auto raw_size = std::uint64_t(framebuffer.stride) * framebuffer.height;
        const auto rgba_size = std::uint64_t(info.width) * info.height * 4;
        if (info.width == 0 || info.height == 0 || info.width > 4096 || info.height > 4096 ||
            info.pixels.size() != rgba_size || raw_size == 0 || raw_size > MaximumFramebufferBytes ||
            rgba_size > MaximumRgbaBytes || std::uint64_t(address) + raw_size > std::numeric_limits<std::uint32_t>::max())
            throw std::runtime_error("invalid gameplay software presentation or framebuffer extent");
        const auto* raw = memory.GetPhysicalPointer(address);
        const auto* last = memory.GetPhysicalPointer(address + static_cast<std::uint32_t>(raw_size) - 1);
        if (!raw || !last) throw std::runtime_error("gameplay framebuffer is outside physical memory");
        const auto base = reinterpret_cast<std::uintptr_t>(raw);
        if (raw_size - 1 > std::numeric_limits<std::uintptr_t>::max() - base ||
            reinterpret_cast<std::uintptr_t>(last) != base + raw_size - 1)
            throw std::runtime_error("gameplay framebuffer is not contiguous physical memory");
        const auto end = std::uint64_t(address) + raw_size;
        for (auto page = (std::uint64_t(address) / Memory::CITRA_PAGE_SIZE + 1) * Memory::CITRA_PAGE_SIZE;
             page < end; page += Memory::CITRA_PAGE_SIZE) {
            const auto* pointer = memory.GetPhysicalPointer(static_cast<std::uint32_t>(page));
            if (!pointer || reinterpret_cast<std::uintptr_t>(pointer) != base + page - address)
                throw std::runtime_error("gameplay framebuffer crosses a noncontiguous physical page");
        }
        auto& output = screens[index];
        output = {info.pixels.data(), raw, rgba_size, raw_size,
                  "rendered_screen_" + std::to_string(screen_id) + ".rgba",
                  "framebuffer_screen_" + std::to_string(screen_id) + ".bin"};
        // Preserve the accepted software renderer's bytes and landscape dimension reversal.
        metadata << (index ? "," : "") << "{\"screen_id\":" << screen_id
                 << ",\"storage_width\":" << info.width << ",\"storage_height\":" << info.height
                 << ",\"width\":" << info.height << ",\"height\":" << info.width
                 << ",\"rgba_payload\":" << Quote(output.rgba_name) << ",\"rgba_bytes\":" << rgba_size
                 << ",\"framebuffer_payload\":" << Quote(output.framebuffer_name) << ",\"framebuffer_bytes\":" << raw_size
                 << ",\"framebuffer_width\":" << framebuffer.width << ",\"framebuffer_height\":" << framebuffer.height
                 << ",\"stride\":" << framebuffer.stride << ",\"format\":" << framebuffer.format
                 << ",\"color_format\":" << static_cast<std::uint32_t>(framebuffer.color_format.Value())
                 << ",\"active_fb\":" << framebuffer.active_fb << ",\"physical_address\":" << address
                 << ",\"right_physical_address\":" << right_address << ",\"color_fill\":" << fill.raw << '}';
    }
    metadata << ']';
    // Validate both pairs before writing. Failed writes remain as incomplete private output.
    for (const auto& screen : screens) {
        WriteExclusive(screen.rgba_name, screen.rgba, screen.rgba_bytes);
        ++session.payload_files_written; Add(session.payload_bytes_written, screen.rgba_bytes);
        WriteExclusive(screen.framebuffer_name, screen.framebuffer, screen.framebuffer_bytes);
        ++session.payload_files_written; Add(session.payload_bytes_written, screen.framebuffer_bytes);
    }
    session.screen_metadata = metadata.str(); session.final_frame_exported = true;
}
}

bool Port::InitializeGameplaySession(const std::filesystem::path& directory) {
    const auto* requested = std::getenv("ROOT_PORT_GAMEPLAY_SESSION_PRESENTATIONS");
    if (!requested) return false;
    const auto limit = ParseLimit(requested);
    RefuseConcurrentObservers();
    std::lock_guard<std::mutex> lock(session_mutex);
    if (session.initialized) throw std::runtime_error("gameplay session is already initialized in this module");
    session.directory = std::filesystem::absolute(directory).lexically_normal();
    ValidateDirectory(session.directory);
    for (const char* name : {"gameplay_session_configuration.json", "gameplay_session_outcome.json",
                            "rendered_screen_0.rgba", "framebuffer_screen_0.bin",
                            "rendered_screen_2.rgba", "framebuffer_screen_2.bin",
                            "gpu_events.jsonl", "gpu_window_events.jsonl"}) {
        std::error_code error;
        const auto status = std::filesystem::symlink_status(session.directory / name, error);
        if ((error && error != std::errc::no_such_file_or_directory) || status.type() != std::filesystem::file_type::not_found)
            throw std::runtime_error("gameplay session output or GPU trace already exists");
    }
    session.initialized = true; session.presentation_limit = limit; session.started = Clock::now();
    selected.store(true, std::memory_order_release);
    try {
        std::ostringstream configuration;
        configuration << "{\"type\":\"gameplay_session_configuration\",\"version\":1,\"mode\":\"normal_gameplay_session\""
                      << ",\"presentation_limit\":" << limit << ",\"maximum_presentations\":" << MaximumPresentations
                      << ",\"presentation_scope\":\"natural_presentations_after_first_top_screen_submission\""
                      << ",\"gpu_trace_absent\":true,\"pica_payload_files_absent\":true,\"final_frame_only\":true"
                      << ",\"maximum_rgba_bytes_per_screen\":" << MaximumRgbaBytes
                      << ",\"maximum_framebuffer_bytes_per_screen\":" << MaximumFramebufferBytes
                      << ",\"maximum_final_payload_bytes\":" << MaximumFinalPayloadBytes
                      << ",\"maximum_configuration_bytes\":" << MaximumConfigurationBytes
                      << ",\"maximum_outcome_bytes\":" << MaximumOutcomeBytes
                      << ",\"wall_deadline_owner\":\"frontend_explicit_at_most_3600_seconds\""
                      << ",\"input_audio_scope\":\"frontend_owned\",\"exact_replay_accepted\":false"
                      << ",\"section_7_accepted\":false,\"goal_accepted\":false"
                      << ",\"host_timing_scope\":\"aggregate_presentation_intervals_and_final_export_including_io\"}\n";
        WriteJson("gameplay_session_configuration.json", configuration.str(), MaximumConfigurationBytes);
    } catch (...) { MarkFailure("configuration_output_failure"); throw; }
    control.store(SessionControl::Active, std::memory_order_release);
    return true;
}
bool Port::GameplaySessionEnabled() { return selected.load(std::memory_order_acquire); }
std::uint32_t Port::GameplaySessionPresentationLimit() {
    return GameplaySessionEnabled() ? session.presentation_limit : 0;
}
void Port::RecordGameplayGspCommand() {
    std::lock_guard<std::mutex> lock(session_mutex); if (Recording()) Add(session.gsp_commands);
}
void Port::RecordGameplayPicaCommandList(std::uint32_t size) {
    std::lock_guard<std::mutex> lock(session_mutex);
    if (Recording()) { Add(session.pica_lists); Add(session.pica_bytes, size); }
}
void Port::RecordGameplayBufferSwap(std::uint32_t screen_id, std::uint64_t ticks) {
    std::lock_guard<std::mutex> lock(session_mutex);
    if (!Recording()) return;
    Add(session.buffer_swaps);
    if (screen_id == 0 && !session.top_screen_submitted) {
        session.top_screen_submitted = true; session.submission_ticks = ticks;
    }
}
void Port::RecordGameplayVBlank() {
    std::lock_guard<std::mutex> lock(session_mutex); if (Recording()) Add(session.vblanks);
}
void Port::RecordGameplayHardwareRegisterWrite() {
    std::lock_guard<std::mutex> lock(session_mutex); if (Recording()) Add(session.hardware_register_writes);
}
void Port::RecordGameplayColorFill() {
    std::lock_guard<std::mutex> lock(session_mutex); if (Recording()) Add(session.color_fills);
}
bool Port::RecordGameplayPresentation(const VideoCore::RendererBase& renderer, const Pica::PicaCore& pica,
                                      Memory::MemorySystem& memory, std::uint64_t ticks) {
    std::lock_guard<std::mutex> lock(session_mutex);
    if (!Recording()) return false;
    Add(session.presentation_callbacks);
    if (!session.top_screen_submitted) return false;
    const auto now = Clock::now();
    if (session.presentations) {
        const auto interval = Nanoseconds(session.last_presentation, now);
        if (!session.interval_samples || interval < session.interval_min_ns) session.interval_min_ns = interval;
        if (interval > session.interval_max_ns) session.interval_max_ns = interval;
        Add(session.interval_total_ns, interval); Add(session.interval_samples);
    } else session.first_presentation_elapsed_ns = Nanoseconds(session.started, now);
    session.last_presentation = now; session.last_presentation_ticks = ticks;
    ++session.presentations;
    session.stop_observed = control.load(std::memory_order_acquire) == SessionControl::StopRequested;
    session.limit_reached = session.presentations == session.presentation_limit;
    if (!session.stop_observed && !session.limit_reached) return false;
    session.terminal_boundary_observed = true;
    session.final_ticks = ticks; session.final_renderer_frame = renderer.GetCurrentFrame();
    session.outcome = session.stop_observed ? "requested_stop" : "presentation_limit";
    const auto export_started = Clock::now();
    try { ExportFinalScreens(renderer, pica, memory); }
    catch (...) { MarkFailure("final_frame_export_failure"); throw; }
    session.export_elapsed_ns = Nanoseconds(export_started, Clock::now());
    session.complete = true;
    control.store(SessionControl::Inactive, std::memory_order_release);
    return true;
}
bool Port::GameplaySessionComplete() {
    std::lock_guard<std::mutex> lock(session_mutex); return session.complete && !session.failed;
}
std::string Port::GameplaySessionOutcome() {
    std::lock_guard<std::mutex> lock(session_mutex);
    if (!session.initialized) return "disabled";
    return session.failed ? "session_failure" : session.outcome;
}
void Port::FinishGameplaySession(const std::string& outcome, std::uint64_t ticks) {
    std::lock_guard<std::mutex> lock(session_mutex);
    if (!session.initialized || session.finished) return;
    const bool pending_stop = control.exchange(SessionControl::Inactive, std::memory_order_acq_rel) == SessionControl::StopRequested;
    if (outcome.empty() || outcome.size() > 128) Fail("invalid_frontend_outcome_extent");
    session.complete = session.complete && !session.failed && session.final_frame_exported && outcome == session.outcome;
    const auto endpoint = session.outcome;
    session.outcome = outcome; session.finished = true;
    try {
        std::ostringstream result;
        result << "{\"type\":\"gameplay_session_outcome\",\"version\":1,\"outcome\":" << Quote(outcome)
               << ",\"endpoint\":" << Quote(endpoint) << ",\"complete\":" << Boolean(session.complete)
               << ",\"failure_reason\":";
        if (session.failure_reason) result << Quote(session.failure_reason); else result << "null";
        result << ",\"presentation_limit\":" << session.presentation_limit << ",\"presentations\":" << session.presentations
               << ",\"presentation_callbacks\":" << session.presentation_callbacks
               << ",\"top_screen_submitted\":" << Boolean(session.top_screen_submitted) << ",\"submission_ticks\":";
        if (session.top_screen_submitted) result << session.submission_ticks; else result << "null";
        result << ",\"last_eligible_presentation_ticks\":";
        if (session.presentations) result << session.last_presentation_ticks; else result << "null";
        result << ",\"finish_ticks\":" << ticks << ",\"stop_pending_at_finish\":" << Boolean(pending_stop)
               << ",\"stop_observed_at_final_presentation\":" << Boolean(session.stop_observed)
               << ",\"limit_reached_at_final_presentation\":" << Boolean(session.limit_reached)
               << ",\"final_frame_exported\":" << Boolean(session.final_frame_exported)
               << ",\"payload_files_written\":" << session.payload_files_written << ",\"payload_bytes_written\":" << session.payload_bytes_written
               << ",\"final_presentation\":";
        if (session.terminal_boundary_observed) result << "{\"presentation_index\":" << session.presentations - 1
            << ",\"renderer_frame\":" << session.final_renderer_frame << ",\"ticks\":" << session.final_ticks
            << ",\"vblank_index\":" << (session.vblanks ? session.vblanks - 1 : 0) << '}';
        else result << "null";
        result << ",\"screens\":" << (session.screen_metadata.empty() ? "null" : session.screen_metadata)
               << ",\"counts\":{\"gsp_commands\":" << session.gsp_commands << ",\"pica_lists\":" << session.pica_lists
               << ",\"pica_bytes_observed_without_export\":" << session.pica_bytes << ",\"buffer_swaps\":" << session.buffer_swaps
               << ",\"vblanks\":" << session.vblanks << ",\"hardware_register_writes\":" << session.hardware_register_writes
               << ",\"color_fills\":" << session.color_fills << "},\"host_diagnostics\":{\"elapsed_ns\":" << Nanoseconds(session.started, Clock::now())
               << ",\"first_eligible_presentation_elapsed_ns\":";
        if (session.presentations) result << session.first_presentation_elapsed_ns; else result << "null";
        result << ",\"presentation_interval_samples\":" << session.interval_samples << ",\"presentation_interval_total_ns\":" << session.interval_total_ns
               << ",\"presentation_interval_min_ns\":";
        if (session.interval_samples) result << session.interval_min_ns; else result << "null";
        result << ",\"presentation_interval_max_ns\":";
        if (session.interval_samples) result << session.interval_max_ns; else result << "null";
        result << ",\"final_export_elapsed_ns\":";
        if (session.final_frame_exported) result << session.export_elapsed_ns; else result << "null";
        result << "},\"gpu_trace_absent\":true,\"pica_payload_files_absent\":true,\"exact_replay_accepted\":false"
               << ",\"section_7_accepted\":false,\"goal_accepted\":false}\n";
        WriteJson("gameplay_session_outcome.json", result.str(), MaximumOutcomeBytes);
    } catch (...) { MarkFailure("outcome_output_failure"); throw; }
}

extern "C" std::uint32_t BrowserGameplaySessionRequestStop() {
    auto observed = control.load(std::memory_order_acquire);
    while (observed != SessionControl::Inactive) {
        if (observed == SessionControl::StopRequested) return 0;
        if (control.compare_exchange_weak(observed, SessionControl::StopRequested,
                                         std::memory_order_acq_rel, std::memory_order_acquire)) return 0;
    }
    return 2;
}
extern "C" std::uint32_t BrowserGameplaySessionIsActive() {
    return control.load(std::memory_order_acquire) == SessionControl::Inactive ? 0 : 1;
}
