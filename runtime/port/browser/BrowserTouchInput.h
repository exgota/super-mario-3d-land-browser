// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <cstdint>

namespace Port {
bool InstallBrowserTouchInput();
void UninstallBrowserTouchInput();
}

extern "C" {
// Pixel requests cover x0..319 and y0..239 in the bottom screen's display axes.
// Success queues a coherent position/press. Normal HID sampling earns delivery.
// A released request must use the canonical zero position.
std::uint32_t BrowserTouchInputSetState(std::int32_t x, std::int32_t y, std::uint32_t pressed);
std::uint32_t BrowserTouchInputIsActive();
std::uint32_t BrowserTouchInputPollCount();
// Last CPU request sample: x bits0..8, y9..16, pressed17. No renderer access.
std::uint32_t BrowserTouchInputSampledState();
}
