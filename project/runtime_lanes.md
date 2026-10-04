# Runtime speed lanes

Issued by Root on 2026-10-04. This is Root's implementation contract under the
owner's 60 fps direction. It does not change M1 or authorize deployment, migration,
factory work, main changes, game-data commits or additional agents.

## Frozen starting point and actual live artifact

All four lanes start at `d36ce04bc054de600aa547461e5c4d5bfbb23a9c` on their own
`root/runtime-*` branches. Root integrates wins into `root/browser-webgl-renderer`.
This commit includes reviewed Audio c46e0e8, default startup buffering zero.
The committed renderer prototype alone is not the selected live artifact.

Root worktree: `/Users/exgota/super-mario-3d-land-browser/build/root_browser_gameplay_session`.
APFS clone source is its `build/` directory. Use `cp -cR`, or read-only symlinks for
sealed input files. Existing absolute input paths in recipes refer to immutable
Root inputs. Rewrite candidate output/source paths to the lane worktree. Never
write through a shared-input symlink or mutate a sealed artifact.

Selected live candidate: `build/browser_command_fill_module_candidate`.
Its `build_manifest.json` is the complete link recipe, module/Wasm identity and
dependency inventory. It derives from `build/browser_live_world_baseline_module`,
with all cost-profile enable/save calls disabled and no draw capture linked.
Complete-target fills derive RGBA8/D24S8 state from the actual guest32bit command,
not a forced depth constant. Its canonical360 gate preserves720GPU images and
all7final GPU-baseline outputs, original HID/PCM/timing and CPU instruction counts.
The common plain-Chrome World run visibly restores ground, Mario, trees, blocks,
castle and HUD. Four existing software/GPU visual-output gaps remain. Receipt: `build/browser_performance_resume/command_fill_short_gate_receipt.json`.

Current uncommitted clear inputs, relative to Root worktree:

| Input | Purpose |
| --- | --- |
| `build/browser_performance_resume/command_fill_webgl_bridge.cpp` | Selected authored bridge, cached state and command-defined fill |
| `build/browser_performance_resume/command_fill_webgl_bridge.h` | Selected bridge declarations |
| `build/browser_performance_resume/command_fill_gpu.cpp` | Local existing-platform adaptation, not a new committed renderer |
| `build/browser_performance_resume/build_browser_command_fill_candidate.py` | Exact four-command candidate recipe |
| `build/browser_performance_resume/stencil_permission_webgl_renderer_draft.mjs` | Actual selected JavaScript renderer |
| `build/browser_performance_resume/BrowserCaptureWorkerFastFrames.mjs` | Actual selected8ms frame delivery worker |
| `build/browser_performance_resume/serve_browser_execution_fast_frames.py` | Existing local diagnostic server |

Additional shader/cache/decoder dependencies are named in the manifest link command.
Read and retain their hashes. Do not silently use the older committed renderer.
No runtime migration is implied by this working boundary. It is not a deploy-ready
directory or the final source-only migration boundary.

## Exclusive file ownership

Each lane may add its named files and edit its listed existing files. Shared-file
changes are proposals to Root, with an exact diff against the frozen selected
input. Root applies and validates them. Do not edit another lane's files.

| Lane | Exclusive writable paths | Immediate implementation target |
| --- | --- | --- |
| R1, `root/runtime-pipelining` | `runtime/port/browser/BrowserGpuCommandQueue.cpp`, `.h`; `BrowserGpuWorker.mjs`; `BrowserRuntimeWorkerTransport.mjs`; `BrowserGpuScheduling.cpp`, `.h`; `BrowserGpuPipeline.cpp`, `.h`; `project/runtime_pipelining_report.md` | Bounded immutable draw/state queue, worker handoff and GSP completion/barrier model. Queue overlap first, with an opt-in admission boundary. |
| R2, `root/runtime-translated-code` | `runtime/port/StaticArmBackend.cpp`, `.h`; `NativeBlockSchedule.cpp`, `.h`; `NativeExecution.cpp`; `NativeTiming.c`, `.h`; `TranslatedFunctionModule.cpp`, `.h`; `runtime/port/webassembly_translation/CMakeLists.txt`; `tools/static_recompiler/src/main.rs`; `tools/static_recompiler/build_runtime_cpu_candidate.py`; `project/runtime_translated_code_report.md` | Remove disabled timing overhead from play, improve CPU compile/dispatch/memory helpers. Generated translations remain ignored/local. |
| R3, `root/runtime-gpu-vertex` | `runtime/port/browser/PicaWebGlVertexShader.mjs`; `BrowserWebGlVertexSubmission.cpp`, `.h`; `PicaWebGlDrawCache.mjs`; `project/runtime_gpu_vertex_report.md` | Raw PICA program/input execution in WebGL2, bypass CPU vertex shader and repeated output conversion. Root wires shared renderer and command processor. |
| R4, `root/runtime-startup-audio` | `runtime/port/browser/BrowserRuntimeInitialization.mjs`; `BrowserWorkerFileCache.mjs`; `BrowserSurfaceCoherence.cpp`, `.h`; `BrowserInputIdentity.cpp`; `BrowserCaptureWorker.mjs`; `BrowserCapturePage.mjs`; `BrowserStreamedAudio.mjs`; `BrowserAudioWorklet.mjs`; `BrowserAudioFileStream.mjs`; `project/runtime_startup_audio_report.md` | Startup loading/compile/pool admission, continuous original audio and live visual coherence. Existing default-off Audio candidate is integrated; no duplicate implementation. |
| Root | All other runtime paths; especially `BrowserWebGlBridge.cpp`, `.h`, `BrowserWebGlRenderer.mjs`, `PicaWebGlShaders.mjs`, `PicaWebGlShaderSpecialization.mjs`, `BrowserExecutionEntry.cpp`, `BrowserFrameOutput.cpp`, `.h`, `GameplaySession.cpp`, `.h`; shared build/server and measurement tools; `project/STATE.md`, this contract | Wire shared interfaces, finish live clear check, integrate and measure wins, preserve software reference and short regression gate. |

Lane-local ignored build recipes and outputs are writable in the lane worktree.
Existing-platform source overlays remain local; submit authored source or an exact
patch input with provenance, not a copied GPL renderer implementation. No new
long capture harness, fragment observer or full exactness replay.

## Shared interfaces

Existing page/worker input and audio packet/lifecycle contracts remain stable.
R4 owns `BrowserCaptureWorker.mjs`; Root proposes R1 hook wiring to R4 and integrates it. Continuous
controls remain held HID12bit mask, circle pad radius154 and touch320x240.
Original PCM is stereo s16 at32728Hz. Preserve packets, starvation counters,
drain/cancel/Stop boundaries and source identity. A buffer cannot repair a
sustained production deficit. No hidden frame skip or audio stretching earns60fps.

R1 designs a versioned queue record with sequence, command kind, immutable byte
extent and completion sequence. Snapshot mutable guest command/state/vertex/texture
data before publication. Acquire/release atomics publish ownership. GPU readback,
CPU reads of GPU-written ranges, fills/transfers, cache invalidation and GSP
interrupt delivery are explicit ordered barriers. Do not signal completion before
the required work/coherent memory is available. Root wires existing GPU::Execute
and GSP platform adapters after R1 supplies the exact hook/barrier proposal.
Keep queue bytes and in-flight frame count bounded and report input latency.

R2 preserves the existing `Context`/`Host`/translated-entry ABI, guest ticks,
instruction charging, SVC/budget exits and memory-observer behavior. CPU provider
selection remains static translated execution. Do not switch CPU strategy. Compile
flags apply to identified CPU inputs. Preserve numerical behavior; no global
fast-math claim. Root handles the final link/build recipe and disabled-profile
instrumentation in shared platform overlays.

R3 supplies `createPicaVertexProgram(descriptor)` returning generated vertex GLSL,
attribute/uniform declarations, output mapping and explicit unsupported-state
diagnostics. Descriptor schema1 contains program words, entry point, swizzle words,
input-register mapping, output mapping, float/int/bool uniforms and enabled input
attributes. Use documented PICA register/ISA interfaces as design references, no
copied GPL renderer implementation. Cache keys include every shader-affecting word.
Vertex submission describes primitive type, vertex count, optional index buffer,
attribute formats/strides/offsets, immutable input bytes and the program descriptor.
Root provides a `TryDrawBrowserWebGlVertexProgram` hook before CPU shader execution;
true means every requested primitive was submitted, false means explicit supported
fallback, never a skipped draw. Existing output-vertex triangle path stays the
software/reference fallback. Respect clipping, quaternion sign, PICA float24 and
perspective interpolation requirements; validate live images and shader logs.

Root's bounded contiguous output-triangle candidate is already prepared in
`build/browser_performance_resume/batched_triangle_webgl_bridge.cpp` and
`batched_triangle_webgl_renderer.mjs`, not yet measured or selected. R3 may inspect
it as a first small reduction; Root will wire it. It preserves22floats/vertex and
65535vertices/batch. Do not independently redo this implementation.

R4 owns further audio changes from d36ce04b. Root's active command-defined fill
visual check is not to be repeated. R4 may observe Root's supplied current images
and receipts read-only, then investigate remaining coherence/lighting defects and
submit a shared-bridge patch proposal. Startup helper exports a cancellable
initialization operation; Root owns its worker call site. Keep URL boot, RomFS
overlay and persistence hooks deferred. Ordinary Chrome/Safari audio admission
uses a real gesture, no automation-policy or browser-security setting changes.

The old Audio thread `mcp:83f08952-217f-48a1-aefc-168a91d5aa7d` is completed and
retired from continuing runtime work. Its final source commit is
`c46e0e8050b2443ad9e2123d2099db269a373203`, integrated asd36ce04b. No new ownership
or restart is assigned to that thread. R4 is the sole continuing writer of all
listed audio files. Retain `project/root_audio_pacing_report.md` and its exact
handoff at `/Users/exgota/.t3/worktrees/super-mario-3d-land-browser/root-browser-audio-pacing/build/audio_pacing/final_handoff.json`.
Cap63489, default0, explicit starvation, Chrome synthetic no measured buffering
gain and Safari untested remain its limits. Use loopback HTTP for current work.
No localhost certificate creation or trust change is requested.

## One measurement and build arbiter

Root-owned shared script:
`tools/static_recompiler/measure_live_world_one.py`, always invoked by its absolute
Root-worktree path. Shared state is outside lane clones at
`/Users/exgota/super-mario-3d-land-browser/build/runtime_coordination`.

`reserve --lane R1 --kind measurement` returns an ownership token. Any existing
reservation refuses admission. `reserve --lane R2 --kind build` uses the same
exclusive reservation, so no runtime-lane build overlaps a measurement. Build
commands run nice5 with a fully detached supervisor. Keep one heavy build active
at a time across these lanes initially. Source inspection and editing need no lock.
`sample --token TOKEN` records timestamped load, exact Python/script factory
classification and sanitized process CPU/RSS/nice. Sample before/after and every
15seconds of the live window. `release --token TOKEN` records the final sample and
releases only that token. Release after owned browser/build helpers have stopped.
No stale-lock stealing or signalling another lane. Send Root a stuck reservation.
Free disk below12GB blocks build admission and is a stop/report condition.

The common plain-Chrome baseline completed and Root released token
`613b6c6384f6451a964970d94690be7f` after its owned Guest window and server closed.
Source edits and reserved candidate builds are released for R1–R4. R2's first
one-object CPU candidate has initial build/measurement priority, then R4's
JavaScript file-cache candidate. No lane repeats the identical common before-run.
Verify the clone's module/input seals and cite the shared receipt. Reserve every
candidate measurement and retain its own source/load/cleanup evidence.

`browser-script` prints the browser-evaluate expression for the shared collector.
It observes displayed renderer-frame changes at8ms, warms10seconds and records
the next30seconds. Visually confirm World1-1 before installing it. Save
`JSON.stringify(window.liveWorldMeasurement)` when complete. `analyze --observation
PATH --output PATH` computes conventional two-middle median, nearest-rank p95/p99,
fps and the fraction over33ms. Audio delta is the full40second observation and is
labeled separately. Record candidate source/module hashes, browser/version, input
conditions, World identity, heap and start/end resources beside the report. Native
presentation telemetry remains a separate corroboration. Do not call menu fps,
instrumented timing, replay-only evidence or a synthetic fixture human acceptance.

Common baseline used `http://127.0.0.1:8805/` in native plain Chrome Guest mode.
That owned window/server is now closed. The earlier T3 Chromium8804 story run
stopped before World and supplies no World validation. Choose a new owned output
and port for each candidate. Do not reuse another lane's live tab.

The exact baseline launch command is retained in
`build/browser_performance_resume/runtime_lanes_chrome_baseline_resources.json`.
It uses the selected module, FastFrames server, original block_schedule.bin from
primary build/root_native_block_scheduling/build/block_scheduled_native, and
Root's build/browser_session_preparation/server_reference_9000. Record mode uses
the movie only for the initial clock. Presentation bound60000, wall bound900seconds,
stream audio/frame output enabled. Full local dump size536870912, SHA256
c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976.
It stays an ordinary local File, never uploaded to the server.

Baseline navigation uses held KeyZ/A300ms every2seconds for title/story prompts,
stopped at the World map. Hold ArrowRight800ms, wait until Mario settles on the
World1-1 red node, then hold KeyZ800ms to enter. An A during map motion was ignored.
Dispatch through the ordinary canvas/page input handlers, release all input and
measure stationary Mario. Sound starts/resumes through a real page gesture.
Confirm the actual World image before the shared10second warmup/30second window;
close DevTools during that window. The collector itself remains unchanged.

Common before:384display intervals,12.8375927508updates/s, conventional median
87.1475ms, p99159.06ms,74.479%over33ms. Native same-frame384intervals separately
measure12.8262424419/s, median75.2425537ms,p99186.8901367ms. The full40second audio
observation adds247808source frames (6195.2/s) and1060096underrun frames.
Peak heap1GiB, zero growth; first eligible27.5444seconds from runtime admission,
excluding earlier page/download/navigation.402shader logs pass, unsupported0.
World host load samples7.8677/7.4468/7.4263 (one-minute). Diagnostic only, not M1.
This is the common R1–R4 before-run, not a causal comparison with older T3 timing.
Receipt: build/browser_performance_resume/runtime_lanes_chrome_baseline_receipt.json,
SHA2568c8626446661973aa2582ea24c136027fb3a1e1533c2b8e5ce68925fd7c73383.
Shared script SHA25687120b74aa9624cddca830e16750b4968ecaa4d13a05b4bb55cc71601c3c5e08.
Instrumented8.649/s and menu readings remain excluded from the live baseline.

Root's R3 hook candidate extracts PrepareBrowserWebGlDraw(memory,pica) without
OutputVertex conversion. Separate selected-source revisions are
build/browser_performance_resume/vertex_submission_webgl_bridge.cpp/.h and
vertex_submission_webgl_renderer.mjs. They are not built or measured yet.
Root-owned renderer.tryDrawVertexBatch(descriptor) uses R3's
createPicaWebGlDrawCache(gl,{textureUnitBase:8,uniformBlockBinding:1,...}).
cache.prepare(descriptor, specializedFragmentSource) returns supported/program/
uniforms/vertexCount/resources. Root reapplies complete fragment configuration
against that program, then cache.draw(prepared). Only a submitted draw marks dirty
and adds draw/triangle statistics. Explicit false restores CPU-fallback state and
counts the reason; generated compile/link records enter the exported diagnostics.
Root's native hook allows GS-disabled List0/Shader3, empty assembler, complete
triangle counts and no pending inverted winding. It preserves delay charging and
closes the draw before an accelerated return. R3 owns raw vertex/resource code.

## Integration and acceptance

Send Root and orchestrator a SPEED REPORT with exact commit, source/artifact hashes,
live median/p99/fps before/after, load/resource receipt, changed behavior, next step
and specific blocker. A source-only or synthetic report labels unmeasured gameplay.
Root reviews and integrates exact lane commits. No lane merges Root/main or submits
to factory. Keep the unchanged software renderer and existing360regression gate.
Generated shaders need every compile/link log and explicit unsupported-state counts.

M1 stays human World1-1 start to goal with sound in plain Chrome AND Safari on
the Mini: conventional median<=16.9ms, fewer than1%over33ms after10seconds, zero
post-load audio underruns, controllable Mario<20seconds, peak heap and short local
recordings. First measured60fps World is a development checkpoint, not full M1.
Then peer. No30fps first share. Migration/deployment remain deferred.

Working estimate after initial lane recon:6–12hours to first measured60fps live
World1-1 in Chrome,10–20hours total to human Chrome/Safari start-to-goal M1 with
sound. Confidence is low. This includes usable World rendering, continuous input
and original audio; the first measured60checkpoint is not full M1. Critical path
is R3's seven observed vertex-program/control-flow families plus Root hook wiring,
R2's actual CPU budget and R1's coherent barriers. Assume R2 reaches a CPU budget
with enough margin and R3 removes per-vertex CPU work without driver/compiler
blockers. Neither assumption is measured yet. Source-only recon has established
specific changes, not performance gains. Re-estimate at the first candidate live
measurement, within90minutes of this contract for the initial feasibility report.
Do not divide the old estimate by four or subtract instrumented category totals.
