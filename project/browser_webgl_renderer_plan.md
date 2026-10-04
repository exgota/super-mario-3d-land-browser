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
The initial estimate is8–16hours to a first GPU World1-1 frame and24–48hours to
verify full-speed play. These are engineering estimates, not measured promises.
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
passed. A private state cache/uniform buffer/full-clear target reuse candidate
c74f470d builds in5commands; its short replay is next. Software remains unchanged.
The preview worker currently polls one screen frame every25ms, a40Hz delivery
ceiling. Faster presentation delivery is required and has not yet been implemented.

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

## Potential independent parallel piece

PICA texture conversion can live alone in
`runtime/port/browser/PicaTextureConversion.mjs`, without touching Root's bridge,
renderer or build files. No second agent has been started or assigned by Root.
Estimated critical-path saving:2–4hours if completed during bridge/shader work.

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
