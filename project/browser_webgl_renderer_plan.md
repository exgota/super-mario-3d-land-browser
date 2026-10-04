# Independent WebGL2 browser play renderer

Owner steering on October 4 removes the Wednesday target. Deliver a real-time
World 1-1 clip in a desktop browser tab as soon as possible. Phones and hosting
are out of scope. Software rendering remains unchanged as the exactness path.
GPU play acceptance reports per-frame visual difference and short-replay fps.
No subagents or delegated tasks. Root topic branches go through the integrator.

Owner approves WebGL2 on the advisor's recommendation on2026-10-04; the delivery
plan records the superseded WebGPU proposal. WebGPU follows only if measured
speed needs it. Whole-level acceptance is one original-movie start-to-goal GPU
run with Azahar state comparison, frame-time percentiles, Wasm heap growth and
pauses, audio underruns and input-to-display latency. Separate long exactness
replays stop. Short360 remains the regression gate. Diagnostic shader compile/
link logs and unsupported-state counts are required; skipped draws never pass.
The pinned mw2-recompiled design is an ideas-only reference, linked in the
delivery plan. First-use cache warming needs measurement beyond async creation.

The measured original recorded cadence is 59.83122493939037 submissions/second
of guest time. Software startup profiling attributes 85.738% of instrumented
wall time to rasterization and worker wait. CPU timings include timer overhead;
meeting the complete frame budget is unverified. Private software experiments
are parked. Performance documentation ca9953f3 was accepted at53edcf73.

The preserved World1-1 window contains51972command files,54930080register
writes and119752draw triggers. The read-only inventory seals every command file,
with SHA-256546565d3cd490b29742e99dec4b6f153a96c1629e5eb690aa90db677b9aa723f.
Draws use colorRGBA8, depthD24S8, depth/stencil tests, six combiners, blend add/
reverse-subtract, one active light and fog. Ordinary2D/disabled textures appear;
procedural texturing and shadow fragment operation do not appear. Texture-format
histograms include inactive units and must be filtered before defining final
used-format coverage. Unknown inherited prefix bits are disclosed in inventory.

T3's actual worker exposes WebGL2 through ANGLE Metal on Apple M4, native ETC1
and ETC compression, and a16384texture-size limit. Capability is not game speed.
The initial estimate was8–16hours to a first usable GPU World1-1 frame and
24–48hours to verify full-speed play. The first World-entry run now fails visually
and measures9.37/s. Those estimates need revision after World cost attribution;
the menu timings do not establish the World CPU or GPU budget.
The first actual GPU short360 replay completes in T3 at9.185970796140145 warm
presentations/second,359intervals39.081334784seconds. First eligible presentation
takes75.328185088seconds. This startup/menu run is not World1-1 speed. Original
movie/HID/audio timing and every PCM sample remain equal; CPU0393977876,
CPU1zero and both fallbacks0. Own Canvas pixels and complete audio consumption
pass. Final RGB mean absolute error is60.3416/255top and56.1195/255bottom;
61.22%top and99.999%bottom pixels exceed8RGBchannel error. Geometry appears
aligned, while background color and bottom-screen shading differ substantially.
Per-frame visual measurement and visual equivalence remain pending. Comparison
and final-only visual report remain local inbuild/browser_performance_resume.
The independently authored shaders compile/link and an8x8GPU triangle readback
passes. Prototype41e08919 links6commands. Two interface compile failures
and their independent source snapshots remain local. Color RAM synchronization
is implemented; depth RAM coherence and shared-depth-target behavior remain open.
A private diagnostic build preserves prototype arithmetic and scheduling. Its
180..360 interval measures81.3448ms/frame: draw configuration30.5771ms(37.589%),
color readback/GPU wait27.2703ms(33.524%), outside bridge16.4109ms(20.174%).
JavaScript target creation takes9.759ms/frame within draw setup, not an additional
share. Outside bridge work is not separately attributed to translated execution.
Proposed gains overlap: specialized shaders20–40ms, target reuse5–9ms and cached
draw state5–15ms/frame. These are estimates requiring replay measurement.

The specialized360 candidate completes9.5813574151warm presentations/second,
359intervals37.468594944seconds. First eligible presentation42.332695040seconds.
Every720screen frame identity, original input/audio timing/PCM, static CPU counts,
own Canvas output and216000consumed audio frames pass. All21generated shader
compile/link records pass; unsupported states count0within this short run.
Against unchanged software frames, mean RGB absolute error is0.165898top and
0.171846bottom on0..255; mean fractions with any RGB channel error>8are0.3677%
and0.0843%. Final errors0.128188top/0.158620bottom. A8/A4textures required zero
RGB; correcting that independently verified decoder error closes most color loss.
All12uncompressed formats match768reference texels/3072channels. ETC compression,
World1-1 lighting and visual equivalence remain unverified. Full-frame observer
IO affects timing. Audio has1209984underrun frames, so synchronized play has not
passed. The retained private state-cache/uniform-buffer/full-clear reuse candidate passes
short360, whole18.77199/s and steady28.20925/s. Buffer orphaning removes the
large in-use vertex-storage hazard; synthetic60draws fall20.1855to3.78325ms.
Actual vertex-streaming replay preserves720GPU frames and reaches54.31896/s
in the steady window. Cached raw lighting/fog tables then reach58.84189/s.
Software remains unchanged. The private presentation worker polls every8ms;
its bounded Canvas observation reaches58.51288updates/s. Public worker still
has25ms polling; the tested optimizations have not been published as runtime code.

Root owns the draw bridge, independent shaders, render targets, texture cache,
memory synchronization, browser build and actual replay. Existing CPU vertex
processing stays during first-frame integration; GPU shaders handle perspective
interpolation, texture combiners, lighting/fog and output tests. CPU vertex work
can move later if profiling requires it. No Azahar renderer implementation is
copied. Existing source/license records remain intact; Root makes no licensing
decision. Data formats use public specifications and current interface headers.

Format references: [Khronos ETC1 specification](https://registry.khronos.org/OpenGL/extensions/OES/OES_compressed_ETC1_RGB8_texture.txt),
[3dbrew texture storage](https://www.3dbrew.org/wiki/CGFX#TXOB), and
[3dbrew PICA registers](https://3dbrew.org/wiki/GPU/Internal_Registers).

## Independent texture conversion contract

Root has completed the formerly proposed parallel texture-conversion piece.
No second agent was started or assigned. The isolated conversion contract is:

Contract: `convertPicaTexture({format, width, height, data})` accepts an integer
PICA format, positive dimensions, and a Uint8Array of the exact observed texture
payload. It returns `{width, height, storage, color, alpha}`. Storage is `rgba8`
or `etc1`; color is an ordinary Uint8Array, alpha is null or an ordinary
width*height Uint8Array. Returned arrays must not use SharedArrayBuffer. RGBA
output is row-major R,G,B,A. ETC1 output is standard big-endian64-bit blocks in
linear block-row order. Optional ETC1A4 alpha is expanded to0..255. Conversion
preserves stored y order; the renderer applies the PICA v inversion. Unsupported
formats or invalid extents throw. No hidden fallback or texture downscaling.

Check each converted texel against the existing software decoder on bounded
retained texture inputs. All samples/images stay ignored/local. Write an
independent implementation from format specifications, without copying GPL
decoder code. Record tested formats, hashes, dimensions and remaining gaps.

## Latest measured candidate, 2026-10-04 02:34 Eastern

Private lookup-cache revision2 uses the unchanged360movie,720RGBA observer and
exclusive180..360profile. Whole359intervals take14.474114816seconds,24.80289845/s;
steady180..360takes16.9947ms/frame,58.84189027/s. Outside bridge11.2543ms(66.22%)
is unclassified, color readback/GPU wait2.7778ms(16.35%), draw configuration1.2703ms
(7.47%). Readback fell from16.2818ms after vertex buffer orphaning. Per-draw LUT
conversion fell after comparing/caching the raw tables. No software arithmetic changed.
Every720frame is byte-identical to the corrected specialized GPU. Original HID,
audio timing, PCM, CPU counts/fallbacks and all21shader compile/link logs pass;
unsupported states count0only within this short menu scope. Comparison SHA-256
60499aaa56a98609728f2ee3407d4483996a5ad5578c3a59062a10e1d23ce350.

Steady nearest-rank p50/p95/p99 are6.150/35.110/38.660ms, maximum39.925ms.
Cadence is bimodal, so this is not smooth60Hz acceptance. Across the complete
warm replay,51intervals exceed50ms, maximum638.355ms. Wasm heap stays1073741824
bytes, observed growth0; JavaScript collection remains unmeasured. First Canvas
writes across178unique stream updates measure58.51288/s. Producer-to-Canvas
p95 is34.630ms, not input-to-display latency or physical scanout. All216000source
audio frames are accepted/consumed, with419968underrun frames. Synchronized audio
and World1-1 speed remain unverified. Existing receipts, source snapshots, rejected
hardware-depth/direct-combiner candidates and failed diagnostics remain local.

## World-entry result, October 4, 2026

Independent lighting rules now match all128software-oracle cases exactly. The
initial shader failed120cases; both observations remain local. Coverage includes
eight models, geometric factors, selected signed inputs and normal mapping.
Rotated quaternions, back-facing lights and signed LUT boundaries remain open.
Native ETC1/ETC1A4 readbacks match5376texels/21504channels across24valid fixtures,
including all modifier tables, differential/individual blocks, flip modes,
multiple tiles and alpha nibbles. No GPL decoder or GPU implementation was copied.
Provenance receipt26657d7b seals sources, fixtures and actual browser observations.

Latest short360 telemetry candidate preserves all720specialized GPU images,
original HID/timing/PCM/CPU counts and21shader logs. Steady180..36053.6499/s,
p95/p9939.795/44.585ms, heap growth0 and497920audio underrun frames. Comparison
ba4d15a9. Changes remain private; software is unchanged.

The same candidate's bounded original-movie7800prefix reaches World1-1. It is
a failed graphics run: top blue, Mario silhouette, level geometry/HUD absent.
Final top RGB MAE140.2144/255,97.91%pixels exceed8channel error; bottom MAE1.1164.
Whole warm15.6275/s, late7680..7800window9.3694/s, mean106.730ms/frame,
p95/p99260.25/287.68ms. Wasm heap stays1GiB with zero observed growth;
JavaScript collection pauses remain unmeasured. All4285760source audio frames
are consumed, with12227328underrun frames. Input-to-display latency is unmeasured.
All402shader compile/link records pass and unsupported count0. This does not
earn visual-equivalence credit. Original HID/timing/PCM and CPU03693917823,
CPU1zero/fallbacks0agree.18of20snapshot files match; CEC mailbox and reference
capture log differ. Counts and PCM do not satisfy the Azahar state gate.
Observation6f905b2744b37f1a7b3790dff2dace4b3b665eea94a829f2d8a52d04cf07bc9c,
receiptc2baabff95c35dbc0e9a2a6a3372e8e3d90deb48383dd4514610698c83382d63.
All images/audio/captures remain ignored/local. Own browser/server are closed.

That run's180..360profile covers startup, not World. Top costs per frame there:
unclassified outside bridge46.948ms(59.84%), draw configuration17.637ms(22.48%),
triangle preparation4.976ms(6.34%). World top-three costs and gains remain
unmeasured. Next: verify stencil write permission against the software oracle,
capture an isolated World draw stream with complete initial state, and profile
the actual World window. Reuse that capture for graphics fixes, avoiding repeated
opening-sequence boots. The original9000movie still ends at bridge crates;
a verified start-to-goal movie/state comparison remains necessary.

A clean potential parallel diagnostic piece is expanding isolated lighting
fixtures to rotated quaternions, back-facing lights and signed LUT boundaries.
It can use new ignored fixture/probe files without touching Root's renderer,
bridge or builders. Estimated overlap saving2–3hours, not a measured promise.
No second agent has been started or assigned.

## Verified stencil correction

The software oracle confirms that framebuffer depth/stencil write permission0
prevents both writes;1/2/3allow them. The GPU applied stencil actions even with
permission0. A one-line mask correction now matches all64isolated browser cases;
the initial path fails15. All18compile/link records pass,unsupported0. Probe
receipt62db53db42356052a6326069b52fa8db2c95811c619d1333300dc242106c7daa.
The selected private module links successfully. Its unchanged360regression gate
preserves all720specialized GPU images, original HID/timing/PCM, CPU counts,
own final Canvas, all216000audio source frames and21shader logs. Comparison
7a240f2c705be28a111f10acd8532c384e593c60cfdc52106dd9c0587e916b24.
Steady180..36049.5436/s,p95/p9944.515/51.855ms,485376audio underrun frames.
This is menu performance on a shared host, not a measured World improvement.
The strict software checker still rejects four approximate GPU visual outputs;
its input/audio outputs pass. No comparison rule changed. An intermediate live
streamed-frame read was mistaken for final output; the complete720comparison
corrects that inference. World impact remains unverified. Next checkpoint:
`root_webgl_world_entry_checkpoint_2026-10-04.md`. All own helpers are closed.
