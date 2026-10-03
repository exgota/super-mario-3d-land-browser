# Browser World 1-1 delivery plan

The owner requested a browser build that plays World 1-1 from start to goal by
Wednesday. The working date is Wednesday, October 7, 2026, America/New_York.
The owner has not specified a cutoff time. Estimates below are elapsed lane work
and verification estimates, not delivery promises. Recorded October 3, 2026.

## Current rendering boundary

The existing browser module executes the statically recompiled ARM CPU, console
services and software PICA renderer inside WebAssembly. A browser worker reads
its actual generated RGBA, and the page displays those pixels with Canvas 2D.
The local server serves runtime files and comparison sidecars and receives
exported evidence. It does not render game pixels or serve the owner's dump.
Azahar runs separately as the reference. WebGPU rendering is not implemented.

The accepted finite browser execution result remains available at
`build/root_browser_execution/build/browser_submission_recovery/result.json`,
SHA-256 `8193373632743cae4162ecb40a120f0d05eec7947202d35196d832a7c49b0b1a`.
This is a finite title-screen execution and displayed-pixel equality result.
Subsequent accepted families add streamed frames, live A/circle-pad/touch input
and original PCM consumption. They do not establish sustained playable speed,
World 1-1 completion, or synchronized continuous sound.

## Ordered remaining work

| Order | Work | Estimate | Exit evidence |
| --- | --- | --- | --- |
| 1 | Finish the surviving native bridge replay and submit its exact comparison and preservation evidence | 20–60 minutes | Completed own-movie replay, unchanged strict window/input/audio/pixel comparison, post-run preservation and integrator submission |
| 2 | Extend the browser runtime to full-level sessions and required controls, with bounded recording and ordinary play free of per-command capture overhead; measure execution costs | 6–12 hours | Real browser session reaches gameplay, controls enter normal HID, finite recording/stop limits hold, CPU/render/frame-delivery measurements |
| 3 | Implement browser WebGPU PICA rendering and address measured performance bottlenecks | 24–48 hours, highest uncertainty | Actual WebGPU draws in the browser; selected original command streams and resulting pixels compare against Azahar; sustained performance is measured |
| 4 | Record and replay a complete natural World 1-1 start-to-goal route | 8–16 hours | Original input movie and initial state; goal reached visibly in original and port; first divergences resolved |
| 5 | Complete Section 7 player, camera, RNG, timer and coin comparisons using verified Pro proposals | 12–24 hours, answer-dependent | Grounded player/update selection, same-input fixed-update observations and passing whole-level regression suite |
| 6 | Verify whole-level browser play, live controls and sound through the goal, then deliver the runnable browser handoff | 6–12 hours | Actual browser reaches goal, rerunnable suite and preserved inputs, explicit runtime/source split and documented local setup |

Total estimate is approximately 57–113 hours. WebGPU coverage and browser CPU
speed are the largest schedule risks. A measured failure or missing evidence
changes the estimate; it does not earn a milestone or a substitute success.

## Renderer implementation plan

Keep the ARM execution and console-service layers in WebAssembly. Translate
PICA200 command state, shader execution and rasterization into a browser WebGPU
service, outside the decompiled game files. Use Azahar's existing software
renderer as the native oracle. Measure CPU, rasterization, readback and frame
copy costs before choosing optimizations. The browser software renderer supplies
a current correctness baseline, not a proven real-time performance baseline.

Address-based source replacement remains the port architecture. The matching
ARMCC build stays authoritative. The current browser module does not establish
complete adapter coverage for every rank-O function now on main. Report the
actual decompiled/recompiled split and hold the pure source claim until the
recompiled share reaches zero. No game data or gameplay screenshots enter git.

## Active work and limits

The current bridge replay survives the relay interruption and is not restarted.
It replays only the new reference's own movie and initial snapshot. Its inclusive
window is 8400..9000 presentations, with 2400 seconds in the provider and 2460
seconds in the outer controller, 512 MiB PICA, 1 GiB monitored capture output,
200000 files, 256 MiB per file, a 15 GiB free floor and two-second polling.
Aggregate limits are monitored ceilings, not filesystem quotas.

The existing implementation helper may finish only its already assigned passive
minimum-tick selector, then stop. That source family is parked until the state
comparison step needs it. No new reviewer or unrelated harness work is started.
Pro receives layout and typed update questions. Only the integrator moves main,
ranks and ledger. Finished port branches carry no matching claims.
