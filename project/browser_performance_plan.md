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
gain from shared-host timing alone.

The first actual short profile passes all seven unchanged original 360 outputs,
movie/snapshot delivery, Canvas identity, full audio source consumption, CPU0
393,977,876 instructions, CPU1 zero and both fallbacks zero. Comparison SHA-256:
`d07bc02db62978e078d76c53e943e9c2ee2b123e864b7302d21876d63c66f763`.

Presentations 180 through 360 take 18,123.695 ms. Exclusive timings cover 99.9545%
of that main execution thread window. Nested time is subtracted. Rasterization
includes waiting for workers. These are startup costs, not a World 1-1 profile.

| Cost | Measured ms/frame | Instrumented wall share | Plan and estimated gain |
| --- | ---: | ---: | --- |
| Triangle clipping/rasterization wait | 86.327 | 85.738% | Test O3 only for the software rasterizer with contraction disabled. A conditional 2–4 times category gain gives 1.75–2.80 times overall. Actual gain is unverified. |
| Translated execution including instruction callbacks | 8.986 | 8.924% | After graphics, profile the instruction callback/translation split and test bounded per-file runtime optimization. A conditional 1.3–2 times category gain gives 1.021–1.047 times overall before graphics changes. |
| Outer static CPU scheduling/dispatch | 2.510 | 2.493% | Calibrate first. Do not optimize this scope yet: frequent timing calls inflate it. Even removing the complete measured category caps the baseline gain at 1.026 times. Expected immediate gain is zero because no change is selected. |

These estimates use Amdahl's law and the observed instrumented shares. They are
conditional cost-model scenarios, not achieved speedups. Calibration below shows
why the CPU percentages cannot yet be treated as intrinsic costs.

A first scanline candidate runs bounding boxes up to 4,096 pixels inline and
batches larger triangles into at most one row range per existing worker. It
passes every original 360 output, but profiled rasterization rises to 112.898
ms/frame, from 86.327. Profile throughput falls from 9.932 to 7.843/s; full warm
replay falls from 9.050 to 7.278/s. Shared-host timings do not prove causation.
The candidate is rejected as a performance improvement. Unpublished public
drafts are retained locally. No production scheduling change. Comparison SHA-256:
`b75c09d8fac3f89b3b3704e2abadb85df5909fab39a1d66f934d4576cede77f5`.

Its post-run calibration measures 100,000 empty scopes in 34.290 ms, about 343
ns/call, with 165 ns/call inside the measured scope. The profile brackets 1,888,507
translated dispatches, estimating about 3.6 ms/frame of timing overhead split
between execution and its parent. Calibration is a later empty loop, not a
same-load subtraction proof. Keep the raw numbers and this limitation.

Both short browsers and identity-verified servers closed after export. Original
inputs and failed candidates remain intact. Next candidate retains original row
scheduling and changes only the rasterizer's compile recipe from O1 to O3 plus
`-ffp-contract=off`. No fast-math, global flags, game-source or CPU changes.

The per-file O3 candidate passes all seven unchanged 360 outputs and delivery
checks. Comparison SHA-256:
`d76930cc0d6fcb3ae0cf9dd048b8e94f4bbd99bffc3c2956cbe4aca47a0ac0bf`.
Its 180-frame profile takes 20,503.355 ms, 8.779/s. Rasterization takes 97.664
ms/frame; translated execution 10.081, outer dispatch 2.773. Other unchanged
costs also rise, so this shared-host run cannot establish a compiler regression.
It establishes no gain. The candidate remains private and is not shipped.

## GPU play renderer direction

The owner's October 3 steering replaces software-rasterizer tuning with an
independently written WebGL2 play renderer. Keep the software path unchanged as
the exactness reference. A more detailed private rasterizer timer compiled but
was never run, and is parked. No further software tuning is planned.

GPU play acceptance uses visual equivalence and measured frame rate. Report
per-frame difference against the software path and actual throughput on the
unchanged short 360 replay. Keep images local. Scope initial GPU functionality
from the preserved World 1-1 command window, then extend as measured needs appear.
Use Azahar as a design reference without copying its renderer implementation.
No licensing decision is made by this lane. CPU, timing, input and audio retain
their existing providers. Full-speed World 1-1 remains unverified.
Its short original 360 equality and performance measurements are pending.

Then observe an ordinary live-input start-to-goal browser run at the measured
cadence with sound. Whole-level goal, Section 7 state comparison, synchronized
sound and physical mobile behavior remain unverified.
