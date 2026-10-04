// SPDX-License-Identifier: GPL-2.0-or-later
#pragma once
#include <cstdint>
#include <filesystem>
#include <string>

namespace VideoCore { class RendererBase; }
namespace Pica { class PicaCore; }
namespace Memory { class MemorySystem; }

namespace Port {
// One selected session per module. Unset selection returns false without files.
// The frontend retains its explicit bounded wall deadline and input/audio policy.
bool InitializeGameplaySession(const std::filesystem::path& directory);
bool GameplaySessionEnabled();
std::uint32_t GameplaySessionPresentationLimit();
void RecordGameplayGspCommand();
void RecordGameplayPicaCommandList(std::uint32_t size);
void RecordGameplayBufferSwap(std::uint32_t screen_id, std::uint64_t ticks);
void RecordGameplayVBlank();
void RecordGameplayHardwareRegisterWrite();
void RecordGameplayColorFill();
bool RecordGameplayPresentation(const VideoCore::RendererBase& renderer,
                                const Pica::PicaCore& pica, Memory::MemorySystem& memory,
                                std::uint64_t ticks);
// Before Finish, complete means a successful final-boundary export for loop exit.
// Finish must receive GameplaySessionOutcome() for that successful endpoint;
// any frontend failure outcome makes the persisted result incomplete. A result
// is usable only after Finish returns successfully and the frontend succeeds.
bool GameplaySessionComplete();
std::string GameplaySessionOutcome();
void FinishGameplaySession(const std::string& outcome, std::uint64_t ticks);
}

extern "C" {
// Return 0 when Stop is accepted/already pending, or 2 when inactive.
// Stop remains pending until a natural presentation after top-screen submission.
std::uint32_t BrowserGameplaySessionRequestStop();
std::uint32_t BrowserGameplaySessionIsActive();
}
