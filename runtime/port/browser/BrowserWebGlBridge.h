#pragma once

#include <cstdint>

namespace Memory { class MemorySystem; }
namespace Pica { class PicaCore; struct OutputVertex; }

namespace Port {
void PrepareBrowserWebGlDraw(Memory::MemorySystem& memory, Pica::PicaCore& pica);
void DrawBrowserWebGlTriangle(Memory::MemorySystem& memory, Pica::PicaCore& pica,
                             const Pica::OutputVertex& first,
                             const Pica::OutputVertex& second,
                             const Pica::OutputVertex& third);
void FinishBrowserWebGlDraw();
void SynchronizeBrowserWebGlColor();
void InvalidateBrowserWebGlSurfaces(std::uint32_t address, std::uint32_t bytes);
}
