// Independently authored presentation admission. No guest pixels are read here.
#include "BrowserWebGlPresentation.h"

#include <emscripten/emscripten.h>

namespace {
using Word = std::uint32_t;
Port::BrowserWebGlPresentationStatistics statistics;
static_assert(sizeof(Port::BrowserWebGlPresentationScreen) == 11 * sizeof(Word));

EM_JS(Word, SubmitBrowserWebGlPresentation, (const Word* screens, Word count, Word frameLow,
                               Word frameHigh, Word ticksLow, Word ticksHigh), {
    const submit = globalThis.submitBrowserWebGlPresentation;
    if (typeof submit !== 'function') return 0;
    const descriptions = [];
    for (let index = 0; index < count; ++index) {
        const offset = (screens >>> 2) + index * 11;
        descriptions.push({screenIdentifier: HEAPU32[offset], surfaceIdentifier: HEAPU32[offset + 1],
            sourceX: HEAPU32[offset + 2], sourceY: HEAPU32[offset + 3],
            sourceWidth: HEAPU32[offset + 4], sourceHeight: HEAPU32[offset + 5],
            width: HEAPU32[offset + 6], height: HEAPU32[offset + 7],
            rotationQuarterTurns: HEAPU32[offset + 8], flags: HEAPU32[offset + 9],
            colorFormat: HEAPU32[offset + 10]});
    }
    // The transport receives owned numbers and strings. No Wasm view survives publication.
    const descriptor = {schemaVersion: 1,
        rendererFrame: ((BigInt(frameHigh >>> 0) << 32n) | BigInt(frameLow >>> 0)).toString(),
        sampledTicks: ((BigInt(ticksHigh >>> 0) << 32n) | BigInt(ticksLow >>> 0)).toString(), screens: descriptions};
    const result = submit(descriptor);
    return Number.isInteger(result) && result >= 0 && result <= 2 ? result : 0;
});
}

Port::BrowserWebGlPresentationResult Port::TryPresentBrowserWebGlFrame(
    const BrowserWebGlPresentationScreen* screens, Word count,
    std::uint64_t rendererFrame, std::uint64_t sampledTicks) {
    if (!screens || !count || count > 2) {
        ++statistics.unsupportedFrames;
        return BrowserWebGlPresentationResult::Unsupported;
    }
    for (Word index = 0; index < count; ++index) {
        const auto& screen = screens[index];
        const bool rotated = (screen.rotationQuarterTurns & 1) != 0;
        if ((screen.screenIdentifier != 0 && screen.screenIdentifier != 2) ||
            !screen.surfaceIdentifier || !screen.sourceWidth || !screen.sourceHeight ||
            screen.rotationQuarterTurns > 3 || screen.flags > 1 || screen.colorFormat > 4 ||
            screen.width != (rotated ? screen.sourceHeight : screen.sourceWidth) ||
            screen.height != (rotated ? screen.sourceWidth : screen.sourceHeight) ||
            (index && screens[0].screenIdentifier == screen.screenIdentifier)) {
            ++statistics.unsupportedFrames;
            return BrowserWebGlPresentationResult::Unsupported;
        }
    }
    const auto result = SubmitBrowserWebGlPresentation(reinterpret_cast<const Word*>(screens), count,
        static_cast<Word>(rendererFrame), static_cast<Word>(rendererFrame >> 32),
        static_cast<Word>(sampledTicks), static_cast<Word>(sampledTicks >> 32));
    if (result == static_cast<Word>(BrowserWebGlPresentationResult::Submitted)) {
        ++statistics.submittedFrames;
        return BrowserWebGlPresentationResult::Submitted;
    }
    if (result == static_cast<Word>(BrowserWebGlPresentationResult::Dropped)) {
        ++statistics.droppedFrames;
        return BrowserWebGlPresentationResult::Dropped;
    }
    ++statistics.unsupportedFrames;
    return BrowserWebGlPresentationResult::Unsupported;
}

const Port::BrowserWebGlPresentationStatistics& Port::GetBrowserWebGlPresentationStatistics() {
    return statistics;
}
