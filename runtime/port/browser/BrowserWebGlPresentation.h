#pragma once

#include <cstdint>

namespace Port {
enum class BrowserWebGlPresentationResult : std::uint32_t {
    Unsupported = 0,
    Submitted = 1,
    Dropped = 2,
};

// The owner resolves display-transfer addresses, scaling and surface lifetime first.
// Source rectangles use bottom-left texture coordinates. Rotation is clockwise.
struct BrowserWebGlPresentationScreen {
    std::uint32_t screenIdentifier{};
    std::uint32_t surfaceIdentifier{};
    std::uint32_t sourceX{}, sourceY{}, sourceWidth{}, sourceHeight{};
    std::uint32_t width{}, height{};
    std::uint32_t rotationQuarterTurns{};
    std::uint32_t flags{}; // Bit zero flips source Y after rotation. Other bits reject.
    std::uint32_t colorFormat{}; // RGBA8, RGB8, RGB565, RGB5A1, RGBA4: zero through four.
};

// Submitted means the owning transport accepted this ordered presentation request.
// Dropped is explicit backpressure. Unsupported requires the coherent reference path.
BrowserWebGlPresentationResult TryPresentBrowserWebGlFrame(
    const BrowserWebGlPresentationScreen* screens, std::uint32_t count,
    std::uint64_t rendererFrame, std::uint64_t sampledTicks);

struct BrowserWebGlPresentationStatistics {
    std::uint64_t submittedFrames{}, droppedFrames{}, unsupportedFrames{};
};
const BrowserWebGlPresentationStatistics& GetBrowserWebGlPresentationStatistics();
}
