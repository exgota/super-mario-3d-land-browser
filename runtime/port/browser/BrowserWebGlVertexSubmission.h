#pragma once

#include <cstdint>

namespace Memory { class MemorySystem; }
namespace Pica { class PicaCore; }

namespace Port {
// Root owns draw-state preparation and the private primitive-assembler/winding guard.
void PrepareBrowserWebGlDraw(Memory::MemorySystem& memory, Pica::PicaCore& pica);

// True submits every requested triangle. False leaves the existing CPU path responsible.
bool TryDrawBrowserWebGlVertexBatch(Memory::MemorySystem& memory, Pica::PicaCore& pica,
                                  bool indexed);
void InvalidateBrowserWebGlVertexResources(std::uint32_t address, std::uint32_t bytes);

struct BrowserWebGlVertexSubmissionStatistics {
    std::uint64_t submitted_draws{};
    std::uint64_t submitted_vertices{};
    std::uint64_t fallback_draws{};
    std::uint64_t invalid_memory_ranges{};
    std::uint64_t scanned_index_bytes{};
};
const BrowserWebGlVertexSubmissionStatistics& GetBrowserWebGlVertexSubmissionStatistics();
}
