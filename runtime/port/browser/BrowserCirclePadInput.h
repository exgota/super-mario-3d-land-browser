// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <cstdint>

namespace Port {
bool InstallBrowserCirclePadInput();
void UninstallBrowserCirclePadInput();
}

extern "C" {
// Integer requests lie in the radius-154 disk. Positive x is right, y is up.
// Success queues a coherent pair; normal HID averaging determines delivery.
std::uint32_t BrowserCirclePadInputSetPosition(std::int32_t x, std::int32_t y);
std::uint32_t BrowserCirclePadInputIsActive();
std::uint32_t BrowserCirclePadInputPollCount();
// Last CPU device sample, before HID averaging: biased x bits 0..8, y 9..17.
std::uint32_t BrowserCirclePadInputSampledPosition();
}
