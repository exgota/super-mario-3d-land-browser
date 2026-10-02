// SPDX-License-Identifier: GPL-2.0-or-later
#include "BrowserButtonInput.h"

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
std::atomic<std::uint32_t> held_state{};
std::atomic<std::uint32_t> poll_count{};
std::atomic<std::uint32_t> renderer_frame{};
std::string previous_button;

class BrowserButtonDevice final : public Input::ButtonDevice {
public:
    bool GetStatus() const override {
        // This runs in normal HID polling on the emulation thread, as do the
        // accepted original input observers. Only atomics cross to the worker.
        const auto frame = Core::System::GetInstance().GPU().Renderer().GetCurrentFrame();
        renderer_frame.store(static_cast<std::uint32_t>(frame), std::memory_order_release);
        poll_count.fetch_add(1, std::memory_order_release);
        return held_state.load(std::memory_order_acquire) != 0;
    }
};

class BrowserButtonFactory final : public Input::Factory<Input::ButtonDevice> {
public:
    std::unique_ptr<Input::ButtonDevice> Create(const Common::ParamPackage&) override {
        return std::make_unique<BrowserButtonDevice>();
    }
};
}

void Port::InstallBrowserButtonInput() {
    previous_button = Settings::values.current_input_profile.buttons[Settings::NativeButton::A];
    held_state.store(0, std::memory_order_relaxed);
    poll_count.store(0, std::memory_order_relaxed);
    renderer_frame.store(0, std::memory_order_relaxed);
    Input::RegisterFactory<Input::ButtonDevice>(factory_name, std::make_shared<BrowserButtonFactory>());
    Settings::values.current_input_profile.buttons[Settings::NativeButton::A] =
        "engine:root_port_browser_button";
    active.store(1, std::memory_order_release);
}

void Port::UninstallBrowserButtonInput() {
    active.store(0, std::memory_order_release);
    held_state.store(0, std::memory_order_release);
    // The headless capture has shut down its system before returning.
    Input::UnregisterFactory<Input::ButtonDevice>(factory_name);
    Settings::values.current_input_profile.buttons[Settings::NativeButton::A] = previous_button;
}

extern "C" std::uint32_t BrowserButtonInputSetHeldState(std::uint32_t held) {
    if (held > 1) return 1;
    if (!active.load(std::memory_order_acquire)) return 2;
    held_state.store(held, std::memory_order_release);
    return 0;
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
