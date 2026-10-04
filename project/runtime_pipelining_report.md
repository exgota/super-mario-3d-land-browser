# Runtime pipelining candidate

R1, `root/runtime-pipelining`, frozen base
`d36ce04bc054de600aa547461e5c4d5bfbb23a9c`. Implementation is opt-in. The selected
game has not yet moved its PICA execution to this pipeline. Root owns that wiring.

All ten new implementation files are independently authored for this work.
Existing Azahar interfaces and the pinned Emscripten runtime are design references.
No renderer implementation was copied. No new license is selected here. Existing
third-party license declarations remain intact. Repository licensing is the owner's
decision.

## Common before-run

Root's sealed, visually confirmed stationary World 1-1 in plain Chrome supplies
the shared before-run. Original stereo audio is active, startup buffering is zero.
Ten seconds warm up, followed by thirty seconds of displayed intervals:

| Measure | Before | R1 after |
| --- | ---: | ---: |
| Display updates/second | 12.8375927508 | Unmeasured |
| Conventional median interval | 87.1475 ms | Unmeasured |
| Nearest-rank p99 interval | 159.06 ms | Unmeasured |
| Intervals | 384 | Unmeasured |
| Over 33 ms | 74.4792% | Unmeasured |
| Same-frame native updates/second | 12.8262424419 | Unmeasured |
| Full 40-second audio underrun delta | 1,060,096 source frames | Unmeasured |

Collector window: 2026-10-04 11:42:04.796 through 11:42:44.797 UTC. Actual-window
one-minute load samples: 7.86767578125, 7.44677734375, 7.42626953125. No runtime-lane
build was present. Shared contention makes this diagnostic, not M1 acceptance.

Sealed evidence in Root's `build/browser_performance_resume/`:
`runtime_lanes_chrome_baseline_receipt.json`, SHA256
`8c8626446661973aa2582ea24c136027fb3a1e1533c2b8e5ce68925fd7c73383`.
Selected module SHA256
`34b63c89ea3b140653fd4d860e1dbc66561cb6e5928dedaa053b617452d3e92a`;
Wasm SHA256
`92df445a692a30e4e24ffca7c7fba573a3253a21e9e0db90e4a4e37747acd22f`.
Shared measurement script SHA256
`87120b74aa9624cddca830e16750b4968ecaa4d13a05b4bb55cc71601c3c5e08`.
The APFS build clone was created before changes. All fifty selected dependency
seals match the shared input. Generated code and game data remain local.

## Native execution interface

`BrowserGpuPipeline` is created and destroyed by the translated CPU thread. Its
constructor requires positive record, byte and unfinished-frame limits, a GPU
`Execute` function and a CPU `Complete` function. Root selects the limits. The
constructor waits for GPU-worker initialization before accepting a job.

`Submit(kind, frame_identifier, submission_ticks, immutable_bytes, result_capacity)`
copies input before release publication. Frame identifiers are monotonic and are
assigned to every job in the corresponding presentation interval. Sequence starts
at one. Zero from the lower-level `TryPublish` means backpressure only. Pipeline
submission waits for the oldest retained job and drains CPU completions to admit
more work. Input and reserved result bytes count against the bound until CPU
retirement, including jobs already dequeued or completed. Maximum records also
bounds empty jobs. No caller-owned pointer or guest callback enters the packet.

`Execute(header, input_span, reserved_result_span)` runs on one GPU worker. It
returns the bytes written after the requested GPU and memory work is coherent.
Results never exceed the declared reservation. Completion sequence is published
with release ordering; CPU acquisition precedes reading the result. FIFO execution
and retirement are enforced. Worker or completion failure closes admission and is
reported by the next control operation. Explicit `Shutdown` joins the worker and
reports failure. Completion handlers cannot reenter the pipeline.

`Complete(header, result_span)` runs only on the CPU producer. Root consumes or
copies the result and stages CPU actions keyed by sequence. After draining, Root
applies guest writes and the original GSP timing, kernel and interrupt actions,
before a dependent barrier returns to guest execution. The result span belongs to
the callback only; a deferred action must own its bytes. Root processes staged
actions after every submission/drain wrapper, including submission's internal
backpressure drains, and keeps deferred storage bounded. A dequeued record is not
completion. `DrainCompletions`, `WaitThrough` and
`Fence` are CPU operations. Root must drain at translated execution safe points and
before processing dependencies, including when no new GPU job is submitted.

## Snapshot addressing

CPU capture uses `CaptureBrowserGpuMemorySnapshot(slices, maximum_bytes)` where each
slice is `{physical_address, const_byte_span}`. It copies sorted, disjoint physical
ranges. The schema is little endian:

| Byte offset | Value |
| --- | --- |
| 0 | `0x53555047`, bytes `GPUS` |
| 4 | Schema version 1 |
| 8 | Range count |
| 12 | Total snapshot byte extent |
| 16 plus 12 per range | Physical address, byte length, payload offset |
| End of directory | Dense immutable range bytes |

On the GPU, construct `BrowserGpuMemorySnapshotView(snapshot_span)` once per job.
Its `Read(physical_address, bytes)` validates containment and uses binary search.
It returns a borrowed const span in the retained record. Missing ranges, crossing
range boundaries, malformed directories, overlaps and physical-address overflow
fail explicitly. They never fall back to live guest RAM. Adjacent slices remain
separate, so Root should coalesce ranges where one bounded read spans both.
The convenience free reader validates a view on each call and is not the hot-path
adapter. Snapshot view and pointers must not survive their job's retirement.

## Exact Root adapter requirements

Root's first integration will queue immutable prepared draw configuration and R3
raw vertex descriptors, rather than wait for full PICA snapshot coverage. Command
parsing remains on CPU in this intermediate stage. This can overlap GPU render
execution with later CPU work, but is not whole-PICA relocation or measured speed.

`BrowserGpuRenderPacketBuilder(metadata, maximum_bytes)` copies each tagged byte
section in `AddSection(tag, bytes)`. `Build()` returns an owned packet for Submit.
The cap is the existing queue byte capacity minus the result reservation; padding
and directory bytes count against it. Root assigns tags and defines each section's
contents. There is no guest/PICA/kernel dependency or serialized callback/pointer.

`BrowserGpuRenderPacketView(packet)` validates schema, kind, extent, unique tags,
8-byte-aligned section offsets, reserved words, bounds and no overlapping nonempty
sections. `Section(tag)` returns a borrowed const span and rejects a missing tag.
`Metadata()` supplies kind, frame identifier and guest submission ticks. Root checks
these against the outer queue header. The sequence stays in the queue header.

Render packet schema is little endian: bytes0..3 `GPRP`, version at4, total byte
extent at8, section count at12, command kind at16, directory-entry size16 at20,
64-bit frame identifier at24, 64-bit submission ticks at32. The directory starts
at40. Each16-byte entry holds tag, aligned payload offset, byte length and zero
reserved word. Tags are arbitrary32-bit integration identifiers. Zero-length
sections are valid. Builder padding is zero. View validation occurs once per job.
Root routes prepared-state and native-descriptor sections to the existing selected
renderer and `tryDrawVertexBatch` on the same GPU pthread, with CPU-only completion
and readback barriers. No additional worker or Wasm instance is added.

Asynchronous raw-draw admission must also preserve explicit CPU fallback. If GPU
`tryDrawVertexBatch` returns false for a previously unknown shader/state, CPU cannot
already have claimed the draw was submitted and advanced past its fallback inputs.
Root can preflight new program/state keys with a sequence barrier, cache their
accepted GPU status, then queue later accepted draws without that barrier. Rejected
draws retain the original CPU path with GPU-owned output submission. An alternative
GPU-side fallback requires all of its immutable input, which is not yet provided.
Failure must not silently omit a draw. Completion callbacks cannot recursively
submit fallback work; stage any CPU action for the next safe point after draining.

1. Instantiate the pipeline only behind an explicit runtime opt-in. Preserve the
   unchanged synchronous software/reference path when disabled. Add the three
   native source objects and the two worker files as pre-JS inputs to the selected
   manifest recipe. Do not instantiate a second Wasm module or custom replacement
   for generated pthreads. Every generated pthread keeps the selected module URL.
2. Split `GPU::Execute` dispatch from CPU-side GSP completion. CPU publication
   captures guest command words and all inputs, frame identity, guest submission
   ticks and the CPU completion action's identity. The completion action stays in
   CPU-owned state keyed by sequence. Never enqueue the unchanged GSP handler.
3. GPU owns PICA registers, command processor/assembler/shader state, bridge surface
   and texture caches, renderer and GL context. CPU reads/writes of those fields
   become jobs or ordered barriers. Initialization/reset/save/restore/Stop must
   fence before changing or destroying that state. SetBufferSwap, ColorFill,
   ReadReg and WriteReg cannot continue to race direct PICA accesses.
4. Capture initial and chained command ranges, shader/swizzle/register state,
   uniforms, vertex-loader/index ranges, texture/LUT data and initial target bytes
   before publication. Root supplies the exact range enumeration against the
   selected private GPU/PICA adapters. Capture must include state changed by the
   command list itself and chained trigger commands. Incomplete enumeration is a
   failed opt-in admission, not a read of shared mutable RAM. GPU reads use the
   per-job snapshot view. Pointer arithmetic must stay inside its bounded span.
5. Replace GPU-side timing reads with `header.submission_ticks`. Capture and debug
   observers must not call the live CoreTiming object on the GPU worker. Do not
   call CoreTiming/SVC/kernel/current-process page-table APIs from GPU execution.
6. GPU writes/readbacks go into the reserved result extent. CPU actions after draining apply
   those bytes through the normal guest memory/coherence observer path before GSP
   delivery. CPU readers of GPU-written targets wait for the relevant sequence.
   Root defines the result schema and target range list for the exact adapter.
7. Preserve guest completion timing and ordering at CPU safe points. An event that
   depends on GPU work cannot signal early, and the CPU cannot advance through a
   blocking dependency without draining completion. Keep VBlank timing/kernel
   mutation on CPU. Queue presentation/readback work before publishing the frame.
8. Save renderer diagnostics on the GPU worker before Shutdown. Existing direct
   CPU EM_JS calls would address the CPU worker's realm, not the GPU owner. Route
   all GL calls and GPU diagnostic extraction to the same worker. Shutdown drains
   jobs, joins and releases that worker's GL context.

The required immutable range enumerator and shared GPU/GSP adapters are not yet
implemented by Root. These are execution prerequisites, not a claim of relocation.

## Ordered barriers

`BrowserGpuScheduling::Barrier(reason, sequence)` waits through that published
sequence and drains coherent CPU completion. Sequence zero selects the latest
published sequence. Counters identify each explicit barrier:

| Reason | Root call boundary |
| --- | --- |
| GuestMemoryRead | CPU/DMA reads an outstanding GPU-written physical range |
| MemoryFill | Before CPU fill writes to a live GPU target or resource |
| MemoryTransfer | Before DMA/display transfer reads or replaces a dependent range |
| CacheInvalidation | Before invalidating a dirty surface/resource |
| CommandOrResourceReuse | Before mutable guest source/state is reused without a complete owned snapshot |
| Presentation | Before frame pixels become CPU-visible or PDC/frame delivery depends on them |
| Stop | Before tearing down guest GPU state, followed by worker join |

Barriers must apply only to actual dependencies. Fencing every submission would
serialize the pipeline. Snapshotting valid read-only inputs permits CPU progress
while that GPU job executes. GPU submissions that depend only on earlier GPU work
remain FIFO on the GPU worker. Published/completed/retired sequences, retained and
peak records/bytes/frames, admission waits and barrier counts are available for
the live receipt. Actual held-input-to-presentation latency remains unmeasured.

## Browser worker admission

`BrowserRuntimeWorkerTransport.mjs` validates secure context, cross-origin isolation,
exact origin and shared Wasm buffer. Its optional outer-worker sharing helper uses
structured sharing without a SharedArrayBuffer transfer list; an OffscreenCanvas
is transferred separately. It does not load Wasm or start an additional worker.

`BrowserGpuWorker.mjs` initializes only inside an existing `em-pthread-*` worker.
CPU admission writes into six native atomic words in actual shared Wasm memory.
The GPU observes the CPU word and changes cell two from 41 to 42, then creates the
selected renderer and its own live OffscreenCanvas WebGL2 context. Native admission
words are schema, CPU isolated, shared sentinel, GPU isolated, GL ownership and
initialized. A browser pass is `[1,1,42,1,1,1]`. Native OS-only checks have zeros in
browser-only fields and cannot establish browser admission.

The helpers are pre-JS in every generated pthread, but only the pipeline GPU thread
calls initialization. PICA/renderer execution is on that worker, not a separate JS
draw worker. Headers/routes belong to Root and Web; exact `127.0.0.1:<owned port>`
origin must be retained for page, outer worker, module, Wasm and pthread workers.
Web's source/HTTP isolation contract is ready. Page plus worker native admission,
actual shared-Wasm/GL ownership and gameplay acceptance remain unmeasured.

## Validation and next integration

Both new JavaScript files pass Node syntax checks. Eleven mocked JavaScript fixture
cases pass. The initial fixture caught an invalid Uint32Array Atomics.notify call;
the redundant notification was removed because native consumer-ready notification
already wakes CPU. Original failure and corrected source hashes are retained in
`build/runtime_pipelining_verification/`. These fixtures do not establish native
browser isolation or WebGL acceptance. Native concurrency, Wasm build,
native browser admission and matched game after-run are pending. No browser has
been launched by R1. Root's arbiter remains mandatory for heavy compilation and
every browser run. R2's first after-run and R4's cache after-run have priority.

Next: bounded native concurrency checks, source/object seals, then Root's opt-in
adapter wiring. Root retains the software reference and selects rendering-regression
checks when needed. Candidate gameplay uses the shared
shared World collector, an owned reservation and a matched plain-Chrome stationary
World 1-1 after-run with original audio. Report observed overlap separately from
frame speed. No M1 or 60 fps claim is made.
