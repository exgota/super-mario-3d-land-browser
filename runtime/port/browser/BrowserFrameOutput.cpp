// SPDX-License-Identifier: GPL-2.0-or-later
#include "BrowserFrameOutput.h"

#include <array>
#include <atomic>
#include <cstring>
#include <iostream>
#include <limits>
#include "core/core.h"
#include "core/core_timing.h"
#include "video_core/gpu.h"
#include "video_core/renderer_software/renderer_software.h"

namespace {
constexpr std::size_t slot_count = 2;
constexpr std::size_t screen_capacity = 1024 * 1024;
enum class SlotState : std::uint32_t { Free, Writing, Ready, Reading };
struct FrameSlot {
    std::atomic<SlotState> state{SlotState::Free};
    std::array<std::uint32_t, 12> metadata{};
    std::array<std::array<std::uint8_t, screen_capacity>, 2> pixels{};
};
static_assert(std::atomic<SlotState>::is_always_lock_free);
std::array<FrameSlot, slot_count> slots;
std::atomic<bool> enabled{};
std::atomic<bool> failed{};
std::size_t producer_cursor{}; // CPU thread only
std::size_t consumer_cursor{}; // outer runtime worker only
std::uint64_t last_frame{}, unique_frames{}, published{}, full_drops{}, unavailable{},
    duplicate_polls{}, frame_gaps{}, geometry_errors{}; // CPU thread only

FrameSlot* ReadingSlot(std::uint32_t lease) {
    if (lease < 1 || lease > slot_count) return nullptr;
    auto& slot = slots[lease - 1];
    return slot.state.load(std::memory_order_acquire) == SlotState::Reading ? &slot : nullptr;
}

bool ValidScreen(const SwRenderer::ScreenInfo& screen) {
    const auto bytes = std::uint64_t{screen.width} * screen.height * 4;
    return screen.width > 0 && screen.height > 0 && screen.width <= 4096 &&
           screen.height <= 4096 && bytes <= screen_capacity && screen.pixels.size() == bytes;
}
}

void Port::StartBrowserFrameOutput() {
    // A worker runs one session. No prior consumer exists when main starts.
    enabled.store(true, std::memory_order_release);
}

void Port::SampleBrowserFrameOutput() {
    if (!enabled.load(std::memory_order_acquire) || failed.load(std::memory_order_relaxed)) return;
    auto& system = Core::System::GetInstance();
    auto& renderer = system.GPU().Renderer();
    const auto frame = renderer.GetCurrentFrame();
    if (!frame) return;
    if (frame == last_frame) { ++duplicate_polls; return; }
    if (frame < last_frame) {
        ++geometry_errors;
        failed.store(true, std::memory_order_release);
        return;
    }
    if (last_frame) frame_gaps += frame - last_frame - 1;
    last_frame = frame;
    ++unique_frames;
    auto* software = dynamic_cast<SwRenderer::RendererSoftware*>(&renderer);
    if (!software) {
        ++geometry_errors;
        failed.store(true, std::memory_order_release);
        return;
    }
    const std::array<const SwRenderer::ScreenInfo*, 2> screens{
        &software->Screen(VideoCore::ScreenId::TopLeft),
        &software->Screen(VideoCore::ScreenId::Bottom)};
    if (!screens[0]->width || !screens[0]->height || !screens[1]->width || !screens[1]->height) {
        ++unavailable;
        return;
    }
    if (!ValidScreen(*screens[0]) || !ValidScreen(*screens[1]) ||
        published >= std::numeric_limits<std::uint32_t>::max()) {
        ++geometry_errors;
        failed.store(true, std::memory_order_release);
        return;
    }
    auto& slot = slots[producer_cursor];
    auto expected = SlotState::Free;
    if (!slot.state.compare_exchange_strong(expected, SlotState::Writing,
                                            std::memory_order_acquire)) {
        ++full_drops;
        return;
    }
    const auto ticks = static_cast<std::uint64_t>(system.CoreTiming().GetTicks());
    slot.metadata = {1, static_cast<std::uint32_t>(++published),
                     static_cast<std::uint32_t>(frame), static_cast<std::uint32_t>(frame >> 32),
                     static_cast<std::uint32_t>(ticks), static_cast<std::uint32_t>(ticks >> 32)};
    for (std::size_t index = 0; index < screens.size(); ++index) {
        const auto& screen = *screens[index];
        // Software output is already landscape row-major, as in the accepted
        // capture observer. The storage dimensions are reversed for display.
        slot.metadata[6 + index * 3] = screen.height;
        slot.metadata[7 + index * 3] = screen.width;
        slot.metadata[8 + index * 3] = static_cast<std::uint32_t>(screen.pixels.size());
        std::memcpy(slot.pixels[index].data(), screen.pixels.data(), screen.pixels.size());
    }
    slot.state.store(SlotState::Ready, std::memory_order_release);
    producer_cursor = (producer_cursor + 1) % slot_count;
}

void Port::StopBrowserFrameOutput() {
    enabled.store(false, std::memory_order_release);
    // Only immutable producer totals are printed. Consumer releases may still
    // finish before normal SDK exit; they never mutate these totals.
    std::cerr << "browser frame output {\"unique_frames\":\"" << unique_frames
              << "\",\"published\":\"" << published << "\",\"full_drops\":\"" << full_drops
              << "\",\"unavailable\":\"" << unavailable << "\",\"duplicate_polls\":\""
              << duplicate_polls << "\",\"frame_gaps\":\"" << frame_gaps
              << "\",\"geometry_errors\":\"" << geometry_errors << "\"}\n";
}

extern "C" std::uint32_t BrowserFrameOutputAcquire() {
    if (failed.load(std::memory_order_acquire)) return std::numeric_limits<std::uint32_t>::max();
    auto expected = SlotState::Ready;
    if (!slots[consumer_cursor].state.compare_exchange_strong(expected, SlotState::Reading,
                                                             std::memory_order_acquire)) return 0;
    const auto lease = static_cast<std::uint32_t>(consumer_cursor + 1);
    consumer_cursor = (consumer_cursor + 1) % slot_count;
    return lease;
}

extern "C" std::uintptr_t BrowserFrameOutputMetadata(std::uint32_t lease) {
    const auto* slot = ReadingSlot(lease);
    return slot ? reinterpret_cast<std::uintptr_t>(slot->metadata.data()) : 0;
}

extern "C" std::uintptr_t BrowserFrameOutputPixels(std::uint32_t lease, std::uint32_t sequence,
                                                  std::uint32_t screen) {
    const auto* slot = ReadingSlot(lease);
    if (!slot || slot->metadata[1] != sequence || (screen != 0 && screen != 2)) return 0;
    return reinterpret_cast<std::uintptr_t>(slot->pixels[screen == 0 ? 0 : 1].data());
}

extern "C" std::uint32_t BrowserFrameOutputRelease(std::uint32_t lease, std::uint32_t sequence) {
    auto* slot = ReadingSlot(lease);
    if (!slot || slot->metadata[1] != sequence) return 1;
    slot->state.store(SlotState::Free, std::memory_order_release);
    return 0;
}
