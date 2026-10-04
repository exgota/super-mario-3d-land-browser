# R3 GPU vertex submission handoff

Status: source handoff ready. Candidate gameplay, driver compile/link and rendered correctness are unmeasured. Root owns integration and the next live gates. No R3 browser was admitted.

## Selected inputs and common live baseline

The active `build/` was created with `cp -cR` from Root's actual build tree before source changes. The earlier canonical-tree clone is preserved as unused `build_initial_clone`. Local game data/compiler links remain uncommitted. Clone evidence is in ignored `build/runtime_gpu_vertex_preparation/clone_identity.json`.

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
| Display updates/s | 12.8375927508 | Unmeasured |
| Conventional display median | 87.1475 ms | Unmeasured |
| Display p99 | 159.0600 ms | Unmeasured |
| Display intervals over 33 ms | 74.4792% | Unmeasured |
| Same-frame native updates/s | 12.82624244 | Unmeasured |
| Native median / p99 | 75.2425537 / 186.8901367 ms | Unmeasured |

Actual-window one-minute load samples were 7.8677, 7.4468 and 7.4263, with no runtime-lane build. Full forty-second audio underrun delta was 1,060,096 frames and source delivery 6,195.2 frames/s. First eligible presentation was 27.54439 seconds after runtime admission; peak heap was 1 GiB with zero growth. All 402 baseline shader diagnostic records passed, unsupported zero. These are shared-contention diagnostics, not M1 evidence. The earlier 12.946839/s World sample and instrumented 8.6/s profile are not the matched comparator.

## Owned source and integration boundary

R3 authored only `PicaWebGlVertexShader.mjs`, `PicaWebGlDrawCache.mjs`, `BrowserWebGlVertexSubmission.cpp` and `.h`, plus this report. Root owns bridge, renderer, command/Pica call sites, delay charging, assembler/winding guards, fragment state and candidate build/admission. The implementation is authored independently against the selected PICA header ABI and documented instruction encoding.

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

Translation lookup uses native hashes only as bucket hints, then compares every instruction and operand word. Static identity includes all input/output mapping, active attribute layout/format, entry, index format, buffer count, clip/input modes and fragment-consumed semantics. Combined program identity includes the exact specialized fragment source. Uniform values and resource addresses remain dynamic.

Raw resource identity includes physical address, byte extent and R32UI representation. Every cache hit compares complete guest bytes. Retained bytes are copied; retained Wasm views are forbidden. Changed bytes allocate a new texture, avoiding mutation of an earlier submitted draw. New texture packing is explicitly little endian, including a partial final word. Resources and programs use bounded LRU eviction; current draw resources are protected from self-eviction. Address-range invalidation deletes every overlapping retained resource.

The 1,872-byte std140 UBO contains float uniforms at byte 0, four integer vectors at 1,536, bool mask at 1,600 and default attributes at 1,616. Unchanged bytes skip upload; changed storage is orphaned before upload. VAO and UBO persist across draws.

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

## Next gates and specific gaps

Root will integrate this frozen source with its separate bridge/renderer/Pica hook revision. R4's cache after-run retains scheduling priority under Root's executable admission contract. R3 holds no build or browser reservation.

Next R3 gates are actual driver compile/link for every encountered variant, explicit fallback/raw-draw counts, a live World image with original geometry and audio, and a matched candidate measurement through the shared collector. Routine 360-frame and byte-exact capture comparisons are parked under the latest Root contract; a bounded reference comparison is appropriate only for an observed rendering regression.

Shader compilation startup cost, GPU re-evaluation, byte comparisons, fallback coverage, driver arithmetic and overall speed are unresolved until that run. There is no measured R3 gain and no M1 claim. Normal Start under two minutes remains a Root entry-calibration question, outside R3's source ownership.
