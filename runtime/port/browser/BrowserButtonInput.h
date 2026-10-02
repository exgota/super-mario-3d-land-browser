// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <cstdint>

namespace Port {
void InstallBrowserButtonInput();
void UninstallBrowserButtonInput();
}

extern "C" {
std::uint32_t BrowserButtonInputSetHeldState(std::uint32_t held);
std::uint32_t BrowserButtonInputIsActive();
std::uint32_t BrowserButtonInputPollCount();
std::uint32_t BrowserButtonInputRendererFrame();
}
