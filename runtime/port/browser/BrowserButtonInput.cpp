// SPDX-License-Identifier: GPL-2.0-or-later
#include "BrowserButtonInput.h"
#include "BrowserFrameOutput.h"

#include <array>
#include <atomic>
#include <memory>
#include <string>
#include "common/settings.h"
#include "core/core.h"
#include "core/frontend/input.h"
#include "video_core/gpu.h"
#include "video_core/renderer_base.h"

namespace {
constexpr char factory_name[] = "root_port_browser_button";
std::atomic<std::uint32_t> active{};
std::atomic<std::uint32_t> gameplay_active{};
std::atomic<std::uint32_t> held_state{};
std::atomic<std::uint32_t> poll_count{};
std::atomic<std::uint32_t> renderer_frame{};
std::string previous_button;
struct ButtonBinding {
    Settings::NativeButton::Values button;
    std::uint32_t mask;
};
// Public Azahar HID PadState bits, not NativeButton enum indices.
constexpr std::array<ButtonBinding, 12> gameplay_bindings{{
    {Settings::NativeButton::A, 1u << 0},
    {Settings::NativeButton::B, 1u << 1},
    {Settings::NativeButton::Select, 1u << 2},
    {Settings::NativeButton::Start, 1u << 3},
    {Settings::NativeButton::Right, 1u << 4},
    {Settings::NativeButton::Left, 1u << 5},
    {Settings::NativeButton::Up, 1u << 6},
    {Settings::NativeButton::Down, 1u << 7},
    {Settings::NativeButton::R, 1u << 8},
    {Settings::NativeButton::L, 1u << 9},
    {Settings::NativeButton::X, 1u << 10},
    {Settings::NativeButton::Y, 1u << 11},
}};
constexpr std::uint32_t supported_button_mask = 0x0fff;
std::array<std::string, gameplay_bindings.size()> previous_gameplay_buttons;

class BrowserButtonDevice final : public Input::ButtonDevice {
public:
    explicit BrowserButtonDevice(std::uint32_t mask) : mask(mask) {}
    bool GetStatus() const override {
        // This runs in normal HID polling on the emulation thread, as do the
        // accepted original input observers. Only atomics cross to the worker.
        if (mask == 1) {
            const auto frame = Core::System::GetInstance().GPU().Renderer().GetCurrentFrame();
            renderer_frame.store(static_cast<std::uint32_t>(frame), std::memory_order_release);
            poll_count.fetch_add(1, std::memory_order_release);
            Port::SampleBrowserFrameOutput();
        }
        return (held_state.load(std::memory_order_acquire) & mask) != 0;
    }
private:
    const std::uint32_t mask;
};

class BrowserButtonFactory final : public Input::Factory<Input::ButtonDevice> {
public:
    std::unique_ptr<Input::ButtonDevice> Create(const Common::ParamPackage& parameters) override {
        return std::make_unique<BrowserButtonDevice>(
            static_cast<std::uint32_t>(parameters.Get("button_mask", 1)));
    }
};
}

void Port::InstallBrowserButtonInput(bool allow_button_input, bool gameplay_controls) {
    previous_button = Settings::values.current_input_profile.buttons[Settings::NativeButton::A];
    held_state.store(0, std::memory_order_relaxed);
    poll_count.store(0, std::memory_order_relaxed);
    renderer_frame.store(0, std::memory_order_relaxed);
    Input::RegisterFactory<Input::ButtonDevice>(factory_name, std::make_shared<BrowserButtonFactory>());
    if (gameplay_controls) {
        for (std::size_t index = 0; index < gameplay_bindings.size(); ++index) {
            const auto& binding = gameplay_bindings[index];
            auto& configured = Settings::values.current_input_profile.buttons[binding.button];
            previous_gameplay_buttons[index] = configured;
            configured = "engine:root_port_browser_button,button_mask:" + std::to_string(binding.mask);
        }
    } else {
        Settings::values.current_input_profile.buttons[Settings::NativeButton::A] =
            "engine:root_port_browser_button";
    }
    gameplay_active.store(gameplay_controls ? 1 : 0, std::memory_order_release);
    active.store(allow_button_input ? 1 : 0, std::memory_order_release);
}

void Port::UninstallBrowserButtonInput() {
    active.store(0, std::memory_order_release);
    const auto gameplay_controls = gameplay_active.exchange(0, std::memory_order_acq_rel);
    held_state.store(0, std::memory_order_release);
    // The headless capture has shut down its system before returning.
    Input::UnregisterFactory<Input::ButtonDevice>(factory_name);
    if (gameplay_controls) {
        for (std::size_t index = 0; index < gameplay_bindings.size(); ++index)
            Settings::values.current_input_profile.buttons[gameplay_bindings[index].button] =
                previous_gameplay_buttons[index];
    } else {
        Settings::values.current_input_profile.buttons[Settings::NativeButton::A] = previous_button;
    }
}

extern "C" std::uint32_t BrowserButtonInputSetHeldState(std::uint32_t held) {
    if (held > 1) return 1;
    if (!active.load(std::memory_order_acquire)) return 2;
    if (held) held_state.fetch_or(1, std::memory_order_release);
    else held_state.fetch_and(~std::uint32_t{1}, std::memory_order_release);
    return 0;
}

extern "C" std::uint32_t BrowserButtonInputSetButtonMask(std::uint32_t buttons) {
    if (buttons & ~supported_button_mask) return 1;
    if (!active.load(std::memory_order_acquire)) return 2;
    if (!gameplay_active.load(std::memory_order_acquire) && (buttons & ~std::uint32_t{1})) return 3;
    held_state.store(buttons, std::memory_order_release);
    return 0;
}

extern "C" std::uint32_t BrowserButtonInputHeldButtonMask() {
    // Requested host state. Actual HID delivery is established by the movie.
    return held_state.load(std::memory_order_acquire);
}

extern "C" std::uint32_t BrowserButtonInputHasGameplayControls() {
    return gameplay_active.load(std::memory_order_acquire);
}

extern "C" std::uint32_t BrowserButtonInputIsActive() {
    return active.load(std::memory_order_acquire);
}

extern "C" std::uint32_t BrowserButtonInputPollCount() {
    return poll_count.load(std::memory_order_acquire);
}

extern "C" std::uint32_t BrowserButtonInputRendererFrame() {
    return renderer_frame.load(std::memory_order_acquire);
}
