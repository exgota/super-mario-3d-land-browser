// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <cstdint>

namespace Port {
void StartBrowserFrameOutput();
void SampleBrowserFrameOutput();
void StopBrowserFrameOutput();
}

extern "C" {
// A lease is a one-based slot index. Metadata is twelve uint32 words:
// version, sequence, renderer frame low/high, HID sample ticks low/high,
// top display width/height/bytes, bottom display width/height/bytes.
std::uint32_t BrowserFrameOutputAcquire();
std::uintptr_t BrowserFrameOutputMetadata(std::uint32_t lease);
std::uintptr_t BrowserFrameOutputPixels(std::uint32_t lease, std::uint32_t sequence,
                                      std::uint32_t screen);
std::uint32_t BrowserFrameOutputRelease(std::uint32_t lease, std::uint32_t sequence);
}
