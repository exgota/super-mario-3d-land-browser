# Browser World 1-1 performance priority

The owner relayed this priority update on October 3, 2026: "Real-time play now
outranks the last pixels." Finish the active table replay and its comparison,
submit ready work through the integrator, record any residual pixels as a known
gap, and park exactness diagnosis. Do not run the prepared fragment observer or
another full-length replay for exactness. Profile first, then optimize using the
short 360-presentation replay as the exactness regression gate.

The target is owner-played World 1-1 from start to goal in this Mac mini's browser,
at the original game's measured frame rate, with live input and audio. Port
runtime only, Root topic branches through the integrator, no direct main changes,
no subagents or delegated tasks. Short runs reduce competition with the factory.

## Current observed behavior

The final table replay completed the unchanged 9,000-presentation movie. All
9,884,160 PCM channel samples, HID/audio timing and bottom-screen outputs agree
with the original reference. The top screen retains exactly the previous 21
different pixels, top hash `5456cf28`. The top framebuffer also differs. This is
a known parked gap, not an exactness pass or World 1-1 goal acceptance.

Comparison SHA-256:
`6e7d56dc72122f49a0b8a8bfad2ba612379131019bb134d5be2e9018894be834`.
The public audio/fog/depth overlay corrected the former PCM sample and one former
pixel. Later normalization and lighting candidates are private; they did not
correct these remaining pixels. No game data or private diagnostic binaries
enter the submission.

The table replay's 8,999 warm presentation intervals total 1,935.995 seconds,
4.6483 presentations/second. Sound consumes every source frame but underruns at
this speed. Continuous synchronized sound and sustained playable speed remain
unverified. The earlier 8.7/s measurement covered startup only.

## Original cadence measurement

Read-only analysis of the preserved original GPU window, presentations 8,400
through 9,000, observes 601 VBlanks and 601 top-screen buffer submissions. Their
600 intervals have median 4,481,136 guest ticks; 568 intervals equal that value
exactly. Both measured cadences are 59.83122493939037 submissions/second of guest
time, using the unchanged original provider's 268,111,856-tick/second ARM11 clock.
This measures the recorded original execution's submission cadence. It is not
physical-hardware frame timing or proof of unique pixels at every submission.

The browser needs about 12.87 times its current long-run throughput to meet this
target. Expected gains require measured cost shares before implementation.

## Profiling and next work

A five-second macOS sample of the identity-verified T3 preview renderer during
World 1-1 confirms active runtime workers, but native sampling leaves Wasm frames
unnamed. It cannot establish three named frame-cost shares. The page exposes the
JavaScript sampling profiler, but its document policy disables construction and
workers do not expose the API. No alternate browser was launched.

Next: collect bounded runtime timings over a short replay, distinguish CPU
execution/scheduling, software graphics, and presentation/audio/host work, and
report the three largest measured costs per frame with their denominator and
limitations. Choose each optimization from those costs, state its estimated gain,
then rerun the unchanged 360-presentation exactness gate. Do not claim a performance
gain from shared-host timing alone. No top-three shares are established yet.

Then observe an ordinary live-input start-to-goal browser run at the measured
cadence with sound. Whole-level goal, Section 7 state comparison, synchronized
sound and physical mobile behavior remain unverified.
