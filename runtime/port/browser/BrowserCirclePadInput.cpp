// SPDX-License-Identifier: GPL-2.0-or-later
#include "BrowserCirclePadInput.h"

#include <atomic>
#include <iostream>
#include <memory>
#include <string>
#include <tuple>
#include "common/settings.h"
#include "core/frontend/input.h"

namespace {
constexpr char factory_name[] = "root_port_browser_circle_pad";
constexpr std::uint32_t active_mask = 0x80000000U;
constexpr std::uint32_t coordinate_mask = 0x1ffU;
constexpr std::int32_t maximum_coordinate = 154;
constexpr std::uint32_t neutral_position = 154U | (154U << 9);
std::atomic<std::uint32_t> position{neutral_position};
std::atomic<std::uint32_t> sampled_position{neutral_position};
std::atomic<std::uint32_t> poll_count{};
static_assert(std::atomic<std::uint32_t>::is_always_lock_free);
bool installed{}; // application CPU thread only
std::string previous_profile;

class BrowserCirclePadDevice final : public Input::AnalogDevice {
public:
    std::tuple<float, float> GetStatus() const override {
        const auto sample = position.load(std::memory_order_acquire);
        sampled_position.store(sample & ~active_mask, std::memory_order_release);
        poll_count.fetch_add(1, std::memory_order_release);
        if (!(sample & active_mask)) return {0.0f, 0.0f};
        const auto x = static_cast<std::int32_t>(sample & coordinate_mask) - maximum_coordinate;
        const auto y = static_cast<std::int32_t>((sample >> 9) & coordinate_mask) - maximum_coordinate;
        return {static_cast<float>(x) / 154.0f, static_cast<float>(y) / 154.0f};
    }
};

class BrowserCirclePadFactory final : public Input::Factory<Input::AnalogDevice> {
public:
    std::unique_ptr<Input::AnalogDevice> Create(const Common::ParamPackage&) override {
        return std::make_unique<BrowserCirclePadDevice>();
    }
};
}

bool Port::InstallBrowserCirclePadInput() {
    if (installed) return false;
    previous_profile = Settings::values.current_input_profile.analogs[Settings::NativeAnalog::CirclePad];
    Input::RegisterFactory<Input::AnalogDevice>(factory_name, std::make_shared<BrowserCirclePadFactory>());
    Settings::values.current_input_profile.analogs[Settings::NativeAnalog::CirclePad] =
        "engine:root_port_browser_circle_pad";
    sampled_position.store(neutral_position, std::memory_order_relaxed);
    poll_count.store(0, std::memory_order_relaxed);
    installed = true;
    position.store(active_mask | neutral_position, std::memory_order_release);
    return true;
}

void Port::UninstallBrowserCirclePadInput() {
    if (!installed) return;
    position.store(neutral_position, std::memory_order_release);
    // Successful capture return has already destroyed the System/HID devices.
    Input::UnregisterFactory<Input::AnalogDevice>(factory_name);
    Settings::values.current_input_profile.analogs[Settings::NativeAnalog::CirclePad] = previous_profile;
    installed = false;
    std::cerr << "browser circle pad closed after " << poll_count.load(std::memory_order_acquire)
              << " device polls; profile restored\n";
}

extern "C" std::uint32_t BrowserCirclePadInputSetPosition(std::int32_t x, std::int32_t y) {
    if (x < -maximum_coordinate || x > maximum_coordinate ||
        y < -maximum_coordinate || y > maximum_coordinate ||
        std::int64_t{x} * x + std::int64_t{y} * y > std::int64_t{maximum_coordinate} * maximum_coordinate)
        return 1;
    const auto requested = active_mask | static_cast<std::uint32_t>(x + maximum_coordinate) |
        (static_cast<std::uint32_t>(y + maximum_coordinate) << 9);
    auto current = position.load(std::memory_order_acquire);
    while (current & active_mask) {
        if (position.compare_exchange_strong(current, requested, std::memory_order_acq_rel,
                                             std::memory_order_acquire)) return 0;
    }
    return 2;
}

extern "C" std::uint32_t BrowserCirclePadInputIsActive() {
    return (position.load(std::memory_order_acquire) & active_mask) != 0;
}

extern "C" std::uint32_t BrowserCirclePadInputPollCount() {
    return poll_count.load(std::memory_order_acquire);
}

extern "C" std::uint32_t BrowserCirclePadInputSampledPosition() {
    return sampled_position.load(std::memory_order_acquire);
}
