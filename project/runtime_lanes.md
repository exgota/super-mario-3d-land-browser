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
World effect is not yet verified. Four existing software/GPU visual-output gaps
remain. Receipt: `build/browser_performance_resume/command_fill_short_gate_receipt.json`.

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

Root currently holds the measurement reservation. The bounded live visual run
ended at a story prompt before World entry when coordination took priority. It
supplies no World clear validation or new World baseline.
Root will collect one common unchanged live baseline for R1–R4 before their first
edits. This consolidates the four identical before-runs. Lanes verify module/input
seals against their clone and cite that receipt. Each candidate then gets its own
reserved before/after comparison against that common baseline. No competing boot.

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

Last visual URL: `http://127.0.0.1:8804/`, Root-owned T3 Chromium tab7, record
60000presentations/600second bound, default audio buffering zero. This is diagnostic
and is not plain Chrome/Safari human M1. Other threads do not control or start
another session at this URL. Root distributes live observations. After this check,
the shared fresh baseline uses plain Chrome on the same selected module and source
seals, same original initial tree, live held controls and active original audio.

Launch recipe uses the selected module, local server above, schedule
`build/root_native_block_scheduling/build/block_scheduled_native/block_schedule.bin`
in the primary checkout, and reference `build/browser_session_preparation/server_reference_9000`
in Root worktree. The movie supplies the initial clock only in record mode.
Full local dump size536870912, SHA256c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976.
It is mounted as the owner's ordinary local File, never uploaded to the server.
Use the server/resource command recorded in
`build/browser_performance_resume/command_fill_live_world_resources.json` as the
exact launch argument inventory. Choose a new owned output/port for each run.

Latest completed cost-timers-off World diagnostic baseline: display12.946839/s,
median57.392500ms, p99152.515ms; native12.912699/s, median75.257568ms. These are
shared-load diagnostic measurements. Instrumented8.649/s is not a clean baseline.
Analysis: `build/browser_performance_resume/live_world_baseline_analysis_revision_2.json`.

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

Estimate pending lane recon: the first critical checkpoint is R1 barrier feasibility,
R2 optimization/CPU budget and R3 raw vertex-program coverage, reported within the
first90minutes of this contract. No measured parallelism gain exists yet. Root will
revise the hours-to60 estimate from those results, rather than divide the previous
estimate by four or promise subtractive gains from instrumented costs.
