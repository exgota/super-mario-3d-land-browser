# Browser World 1-1 delivery plan

The owner now targets a shareable playable World1-1 build for personal use and
friends. On October4 the owner says "lets get it good enough to be on the vercel
deployment, playable on native browser, mac mini, macbook neo, iphone if they so
please". The advisor prioritizes Mini Chrome/Safari, then the MacBook, then iPhone.
The former Wednesday date is removed. Root continues the existing runtime
worktree; Web owns the site and deployment. No upload is authorized by this plan.
The active GPU plan is `browser_webgl_renderer_plan.md`. Earlier estimates below
are historical lane estimates, not delivery promises. Updated October4,2026.

## Owner-approved renderer decision, October 4, 2026

The owner chooses WebGL2 on the advisor's recommendation, replacing the earlier
WebGPU proposal. Approval relayed verbatim: "ok sounds great! go for it". The
decision rationale is the Mini's working Metal/ANGLE path, native ETC support,
desktop browser portability and avoiding additional renderer migration time.
Actual WebGL2/ETC capability is verified in T3 on this Mini; support in every
desktop browser and the time saved are decision rationale, not measured coverage.
WebGPU may follow only if measured speed requires it. Device and hosting scope
is updated above; Root does not own the deployment. The initial GPU short360 result is9.18597warm presentations/second;
the recorded original cadence is59.83122/second. Visual and speed acceptance
remain pending. Software rendering stays unchanged as the exactness reference.

GPU play acceptance combines the existing whole-level requirement with one
original-input-movie World1-1 start-to-goal GPU browser run and Azahar state
comparison. In that same run, retain frame-time percentiles, Wasm heap growth
and pauses, audio underruns, and input-to-display latency. Keep short360 as the
regression gate. This replaces separate long exactness replays. A visibly reached
goal, grounded state comparison and measured runtime evidence are all required;
a completed presentation count or skipped draw does not earn success.

## Active milestone order and acceptance

The orchestrator's M1 gate is a human playing World1-1 start-to-goal with sound
in plain Chrome and Safari on the Mini: median displayed interval<=16.9ms,
<1%intervals>33ms after the first10seconds, zero post-load audio underruns,
<20seconds to controllable Mario, retained peak heap and short local recordings.
Replay, machine and shader measurements prepare this gate; they are not human
play evidence. Short360 remains the regression gate and software stays unchanged.
The combined original-movie/Azahar state comparison above remains a preparation
requirement; its9000prefix has not yet reached the goal.

M2 follows on `ssh peer`, with measured memory and evenly paced declared30fps
allowed if60fps does not fit. iPhone follows the first share and higher-priority
work. URL loading, RomFS overlay and save persistence are a deferred Web contract;
only M1-required continuous execution, held input and audio are on Root's current
critical path. Web reconstructs and verifies the full original File, so no trimmed
image runtime hook is needed. No deploy-ready runtime artifact exists yet.

Factory wind-down remains outside Root's scope. Do not signal, restart, modify
or submit to factory while this work is separate. Every new measurement retains
timestamped machine load and read-only factory process observations. Factory-
contention diagnostics identify costs but cannot certify M1 or headline fps.
Acceptance waits for observed absence of `factory.py`. Earlier runs lack such
absence evidence and remain diagnostics; missing geometry is a rendering failure.

The new private playable repository holds source only. Root has read its README
and AGENTS. Do not copy runtime source for migration during M1. After M1, identify
the exact selected source-only commit/file boundary before migration. Built Wasm,
schedule, game-derived translations, game files and local capture payloads stay
outside commits. Only the orchestrator merges the new main. Owner confirmation
of the exact upload manifest remains required before deployment.

## Current rendering boundary

The verified browser module executes the statically recompiled ARM CPU, console
services and software PICA renderer inside WebAssembly. A browser worker reads
its actual generated RGBA, and the page displays those pixels with Canvas 2D.
The local server serves runtime files and comparison sidecars and receives
exported evidence. It does not render game pixels or serve the owner's dump.
Azahar runs separately as the reference. The independent WebGL2 prototype now
executes the short360 movie and produces its own browser pixels. The corrected
short run has low visual error and approaches60Hz only in the steady menu window.
The original-movie7800prefix reaches World1-1 but fails visually, with a blue
top screen, missing level geometry and9.3694presentations/second in7680..7800.
Top RGB mean absolute error140.2144/255;97.91%pixels exceed8channel error.
All402shader compile/link records pass, demonstrating that shader validation
alone does not establish correct draw semantics. The GPU play path is unaccepted.
World cost attribution, coherence and synchronized audio remain unresolved.
WebGPU rendering is not implemented.
The retired compiler closure has now been reconstructed with historical byte
identity. A fresh module exists. Actual default360 replay and normal360 replay
passed original input/PCM/pixels/framebuffers; default also passed the full GPU
stream comparison. All12 standard buttons and natural Stop passed actual browser
HID/movie delivery. The capped9000presentation browser World1-1 replay completed
at the bridge crates. The resumed audio/fog candidate now matches all9884160PCM
channel samples, input/timing/CPU counts and the bottom screen. Combined
depth/audio/fog corrects one former pixel;21remain, no new pixels. The private
quaternion-normalization replay leaves the same21pixels unchanged. A native-order
lighting rotation/reduction replay leaves the same21pixels unchanged. Broader
native-order table/distance arithmetic matches1048576synthetic lighting cases
and completed the original movie in the owner-started T3 thread. It retains the
same21pixels, all other comparisons pass. The owner has parked this residual gap
and authorized ready-work submission. Current order: browser_performance_plan.md.
The measured normal startup presentation rate is about8.7/sec after first frame;
the resumed audio/fog9000prefix averages4.3916/sec across8999warm intervals;
the depth candidate averages4.1685/sec on a shared10core host with load14.14,
well below playable speed; separate execution-cost measurements remain pending.

The accepted finite browser execution result remains available at
`build/root_browser_execution/build/browser_submission_recovery/result.json`,
SHA-256 `8193373632743cae4162ecb40a120f0d05eec7947202d35196d832a7c49b0b1a`.
This is a finite title-screen execution and displayed-pixel equality result.
Subsequent accepted families add streamed frames, live A/circle-pad/touch input
and original PCM consumption. They do not establish sustained playable speed,
World 1-1 completion, or synchronized continuous sound.

## Earlier ordered estimates

| Order | Work | Estimate | Exit evidence |
| --- | --- | --- | --- |
| 1 | Finish the surviving native bridge replay and submit its exact comparison and preservation evidence | 20–60 minutes | Completed own-movie replay, unchanged strict window/input/audio/pixel comparison, post-run preservation and integrator submission |
| 2 | Recover the retired browser link inputs, then extend the runtime to full-level sessions and required controls, with bounded recording and ordinary play free of per-command capture overhead; measure execution costs | 6–12 hours | Real browser session reaches gameplay, controls enter normal HID, finite recording/stop limits hold, CPU/render/frame-delivery measurements |
| 3 | Implement browser WebGPU PICA rendering and address measured performance bottlenecks | 24–48 hours, highest uncertainty | Actual WebGPU draws in the browser; selected original command streams and resulting pixels compare against Azahar; sustained performance is measured |
| 4 | Record and replay a complete natural World 1-1 start-to-goal route | 8–16 hours | Original input movie and initial state; goal reached visibly in original and port; first divergences resolved |
| 5 | Complete Section 7 player, camera, RNG, timer and coin comparisons using verified Pro proposals | 12–24 hours, answer-dependent | Grounded player/update selection, same-input fixed-update observations and passing whole-level regression suite |
| 6 | Verify whole-level browser play, live controls and sound through the goal, then deliver the runnable browser handoff | 6–12 hours | Actual browser reaches goal, rerunnable suite and preserved inputs, explicit runtime/source split and documented local setup |

These earlier estimates are retained from the main-side plan. The October 3
owner priority in `project/browser_performance_plan.md` supersedes their order.
The earlier total estimate was approximately 57–113 hours. WebGPU coverage and browser CPU
speed are the largest schedule risks. A measured failure or missing evidence
changes the estimate; it does not earn a milestone or a substitute success.

## Renderer implementation plan

Keep the ARM execution and console-service layers in WebAssembly. Translate
PICA200 command state, shader execution and rasterization into a browser WebGL2
service, outside the decompiled game files. Use Azahar's existing software
renderer as the native oracle. Measure CPU, rasterization, readback and frame
copy costs before choosing optimizations. The browser software renderer supplies
a current correctness baseline, not a proven real-time performance baseline.

Design references supply ideas only. No GPL renderer implementation is copied.
The pinned [mw2-recompiled rendering design at5855c754](https://github.com/paulcombal/mw2-recompiled/blob/5855c754f2515f8ca0033a2ce2b0897c3fbaa7e2/docs/rendering.md)
informs draw-time guest-state resolution, complete pipeline keys, memory-write
invalidation and cache warming. PICA behavior must be measured independently;
Xenos-specific assumptions do not establish PICA behavior. Async creation alone
does not remove first-use stutter. A diagnostic WebGL2 build must retain every
generated shader's compile/link logs and count unsupported states by reason.
Skipped draws cannot count as successful rendering. Cache warming must be
measured at actual first use, including driver work after program linking.

Address-based source replacement remains the port architecture. The matching
ARMCC build stays authoritative. The current browser module does not establish
complete adapter coverage for every rank-O function now on main. Report the
actual decompiled/recompiled split and hold the pure source claim until the
recompiled share reaches zero. No game data or gameplay screenshots enter git.

## Active work and limits

The bridge replay completed and root/world-one-bridge-continuation e431761ab
was accepted at de8c5810. Exact inclusive8400..9000 window/input/audio/screens,
zero CPU fallbacks and post-preservation evidence remain sealed. Its finite
limits were2400 provider seconds,2460 controller seconds,512MiB PICA,1GiB
monitored output,200000files,256MiB per file and15GiB free floor. No goal credit.

The browser closure is now recovered:23archives, the Node main object, full
Node wasm and full browser wasm match historical hashes. Recovery remains
separate from new browser runtime verification. The active family is
root/browser-webgl-renderer. Selected limit <=60000 presentations, provider
wall <=3600 seconds and audio <=256MiB. Ordinary mode exports no GPU/PICA files.
Its session completion earns no complete replay or goal claim.

Root now owns C++/build and actual browser verification in its owner-started T3
thread. No subagents, delegated tasks or Codex relay are used. The passive minimum-tick observer
family remains parked until grounded state comparison needs it. No new reviewer
or unrelated harness work is started.
Pro receives layout and typed update questions. Only the integrator moves main,
ranks and ledger. Finished port branches carry no matching claims.
