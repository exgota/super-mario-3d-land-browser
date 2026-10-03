// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <cstdint>

namespace Port {
void InstallBrowserButtonInput(bool allow_button_input = true, bool gameplay_controls = false);
void UninstallBrowserButtonInput();
}

extern "C" {
std::uint32_t BrowserButtonInputSetHeldState(std::uint32_t held);
std::uint32_t BrowserButtonInputSetButtonMask(std::uint32_t buttons);
std::uint32_t BrowserButtonInputHeldButtonMask();
std::uint32_t BrowserButtonInputHasGameplayControls();
std::uint32_t BrowserButtonInputIsActive();
std::uint32_t BrowserButtonInputPollCount();
std::uint32_t BrowserButtonInputRendererFrame();
}
