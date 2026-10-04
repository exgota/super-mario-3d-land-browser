#pragma once
#include <cstdint>

namespace Memory { class MemorySystem; }
namespace Pica { class PicaCore; struct DisplayTransferConfig; }

namespace Port {
bool ResolveBrowserWebGlRenderSurface(std::uint32_t address, std::uint32_t width,
    std::uint32_t height, std::uint32_t format, std::uint32_t& identifier);
bool TransferBrowserWebGlDisplaySurface(Memory::MemorySystem& memory,
    const Pica::DisplayTransferConfig& configuration);
bool PresentBrowserWebGlDisplayFrame(Memory::MemorySystem& memory, Pica::PicaCore& pica,
    std::uint64_t renderer_frame, std::uint64_t sampled_ticks);
bool BrowserWebGlDisplayFrameHandled(std::uint64_t renderer_frame);
void FlushBrowserWebGlDisplayRegion(std::uint32_t address, std::uint32_t bytes);
void InvalidateBrowserWebGlDisplayRegion(std::uint32_t address, std::uint32_t bytes);
void FlushBrowserWebGlDisplaySurfaces();
}
