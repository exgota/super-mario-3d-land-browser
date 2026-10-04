# Root browser runtime state

Branch: root/browser-webgl-renderer. Root works only on runtime/port,
tools/static_recompiler and project notes. No main/rank/ledger/factory changes,
submission, runtime migration, deployment, subagents or delegated work.
Existing R1–R4 are independent owner-launched T3 threads.

## Goal and acceptance

Owner wants 60 fps World 1-1 in a browser as soon as possible.
M1 requires a human Mini Chrome AND Safari start-to-goal playthrough with sound,
median interval <=16.9 ms, <1% intervals >33 ms after 10 seconds, zero post-load
underruns, <20 seconds to controllable Mario, peak heap and local recordings.
Current functional/machine checks are preparation. M1 is not complete.
Peer follows M1. iPhone, Web persistence/overlay hooks and migration remain deferred.
Only the integrator moves main. Matching credit from this work is zero.

## Current integrated result

CPU descriptor cache95da integrated as10e0ad9e. R1 ordered GPU bitmap transport
abc8a4b7 integrated as9d02873c. R3 GPU presentation/copy/cache efafe5fb integrated
as3402fbb0. Root display-transfer/guest-read hooks are aee57b21/0601807f.
Source-only commits. Generated code, Wasm, schedules, game data and images stay local.

Actual plain-Chrome World job20261004T145822796727_b709642e completed normally.
Artifact build/browser_gpu_descriptor_candidate includes descriptor-cache CPU,
raw GPU/ring and prepared-draw GPU pthread, without bitmap presentation.
611 display intervals:20.399159/s, median48.01ms, p99119.555ms,
59.083% over33ms. Actual image retains Mario/ground/trees/blocks/castle/HUD.
Entry80.664s. Full40s audio458752source frames/849152underruns.
In-window1minload7.1929/7.6494; functional copy smoke overlaps warmup and other
host work remains. Diagnostic only, no isolated causal improvement or M1 claim.
Native same-frame throughput20.3891/s; bimodal intervals remain.
Heap1GiB. Raw submitted276653draws; fallback/invalid/guard counts zero.
540shader logs pass, unsupported states empty. These draw totals cover the whole run.
One result: build/runtime_measurements/20261004T145822796727_b709642e/analysis.json.
Normal Stop exported final screens/PCM/input. Browser inventories empty, server
absence observed, measurement reservation released.

## Working scene entry

R4 normal map Save and Quit produced ordinary GameData f260bde9 and entire20file
user tree, with no save byte edits. Two fresh plain-Chrome boots reached visible
World1-1 in108.4721s and98.9805s from server admission.
Reference: /Users/exgota/.t3/worktrees/super-mario-3d-land-browser/root-runtime-startup-audio/build/runtime_startup_audio/map_saved_reference
Recipe: /Users/exgota/.t3/worktrees/super-mario-3d-land-browser/root-runtime-startup-audio/build/runtime_startup_audio/saved_map_world_one_navigation_recipe.json
Requires shared tool's entry_node_anchor support. Wait for W1-1 node before A.
Original movie preserved. Both natural Stop/browser/server cleanup passed.
Changed initial-save conditions are explicit. No startup-cache causality claimed.

## Paused at owner request, 2026-10-04

Root has no active browser, server, build or measurement runner. Primary reservation
is absent. R1–R4/Web/orchestrator received the explicit restart pause.
Source fd6aad35 adds bounded native display-transfer diagnostics and preserves failed
runtime logs. Revision4 build passed3.5211s and has not been launched.
Revision3 functional job20261004T153534080114_5706fa8a reached intact World in72.3911s,
but still published RGBA frames and aborted at Natural Stop. Browser cleanup passed,
final inventories empty, owned server absent. No new speed or bitmap-success claim.
Next work is recorded in project/runtime_lanes.md, Resume after T3 restart.
R1 retention94c28cb2 and R4 bitmap paint527cad1c are integrated. R2 scheduler b33cea6a
archive is ready but not integrated. No source migration/factory submission/deployment.

GPU presentation supports only complete unscaled tiled-to-linear transfers and
explicit pixel formats. Unsupported requests take the coherent software path.
Guest reads/writes materialize aliases through cached-page callbacks. Do not remove
those barriers to inflate speed. Normal frame readback removal is not proved yet.
Keep software reference. Routine360 and byte-exact captures are parked; run a
bounded comparison only if a change actually breaks rendering.

## Other retained evidence and limits

Previous raw GPU World job20261004T131750353522_1591068a measured19.03595/s,
median54.9925ms/p99111.395ms under load10.58–11.90. Geometry present.
R1 real Chrome draw-pthread smoke20261004T141731400030_01de787a passed2671draws,
zero fallback, shared heap/OffscreenCanvas admission and joined Stop.
R3 real Chrome presentation/copy smoke42bitmaps/6shader logs passed; no game/FPS.
Last21software pixels stay parked. Accepted software reference and sealed original
movie evidence remain. Whole-level goal and Azahar typed-state comparison unproved.
Initial heap1GiB/max2GiB/pool18. No memory/mobile acceptance.

Browser registry stale pointers from deleted, previously cleaned Root failed runs
were retired under its registry lock. Source322b0843 handles confirmed Darwin
process-exit races while retaining unknown identity failures. No unrelated signals.
Source-only private playable migration waits for a verified M1 boundary.
Estimate remains LOW confidence:6–12h first measured60Chrome,10–20h humanM1.
The20.40/s combined result leaves substantial GPU/CPU overhead; no subtractive gain
is inferred. Next checkpoint is actual bitmap-path World cost and image validation.
