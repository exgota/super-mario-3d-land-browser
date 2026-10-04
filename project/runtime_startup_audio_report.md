# Runtime startup, audio and correctness, R4

## Source and artifact boundary

Branch: `root/runtime-startup-audio`, based on integrated `d36ce04b`.
The original Audio commit `c46e0e8` is not an ancestor. Its two audio modules
and report are byte-identical to the separately integrated content. No further
audio buffering change is part of this candidate.

The lane first cloned Root's actual `build/` with `cp -cR`, then verified all
43 selected manifest/dependency inputs against that clone. Game data, compilers
and the virtual environment use local symlinks. No game input or generated
runtime artifact enters this commit. The selected source includes Root's private
command-defined clear, not the older public prototype.

Selected module SHA256:
`34b63c89ea3b140653fd4d860e1dbc66561cb6e5928dedaa053b617452d3e92a`.
Selected Wasm SHA256:
`92df445a692a30e4e24ffca7c7fba573a3253a21e9e0db90e4a4e37747acd22f`.
Both remain unchanged. The worker preserves the selected private 8 ms preview
poll. Its public-source diff includes the older public 25 ms poll replacement;
the separate selected-private-worker diff contains only this lane's changes.

Ignored evidence directory: `build/runtime_startup_audio/`.
`selected_input_seals.json` preserves the initial exact dependency inventory.
`candidate_source_seals.json` records the four changed modules, the selected
worker diff and the local server recipe. All 12 other served browser assets
equal the sealed common baseline. The local server serves these owned source
modules beside the unchanged selected module and ordinary owner-selected File.

## Common live before-run

Root supplied the single common before-run before these source edits. It used
native plain Chrome Guest, loopback HTTP, original initial files and stereo audio
at 32,728 Hz, startup buffer zero, stationary Mario in actual World 1-1.
DevTools was closed during the timed observation. No runtime-lane build overlapped.

Receipt:
`/Users/exgota/super-mario-3d-land-browser/build/root_browser_gameplay_session/build/browser_performance_resume/runtime_lanes_chrome_baseline_receipt.json`.
SHA256: `8c8626446661973aa2582ea24c136027fb3a1e1533c2b8e5ce68925fd7c73383`.

| Measurement | Common before |
| --- | ---: |
| Display intervals after 10 s warmup, next 30 s | 384 |
| Display updates per second | 12.83759275 |
| Conventional display median | 87.1475 ms |
| Display p99 | 159.06 ms |
| Display intervals over 33 ms | 74.479% |
| Same-frame native median / p99 | 75.2425537 / 186.8901367 ms |
| First eligible frame from native runtime admission | 27.544389888 s |
| Peak Wasm heap / growth | 1 GiB / zero |
| Source audio frames in full 40 s | 247,808 |
| Source audio frames per second, 40 s nominal scope | 6,195.2 |
| Audio underrun frames in full 40 s | 1,060,096 |

Actual one-minute host load samples in that window were 7.8677, 7.4468 and
7.4263. This is diagnostic evidence under shared host contention, not M1.
The 40 s audio delta includes the 10 s display warmup. It does not belong to
the 30 s display interval window.

The previous 43.290455 s first-frame observation and about 52 s description
are historical, not the fresh common baseline. The earlier 2,084,864 underrun
delta spans approximately 81.17 s. Independently recomputed production was
7,013.931 frames/s, not a 30 s measurement.

## Candidate behavior

`BrowserWorkerFileCache.mjs` overrides only reads on distinct immutable WORKERFS
input nodes in gameplay mode. It reads aligned 1 MiB pages into a global 16 MiB
least-recently-used cache. Reads preserve exact ranges, cross-page boundaries,
zero-length results and EOF. Existing seek, metadata and read-only write refusal
operations are retained. Capture output and the append-only audio file are
excluded. Disposal restores each node's original operations.

The existing native full-input SHA256 validation is still required before
`callMain`. Caching does not bypass identity, preload the complete dump, change
guest ticks, select another CPU provider or alter the software renderer.

`BrowserRuntimeInitialization.mjs` exports an operation with `ready`, `cancel`,
stage checkpoints and observations. Cancellation rejects admission and guards
pre-run callbacks. Import/compile internals are not claimed to be interruptible;
the page retains ownership of abnormal worker and pthread disposal.

The worker records module import, runtime initialization, identity, admission
and first copied preview checkpoints. Cache counters are snapshotted at admission,
first preview and completion, without a clock read on each file read or frame.
The page exposes `runtimeInitializationObservation()` and preserves the existing
input/audio lifecycle. No visible interface or control layout changes.

## Verification and current limits

Node syntax checks and `git diff --check` pass. The Impeccable manual detector
reports no findings on the changed page. A transient Node byte oracle verifies
50 offset/length boundaries against independent input bytes, including page
crossings, EOF and zero-length reads. A 33 MiB fixture exercises eviction and
observes a peak of exactly 16 MiB. Original operations restore on disposal.
Cancellation prevents pre-run admission; normal initialization succeeds.
These checks are source contracts, not browser or gameplay evidence.

The first oracle fixture was smaller than the 16 MiB capacity, so its eviction
assertion failed. That failed receipt is retained separately as
`source_verification_failure_001.json`; the production implementation did not
change to make the check pass. `source_verification.json` holds the corrected
fixture result.

Candidate live after-run is pending R2's first measurement and helper handback.
R4 will reserve through Root's shared `measure_live_world_one.py`, use the
unchanged 10 s warmup / 30 s collector, retain source/artifact/input/resource
seals, and release only after its own helpers stop. No performance improvement
or human M1 is accepted by the source checks.

The retired Audio lane's initial-only threshold remains default zero with a
63,489-frame cap. Its synthetic PCM and actual Chrome samples were exact;
Chrome default and buffered uninterrupted cases both had zero underruns, so
there is no observed Chrome buffering gain. Real World source production is
far below 32,728 frames/s. A finite startup buffer cannot repair that sustained
deficit. Starvation remains reported; no stretching, repeating or dropping PCM.
Safari is untested under its unchanged automation admission.

Root's supplied final live World frame visibly has ground, Mario, trees, blocks,
castle/fence and HUD. Root's command-defined fill restored this geometry. No
second visual change is supported by that observation, and R4 does not claim
pixel equivalence or full M1 from it.
