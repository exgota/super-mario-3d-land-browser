# Runtime translated-code speed report

R2, 2026-10-04. First page-table slice is frozen for Root integration review.
The old-ABI candidate linked and ran World 1-1 in plain Chrome. The canonical
360 guest/HID/PCM/counter gate is pending. Preservation, acceptance, a causal
speed gain and M1 are not claimed.

## Change and boundary

The generated `mem_read8/16/32` and `mem_write8/16/32` helpers already contain
direct, within-page RAM paths. The selected adapter supplied an all-null page
array, so these helpers always called the host memory functions.

`StaticArmBackend::RefreshMemoryPages` now binds the actual current
`MemorySystem` page-pointer array at Run entry and after successful SVC return.
It reads the current memory-system table rather than an inactive CPU's saved
table. Remapping, watchpoints and rasterizer cache invalidation update this same
array. The pinned `PageTable` requires null pointers for every non-`Memory`
attribute. Cached GPU pages, watchpoints, unmapped pages and cross-page accesses
therefore retain the existing callback paths. The array is not copied.

Presence of any trace, write observer or execution observer keeps the all-null
callback table in the current authored adapter, including an inactive observer.
The historical live adapter has only the trace argument; its exact overlay
guards trace presence. No Context, Host, Entry, data member, constructor or
virtual-method change was introduced by this slice. Guest ticks, per-instruction
charging, floating point, SVC and budget handling remain source-identical except
for page binding at the two stated boundaries. Their runtime equivalence still
requires the canonical gate.

The live artifact predates the committed observer constructors and data members.
Replacing it with the whole current adapter alone would use a different caller
closure. The candidate instead patches the exact original adapter/header. Root
must integrate the current authored source with its matching caller closure.

## Live measurements

All observations use native Chrome Guest 154.0.8037.95, loopback HTTP, the complete
original File, `reference_9000`, record mode, 60,000-presentation/900-second bound,
active original stereo audio and default startup buffering zero. Mario remained
stationary at the starting area. Ground, Mario, trees, blocks, castle and HUD were
visually confirmed. DevTools installed the unchanged shared collector and stayed
closed throughout its 10-second warmup and next 30-second display window.

| Observation | Intervals | Display/s | Median ms | p99 ms | Over 33 ms |
| --- | ---: | ---: | ---: | ---: | ---: |
| Root common before | 384 | 12.83759275 | 87.1475 | 159.060 | 74.479% |
| Primary after, fully sampled | 434 | 14.48605384 | 78.9350 | 137.125 | 63.594% |
| Earlier after, late shared sampler | 400 | 13.34388835 | 86.8125 | 167.980 | 71.000% |

These are shared-load diagnostics. The primary after's one-minute load samples
were 4.84375, 4.21289 and 4.07910. Root's common before samples were 7.86768,
7.44678 and 7.42627. The earlier after supervisor recorded 8.38574, 7.72119 and
9.05664. Different host load and two differing after windows prevent attributing
the measured difference to the CPU source change. No 60 fps or audio acceptance
was observed.

The primary after runs from 12:16:11.065 through 12:16:51.065 UTC. Shared samples
at 12:16:18.813, 12:16:33.816 and 12:16:48.799 cover it, with factory classification
complete and no factory script observed. The earlier after runs from
12:11:22.479 through 12:12:02.481 UTC. Its shared sampler began 31.45 seconds into
the observation. Separate supervisor samples cover that full window. Shared
factory absence was unproven while the stdin sampler was active. That raw result
and its limitation remain preserved.

Native telemetry is separate. Between the primary after's first and last
displayed renderer frames 12062 and 12502, 440 natural native intervals measured
14.67672765/s, median 66.8973389 ms, p99 143.5200195 ms and 50% over 33 ms. Six
native presentations in this extent were not individually observed by the
display collector. Root's common before native extent measured 12.82624244/s,
median 75.2425537 ms and p99 186.8901367 ms.

Audio deltas include the entire 40 seconds, including warmup:

| Observation | Original source frames/s | Underrun frames |
| --- | ---: | ---: |
| Root common before | 6195.2 | 1060096 |
| Primary after | 7680.0 | 1000704 |
| Earlier after | 6912.0 | 1031424 |

The original source rate remains 32728 Hz. Both after windows starved. Peak Wasm
heap was 1 GiB with zero growth. First eligible presentation was 26.871320064
seconds after runtime admission. All 414 retained shader logs pass; unsupported
state counts are empty. Natural Stop completed with final screens, live input and
original PCM saved. CPU0 recorded 5,952,788,311 instructions, CPU1 zero, and both
recorded zero interpreter/JIT fallbacks. These counters do not replace an exact
before/after guest replay comparison.

## Build and source evidence

The lane's `build/` was created using an APFS `cp -cR` clone of Root's actual
runtime build before source work. Sealed shared input paths stayed read-only.
The selected baseline manifest is
`188b6cb68f99f0f50c3e56f9fc87b1a8d382063be3b5acf2b6e10206c5bce84f`.
The common before receipt is
`8c8626446661973aa2582ea24c136027fb3a1e1533c2b8e5ce68925fd7c73383`.
The shared collector/arbitration script is
`87120b74aa9624cddca830e16750b4968ecaa4d13a05b4bb55cc71601c3c5e08`.

`tools/static_recompiler/build_runtime_cpu_candidate.py` accepts the sealed
baseline manifest/receipt, recovery receipt, shared coordination script, R2 build
token and an absent lane output directory. It verifies pins and reservation,
patches the exact old adapter/header from the original successful compile recipe,
compiles one object at the original effective `-O1`, APFS-clones the CPU adapter
archive, replaces that one named member and retains archive names/order. It then
uses the full selected link command, replacing only the CPU archive and output
path. Generated translations, scheduling and SoftFloat remain unchanged. All
selected pre-JavaScript files, objects and archives are hashed before/after.
The full baseline manifest, exact commands, logs, original recipe, source patch
and execution-time builder are retained locally. No 37-unit build was run.

Build token `99eca09e6c414848aee82eec275e14fb` covered a fully detached nice5 build.
It completed successfully in 4.35 seconds and was released. The module remains
3,138,813 bytes with SHA256
`34b63c89ea3b140653fd4d860e1dbc66561cb6e5928dedaa053b617452d3e92a`.
The candidate Wasm is 152,419,333 bytes with SHA256
`ddd385bc299fe519b093bf2a5a396366befff787082f491dc802e3ebe9fceee2`.
The baseline Wasm remains 152,419,079 bytes with SHA256
`92df445a692a30e4e24ffca7c7fba573a3253a21e9e0db90e4a4e37747acd22f`.

Local evidence is under `build/runtime_translated_code/`:

- `page_table_candidate/build_manifest.json`, original linked manifest snapshot,
  exact old-ABI `adapter_source.patch`, original-header overlay and build logs.
- `page_table_world_measurement_receipt.json`, both raw observations and both
  shared-script analyses, native corroboration and hashes for the complete handoff.
- Build, failed server, successful server, scheduled sampler and shared token
  resource receipts. The successful server's 40 asset/sidecar seals verify after
  the run, and its configuration exactly equals the common baseline's values.
- Saved capture manifest/receipt, native frame CSV, shader logs, HID events/movie,
  original PCM/audio events, final RGBA/screens and `page_table_world_final_frame.png`.
- `page_table_helper_handback.json`. Owned Guest window closed; observed hot
  renderer PID95735, server PID94635 and sampler PID17332 are absent. Measurement
  token `353ad90546844a45acba617b739452c6` is released.

No generated translation, game file, extracted data or save is committed. The
source commit and final authored-file seals are recorded in the local handoff.

## Retained failures and next slice

The initial relocated server could not find its sibling FastFrames worker and
exited 1. Its log/resource receipt are retained. A read-only link supplies the
exact baseline worker. The ignored server overlay changes only the output-path
validator so capture files remain inside this lane's build directory.

The first build manifest omitted the existing server's module/Wasm digest and
gameplay-capability keys. These metadata keys were completed without changing
the linked binary. The original build receipt and execution-time builder are
retained; the committed builder now writes the required keys directly. Python
AST and whitespace checks pass. `development_environment.sh` found this lane's
missing `.venv`; the stdlib-only candidate builder succeeded using its explicit
pinned Emscripten recipe. Local environment setup still needs repair before a
traditional repository-toolchain command.

Root's canonical 360 guest/HID/PCM/counter gate remains the integration gate.
The next bounded CPU experiment should compile the existing adapter and scheduler
at `-O3` while keeping the translation archive and final link unchanged. This
targets the per-instruction callback/dispatch path without a 37-unit rebuild or
new ABI layout. Follow it with a measured address/mode dispatch cache only if
the small compile slice leaves dispatch dominant. Generated timing-expression
removal, CPU LTO and SIMD remain unmeasured later experiments; numerical behavior
and observer builds must stay intact. Current source/report are frozen. Builds
and browser work stay parked during the coordinated R3 check and R4 measurement.
