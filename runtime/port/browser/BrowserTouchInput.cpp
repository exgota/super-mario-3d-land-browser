// SPDX-License-Identifier: GPL-2.0-or-later
#include "BrowserTouchInput.h"

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include "common/settings.h"
#include "core/frontend/input.h"

namespace {
constexpr char factory_name[] = "root_port_browser_touch";
constexpr std::uint32_t active_mask = 0x80000000U;
constexpr std::uint32_t pressed_mask = 1U << 17;
constexpr std::uint32_t x_mask = 0x1ffU;
constexpr std::uint32_t y_mask = 0xffU;
std::atomic<std::uint32_t> touch_state{};
std::atomic<std::uint32_t> sampled_state{};
std::atomic<std::uint32_t> poll_count{};
static_assert(std::atomic<std::uint32_t>::is_always_lock_free);
bool installed{}; // application CPU thread only
std::string previous_touch_profile;
bool previous_button_touch{};
bool previous_controller_touch{};

class BrowserTouchDevice final : public Input::TouchDevice {
public:
    std::tuple<float, float, bool> GetStatus() const override {
        const auto sample = touch_state.load(std::memory_order_acquire);
        sampled_state.store(sample & ~active_mask, std::memory_order_release);
        poll_count.fetch_add(1, std::memory_order_release);
        if ((sample & (active_mask | pressed_mask)) != (active_mask | pressed_mask))
            return {0.0f, 0.0f, false};
        const auto x = sample & x_mask;
        const auto y = (sample >> 9) & y_mask;
        // HID scales and truncates. Pixel centers avoid a rounded float
        // falling immediately below the requested integer boundary.
        return {(static_cast<float>(x) + 0.5f) / 320.0f,
                (static_cast<float>(y) + 0.5f) / 240.0f, true};
    }
};

class BrowserTouchFactory final : public Input::Factory<Input::TouchDevice> {
public:
    std::unique_ptr<Input::TouchDevice> Create(const Common::ParamPackage&) override {
        return std::make_unique<BrowserTouchDevice>();
    }
};
}

bool Port::InstallBrowserTouchInput() {
    if (installed) return false;
    auto& profile = Settings::values.current_input_profile;
    previous_touch_profile = profile.touch_device;
    previous_button_touch = profile.use_touch_from_button;
    previous_controller_touch = profile.use_touchpad;
    Input::RegisterFactory<Input::TouchDevice>(factory_name, std::make_shared<BrowserTouchFactory>());
    profile.touch_device = "engine:root_port_browser_touch";
    // Neutral primary touch otherwise consults two ordinary fallback sources.
    // Explicit browser touch owns this subsystem for its finite recording.
    profile.use_touch_from_button = false;
    profile.use_touchpad = false;
    sampled_state.store(0, std::memory_order_relaxed);
    poll_count.store(0, std::memory_order_relaxed);
    installed = true;
    touch_state.store(active_mask, std::memory_order_release);
    return true;
}

void Port::UninstallBrowserTouchInput() {
    if (!installed) return;
    touch_state.store(0, std::memory_order_release);
    // Successful capture return has already destroyed the System/HID devices.
    Input::UnregisterFactory<Input::TouchDevice>(factory_name);
    auto& profile = Settings::values.current_input_profile;
    profile.touch_device = previous_touch_profile;
    profile.use_touch_from_button = previous_button_touch;
    profile.use_touchpad = previous_controller_touch;
    installed = false;
    std::cerr << "browser touch closed after " << poll_count.load(std::memory_order_acquire)
              << " device polls; profile restored\n";
}

extern "C" std::uint32_t BrowserTouchInputSetState(std::int32_t x, std::int32_t y,
                                                std::uint32_t pressed) {
    if (x < 0 || x > 319 || y < 0 || y > 239 || pressed > 1 ||
        (!pressed && (x != 0 || y != 0))) return 1;
    const auto requested = active_mask | static_cast<std::uint32_t>(x) |
        (static_cast<std::uint32_t>(y) << 9) | (pressed << 17);
    auto current = touch_state.load(std::memory_order_acquire);
    while (current & active_mask) {
        if (touch_state.compare_exchange_strong(current, requested, std::memory_order_acq_rel,
                                                std::memory_order_acquire)) return 0;
    }
    return 2;
}

extern "C" std::uint32_t BrowserTouchInputIsActive() {
    return (touch_state.load(std::memory_order_acquire) & active_mask) != 0;
}

extern "C" std::uint32_t BrowserTouchInputPollCount() {
    return poll_count.load(std::memory_order_acquire);
}

extern "C" std::uint32_t BrowserTouchInputSampledState() {
    return sampled_state.load(std::memory_order_acquire);
}
