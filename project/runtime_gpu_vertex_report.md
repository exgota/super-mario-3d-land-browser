# R3 GPU vertex submission handoff

Status: the initial raw-vertex integration reached live World 1-1 at 19.035949/s, median 54.9925 ms and p99 111.395 ms. The aligned uniform ring is queued, unmeasured. GPU-only presentation and immutable display-copy sources are ready for Root/R1/R4 integration; their functional driver checks remain separate from a guest World measurement.

## Selected inputs and common live baseline

The active `build/` was created with `cp -cR` from Root's actual build tree before source changes. The earlier unused canonical-tree clone was removed in the owner-authorized local cleanup. Local game data/compiler links remain uncommitted. Clone evidence is in ignored `build/runtime_gpu_vertex_preparation/clone_identity.json`.

The exact selected input is Root's `build/browser_command_fill_module_candidate`, including its complete manifest and private dependencies. The committed renderer prototype is not that input.

| Input | SHA256 |
| --- | --- |
| Selected module | `34b63c89ea3b140653fd4d860e1dbc66561cb6e5928dedaa053b617452d3e92a` |
| Selected Wasm | `92df445a692a30e4e24ffca7c7fba573a3253a21e9e0db90e4a4e37747acd22f` |
| Common before receipt | `8c8626446661973aa2582ea24c136027fb3a1e1533c2b8e5ce68925fd7c73383` |
| Shared collector script | `87120b74aa9624cddca830e16750b4968ecaa4d13a05b4bb55cc71601c3c5e08` |

Receipt: `/Users/exgota/super-mario-3d-land-browser/build/root_browser_gameplay_session/build/browser_performance_resume/runtime_lanes_chrome_baseline_receipt.json`.

Root confirmed native plain Chrome Guest, World 1-1, stationary Mario, restored ground/trees/blocks/HUD, original active stereo audio and default buffering zero. The collector ran 2026-10-04 11:42:04.796 through 11:42:44.797 UTC. After ten seconds of warmup, the next thirty seconds supplied 384 display intervals.

| Measurement | Common before | R3 after |
| --- | ---: | --- |
| Display updates/s | 12.8375927508 | 19.0359493904 |
| Conventional display median | 87.1475 ms | 54.9925 ms |
| Display p99 | 159.0600 ms | 111.3950 ms |
| Display intervals over 33 ms | 74.4792% | 64.5614% |
| Same-frame native updates/s | 12.82624244 | Unmeasured |
| Native median / p99 | 75.2425537 / 186.8901367 ms | Unmeasured |

Actual-window one-minute load samples were 7.8677, 7.4468 and 7.4263, with no runtime-lane build. Full forty-second audio underrun delta was 1,060,096 frames and source delivery 6,195.2 frames/s. First eligible presentation was 27.54439 seconds after runtime admission; peak heap was 1 GiB with zero growth. All 402 baseline shader diagnostic records passed, unsupported zero. These are shared-contention diagnostics, not M1 evidence. The earlier 12.946839/s World sample and instrumented 8.6/s profile are not the matched comparator.

## Owned source and integration boundary

R3 owns `PicaWebGlVertexShader.mjs`, `PicaWebGlDrawCache.mjs`, `BrowserWebGlVertexSubmission.cpp` and `.h`, plus this report. Root extended ownership to `PicaWebGlPresentation.mjs` and `BrowserWebGlPresentation.cpp`/`.h`. Root owns bridge, renderer, command/Pica call sites, delay charging, assembler/winding guards, fragment state and candidate build/admission. The implementation is authored independently against the selected PICA header ABI and documented instruction encoding.

Native entry:

```cpp
bool Port::TryDrawBrowserWebGlVertexBatch(Memory::MemorySystem&, Pica::PicaCore&, bool indexed);
void Port::PrepareBrowserWebGlDraw(Memory::MemorySystem&, Pica::PicaCore&); // Root implements
void Port::InvalidateBrowserWebGlVertexResources(std::uint32_t address, std::uint32_t bytes);
const Port::BrowserWebGlVertexSubmissionStatistics& Port::GetBrowserWebGlVertexSubmissionStatistics();
```

Root calls the entry after unchanged vertex delay charging and before `LoadVertices`, with GS disabled, List topology 0 or Shader topology 3, nonzero count divisible by three, an empty assembler and no pending inverted winding. Native submission repeats the topology/count checks and validates every physical byte range. Successful Root submission closes the draw through `FinishBrowserWebGlDraw`; rejection enters the existing CPU fallback with ordinary winding.

Native code calls Root's preparation helper, then synchronously calls `renderer.tryDrawVertexBatch(descriptor)`. The descriptor owns no asynchronous lifetime. R1 or Root must copy its heap views before publishing work to another thread or deferred queue.

JavaScript attachment order places `PicaWebGlVertexShader.mjs` before `PicaWebGlDrawCache.mjs`, then the Root renderer integration. Both are global pre-JS modules, without ES exports.

```javascript
const cache = globalThis.createPicaWebGlDrawCache(renderer.gl, {
    maximumPrograms: 128, maximumResources: 256,
    maximumResourceBytes: 256 * 1024 * 1024,
    textureUnitBase: 8, uniformBlockBinding: 1,
});
const prepared = cache.prepare(descriptor, currentSpecializedFragmentSource);
const newRecords = cache.takeShaderDiagnostics(); // Drain on success and fallback.
if (!prepared.supported) return false;
// Root configures the same fragment/draw state with prepared.program and prepared.uniforms.
cache.draw(prepared);
// Root marks surface dirty and increments draw/triangle counters after submission.
```

`prepared.uniforms` is a Map for Root's existing per-program uniform lookup. Root must reapply fragment samplers 0 through 7, register UBO binding 0, lighting/fog, viewport/depth, depth and separate-alpha uniforms, blending and raster state after selecting the combined program. CPU fallback must reset the ordinary output-vertex program configuration. Raw vertex resources start at texture unit 8, vertex UBO binding 1. The cache queries actual combined/vertex texture and UBO limits and explicitly rejects excess usage.

## Descriptor schema 1

| Field | Meaning |
| --- | --- |
| `primitiveType`, `vertexCount`, `indexFormat`, `vertexOffset` | Topology 0/3, complete triangle count, index bytes 0/1/2; nonindexed offset only |
| `programWords`, `swizzleWords`, `entryPoint`, `programIdentity` | Full 4,096-word instruction and operand arrays, entry and native cached-hash bucket hint |
| `inputAttributeCount`, `inputRegisterMapping` | Active input count and sixteen destination register numbers |
| `attributes` | Ordered input records: index, default flag, format 0/1/2/3, components, compact bufferIndex, byteOffset, byteStride |
| `buffers`, `indexBuffer` | Physical address and Uint8Array guest byte extent, indexBuffer null for nonindexed draws |
| `outputMask`, `outputCount`, `outputMapping` | Enabled shader output registers and seven rasterizer semantic mapping words |
| `floatUniforms`, `integerUniforms`, `booleanUniforms`, `defaultAttributes` | 384 float values, sixteen integer values, sixteen-bit bool mask, 64 default floats |
| `clipEnabled`, `inputToUniform` | Explicit fallback modes |

`consumedOutputSemantics` is derived inside cache preparation from the exact specialized fragment source and participates in the static program key. Position is always live. Standalone translator calls without this derived field conservatively require all mapped outputs.

## Vertex semantics and CPU work

Raw guest attribute/index bytes are pulled in the vertex shader using `gl_VertexID` and integer R32UI textures. Signed BYTE/SHORT are sign extended; UBYTE is nonnormalized; FLOAT uses little-endian `uintBitsToFloat`. Unaligned and cross-word loads are explicit. Native loader interpretation preserves element alignment, padding selectors 12 through 15, later-loader overrides, default register attributes and sequential input-register collisions. Missing vector components are `(0,0,0,1)`.

Instruction operands retain source swizzles, negate bits, destination masks and address-relative uniforms. Matrix operations use those same instruction rules. Enabled shader output registers are compacted before the rasterizer's component semantics are mapped. Unmapped output semantics start at one. Color is absolute-valued and saturated before interpolation.

The fragment varying interface is unchanged: `vColor` vec4, `vTextureZero` vec3, `vTextureOneTwo` vec4, `vQuaternion` vec4 and `vView` vec3. Clip position is `(x,y,2*z+w,w)`; Root keeps the existing viewport/depth range. Custom clip-plane and input-to-uniform modes explicitly fall back.

Triangle quaternion sign follows the first vertex in each original triangle. Nonfirst vertices evaluate that first vertex again on the GPU before the sign comparison. This costs five shader evaluations per triangle. Vertex order and winding are preserved without CPU output expansion.

Temporary/output arrays start at zero for each GPU evaluation. Path-sensitive definite-assignment and backward component-liveness analysis reject observable dependencies on a prior vertex's scratch values. Dead arithmetic components and fragment-unused texture W do not force fallback. A projected texture fragment that consumes an unsafe W remains rejected. Static analysis is cached; it is not a per-vertex CPU interpreter.

The native path still scans each index once for the maximum memory extent. It also copies uniforms per draw, compares resource bytes for coherence and interprets at most sixteen attributes per draw. It performs no CPU attribute conversion, PICA vertex shader execution, output-vertex expansion or triangle float assembly. Byte comparison and the index scan remain CPU costs to measure.

The selected platform's f24 wrapper stores float32 after raw conversion. Uniforms/defaults use its `ToFloat32`; GPU arithmetic preserves zero-times-infinity behavior, NaN-sensitive min/max selection and separate multiply/add expressions. Driver fusion, exceptional values and floating-point precision still need observed rendering validation. No full numeric exactness claim is made.

Supported lowering includes observed arithmetic, CMP/MOVA, CALL/CALLC/CALLU, IFU/IFC, forward conditional jumps and a single loop with BREAK/BREAKC. Backward/crossing jumps, recursion, nested loops, invalid mappings and unsupported opcodes reject with reasons. No invented instruction execution cap is imposed on GPU work.

## Cache identity and invalidation

Translation lookup uses native hashes only as bucket hints, then compares every instruction and operand word. Static identity includes all input/output mapping, active attribute layout/format, entry, index format, buffer count, clip/input modes and fragment-consumed semantics. Combined program identity includes the exact specialized fragment source. A bounded two-level lookup uses the full immutable translation key and exact fragment string separately, avoiding repeated combined GLSL string construction. Negative compile results and LRU eviction retain the same identity. Raw sampler units are initialized once per program; vertex offsets update only when that program's value changes. Uniform values and resource addresses remain dynamic.

Raw resource identity includes physical address, byte extent and R32UI representation. Every cache hit compares complete guest bytes. Retained bytes are copied; retained Wasm views are forbidden. Changed bytes allocate a new texture, avoiding mutation of an earlier submitted draw. New texture packing is explicitly little endian, including a partial final word. Resources and programs use bounded LRU eviction; current draw resources are protected from self-eviction. Address-range invalidation deletes every overlapping retained resource.

The 1,872-byte std140 UBO contains float uniforms at byte 0, four integer vectors at 1,536, bool mask at 1,600 and default attributes at 1,616. Unchanged bytes skip upload and retain the current range. Changed packets append into a 1 MiB ring using the actual WebGL2 offset alignment. The ring is orphaned on wrap, and binding 1 selects only the active 1,872-byte range. VAO and UBO persist across draws.

`resolveConvertedTexture(descriptor, operations)` is an available byte-coherent fragment-texture helper with complete address/extent/format/conversion identity and owner-supplied conversion/upload/destruction. It does not replace Root's existing decoder or texture cache. Root must explicitly attach it if adopted. Current integration can retain the selected fragment texture cache while raw vertex byte textures use this R3 cache.

## Diagnostics and verification

`cache.shaderDiagnostics` is cumulative. Each new combined program records successful and failed vertex compilation, fragment compilation and link logs as `{key, stage, passed, log}`. `takeShaderDiagnostics()` drains only records added since the prior drain. Cache hits emit no duplicate shader records. `prepared.diagnostics` is per-prepare, empty on success and populated on fallback. Compile failures are cached. `cache.unsupportedStates` counts reasons; Root should export a current snapshot rather than sum snapshots repeatedly.

Cache statistics cover successful draws/vertices, translation/program/resource/uniform hits and uploads, invalidations, uploaded bytes and fallback draws. Native statistics also cover pre-JavaScript fallback, invalid physical ranges and scanned index bytes. Export the native getter if native admission rejects must be counted; JavaScript cache counters alone cannot prove all PICA draws took the raw path. Root's earlier guard rejects are a separate count.

The maximum retained shader log count defaults to 12,288. Root's combined exported history must include existing fragment logs as well as these records and preserve failures. The current inventory requires 168 raw stage records if all 56 variants compile and link once, well below that bound.

Observed checks:

- The selected-ABI native object compiled with the manifest's flags, `nice -n 5`, detached, exit zero in 1.358016625 seconds. The source still matches its immutable compile input. No warnings/errors were emitted under those flags.
- Supervisor 27942 and compiler 27943 were independently observed absent before releasing shared build token `397cf2ef6ea64453807a2cec24c1a333`. Canonical receipt is in `build/runtime_coordination/397cf2ef6ea64453807a2cec24c1a333.json` outside this clone.
- Both JavaScript files pass syntax checks. Only R3-owned sources and this report are committed.
- The existing retained capture's first 512 command files contain 974 GS-disabled draws: 874 Shader3 and 100 List0. Seven reachable instruction families contain 19/76/103/299/346/371/388 instructions. All 974 draws generate supported source across 56 vertex/fragment variants, zero generation fallback. Final generation took 1,972.703 ms on this loaded host.
- This retained inventory covers source generation only. Unwritten inherited registers are explicitly zero-filled/listed. It does not establish live shader coverage, vertex correctness or speed.

Native object SHA256: `64186b5034c60f851fd18e104aefdc9a784fcac37d57de535da5a4723b7d66d6`.

Source seals and commit are in ignored `build/runtime_gpu_vertex_preparation/source_handoff.json`. The exact native command/result, retained validation cases/variants and failure history remain local. Generated shaders and captured game state are not committed.

An earlier conservative analysis rejected 861 retained draws; it was replaced with correlated control-flow/component liveness. A cleanup then left a stale `required` field reference, caught by the source inventory and corrected before freezing. Both failures remain recorded. The original selected module and software reference are preserved.

## Live GPU result and remaining costs

Root's job `20261004T131750353522_1591068a` measured native plain Chrome World 1-1 with visible geometry and original audio. Ten-second warmup plus thirty-second display window produced 570 intervals. The result includes R2's page-array CPU and the first R3 raw path, before the memo/ring or presentation changes. All 1,264,409 native-eligible draws submitted, zero fallback/invalid/guard rejects, and all 1,011 shader logs passed. Full forty-second audio underrun delta remained 897,536. No M1, human-play or full exactness claim.

`build/runtime_gpu_vertex_preparation/gpu_path_cost_profile.json` contains the cost profile. Whole-cold-run counters divided by 7,837 natural presentations give 161.36 draws, 26,377 state-cache calls and 5,816 forwarded changes per presentation. Fragment texture uploads averaged 0.055 and raw uploads 0.146. Readbacks averaged 1.507 and 4.477 ms; draw submission averaged 0.715 ms, excluding prepare/native work and GPU execution. These are cold-run averages, not World-window deltas. Readback timing includes its error query, so overlapping timings cannot be added.

The largest measured blocking GPU call is the synchronous surface readback. Root made its mandatory error query diagnostic-only and removed the duplicate full draw configuration. GPU-only presentation below removes readback and software pixel materialization from admitted ordinary frames while retaining true guest-read coherence.

## GPU presentation and display-copy interface

```cpp
BrowserWebGlPresentationResult Port::TryPresentBrowserWebGlFrame(
    const BrowserWebGlPresentationScreen* screens, std::uint32_t count,
    std::uint64_t rendererFrame, std::uint64_t sampledTicks);
```

The eleven screen words are `screenIdentifier`, `surfaceIdentifier`, `sourceX/Y/Width/Height`, `width/height`, `rotationQuarterTurns`, `flags`, `colorFormat`. Source coordinates use the texture's bottom-left origin. Clockwise rotation is followed by source-Y flip in flags bit zero. Output extent must equal the rotated source extent. Formats 0 through 4 are RGBA8/RGB8/RGB565/RGB5A1/RGBA4. Native admission copies numbers and exact uint64 decimal strings into `globalThis.submitBrowserWebGlPresentation`. Result is Unsupported 0, Submitted 1 or Dropped 2. Submitted means ordered transport admission, not completed paint.

```javascript
const presentation = createPicaWebGlPresentation(renderer, {publishFrame});
presentation.copyDisplaySurface({sourceSurfaceIdentifier, destinationSurfaceIdentifier,
    width, height, colorFormat, flipVertically}); // Numeric 0 or 1. Strict boolean flip.
presentation.present({schemaVersion:1, rendererFrame, sampledTicks, screens}); // Numeric 0/1/2.
```

Copy supports an unscaled source with equal width and source height greater than or equal to the destination height, plus a distinct GPU alias on the same context. Cropping retains physical input rows `[0,height)` before optional destination-row reversal. Render-surface textures reverse physical row order, so the source texture rectangle starts at `source.height-height`, 80 for the observed bottom screen. Full-height copies retain Y offset zero. Requests exceeding the source rows or changing width reject explicitly. It creates/reuses a separate RGBA8 texture/FBO, applies Y flip and destination color quantization on GPU, and inserts an alias into `renderer.surfaces` only after submission. Missing/oversized/scaled/colliding sources and surface-budget failures reject explicitly. There is no bitmap-admission callback on copy. A publisher can be installed later. Alias `readback` starts null; Root allocates CPU storage only for actual guest-read materialization. The renderer owns alias GPU-resource lifetime and native physical-address/format mapping. Root's unsupported transfer path retains the coherent reference.

Presentation publishes one GPU bitmap and screen rectangles. Normal World dimensions observed in the actual outcome are top storage 240x400/display 400x240 and bottom storage 240x320/display 320x240, format 2 and stride 480. A combined 400x480 bitmap places top at `(0,0,400,240)` and bottom at `(40,240,320,240)`. Source crop/rotation comes from Root's address/transfer mapping.

`publishFrame(packet, [packet.bitmap])` receives `{schemaVersion:1,type:'browser_webgl_presentation',sequence,rendererFrame,sampledTicks,width,height,bitmap,screens:[{screenIdentifier,x,y,width,height}]}`. Screen rectangles use the bitmap's top-left origin. Return true after admission transfers bitmap ownership to R1/R4, which close it after paint or downstream drop. Return false before transfer for backpressure; R3 closes it. Unsupported publisher results remain explicit fallback. Neither ordinary presentation nor copy calls `readPixels`, `getBufferSubData` or `getError`.

R1 owns ordered kind9 presentation and kind16 copy transport on the existing GPU owner and one shared Wasm. R4 owns bitmap crop/paint and acknowledgement. Root owns actual transfer eligibility, physical mapping, guest-read barriers and final reference export. `takeShaderDiagnostics()` drains presentation vertex/fragment/link logs without duplication; frame/copy/alias/drop/fallback counters are cumulative.

## Current checks and measurement gap

The presentation native object compiled with selected Wasm32/pthread/exception flags, nice 5, exit zero in 0.45205 seconds. Program-cache checks pass exact-source separation, LRU recompile, cached failed programs, diagnostic drain, bounded lookup disposal and per-program sampler/offset reuse.

Native headless Chrome 154 with ANGLE Metal Apple M4 passed all forty synthetic rotation/flip/color-format cases, including nonmax packed-color values, bitmap transfer ownership, explicit unsupported fallback and backpressure close. Three shader logs passed; context loss, browser process cleanup and helper absence were verified. This is functional GL evidence, not gameplay. Copy-specific driver verification remains pending: the latest attempts stopped before GL during shared browser-process inventory, including `kern_procargs2` errno 22 for a departing process. The source checks pass; no copy-driver success is claimed.

The ring candidate is private `build/browser_vertex_uniform_ring_candidate`, a cache-only relink of the measured raw module with unchanged Wasm, source commit `661bf66b`. Pending job `20261004T142041616072_28b246fc` was updated under `runner.lock` to R4's ordinary twenty-file `map_saved_reference` and `saved_map_world_one_navigation_recipe.json`, including `entry_node_anchor`. Its state is `held` so Root's descriptor-cache GPU/pipeline measurement runs first. R4 observed two fresh functional World entries in 108.4721 and 98.9805 seconds from server admission. These explicitly changed saved inputs do not establish a new speed comparison. Both earlier private-server setup failures occurred before browser admission and remain recorded.

Owner-authorized cleanup removed forty-two obsolete local build directories with no errors, preserving the Ring artifact, selected/native inputs and retained shader inventory. Shared-volume free space increased from 24.3270 to 30.0187 decimal GB during cleanup, a measured 5.6917 GB increase. Root was cleaning the same volume concurrently; that increase cannot be attributed exclusively to R3 or to logical APFS clone sizes. No shared or owner game data was removed.

The remaining acceptance gate is one integrated live World measurement with actual guest transport, framebuffer aliases, original audio and visible geometry. Under 33 ms is a target, not an observed result. Routine full exactness captures remain parked; an observed rendering regression permits a bounded comparison.

## Resumed crop correction

Root revision5 reached visibly intact World 1-1 in 62.235 seconds but still presented software RGBA. Actual transfers are top 240x400 to 240x400 and bottom 240x400 to 240x320, RGBA8 to RGB565, no scaling or flip. The previous full-height requirement rejected the bottom transfer and forced the entire frame into software presentation. The owned copy now accepts bounded destination-height cropping with the original framebuffer texture orientation. Request fields, numeric 0/1 results, immutable GPU aliases, transport and guest-read coherence are unchanged. Root retains transfer eligibility and address mapping.

Native Chrome 154 with ANGLE Metal Apple M4 passed four actual-extent copy cases: both heights, both flip directions, per-pixel RGB565 and clockwise rotation3 checks, source overwrite before paint and alias reuse. The existing forty presentation cases and two full-extent copies still passed. All nine shader logs passed. Active GL workload was 64.6 ms. Context loss, empty browser inventory and detached helper/process-group absence were observed. One result: `build/runtime_gpu_vertex_preparation/presentation_crop_coordinate_driver/result.json`. The initial crop check used a direct physical-row texture and missed the selected framebuffer reversal. Root identified the original `GetPixel(x,y)` mapping, and both code and per-pixel oracle were corrected before integration. The earlier check remains retained as superseded evidence. No game or Wasm was loaded.

The latest measured combined World result remains 20.399159/s, median 48.01 ms and p99 119.555 ms. Its whole-cold-run profile averages 129.78 draws, 3,536.11 forwarded state calls, 0.165 fragment texture uploads and 0.282 raw uploads per natural presentation. Synchronous readback averages 6.034 ms, larger than the measured 0.762 ms draw-submission time. These are cold-run averages, not World-window deltas. The crop correction is the prerequisite for removing normal-frame readback from both screens; its live effect remains unmeasured until Root's bitmap and natural-Stop integration passes.
