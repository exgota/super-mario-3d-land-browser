# Browser native arithmetic proposal

Root resumed the owner-paused port lane in T3 Code on October 3, 2026. The
pushed gameplay checkpoint is `0d8e5cc53ea947a704370b7f8d618eb6a4e66277`.
All 32 sealed checkpoint records retain their recorded size and SHA-256.
The gameplay family remains held and unsubmitted. Matching claims and newly
translated bytes are zero. Main, ranks, ledger and matching sources are unchanged.

## Observed portability differences

The preserved public Azahar sources agree between the native and WebAssembly
providers. Their recorded compiler recipes differ: Apple clang ARM64 at O3
contracts selected arithmetic, while the WebAssembly O1 provider rounds its
products and additions separately. This is floating-point contraction.

Preserved native audio objects establish fused gain ramps and stereo downmix
that rounds the rear product before fusing the front product. The existing
synthetic audio diagnosis compares 1,048,576 gain-ramp cases and 1,048,576 stereo
cases. Explicit `std::fma` reproduces the native results across both targets.
Actual block operands remain unrecorded.

Fresh graphics cases compare 1,048,576 synthetic inputs. Native ARM64 pairwise
Vec4 float reduction differs from scalar reduction in 311,519 results. The fog
LUT/color expressions differ in 55 integer color results. Explicit native-order
arithmetic agrees across targets. Vec4 float reduction is excluded from this
overlay because the inspected clipping/interpolation paths use generic f24.

The native rasterizer object also establishes rounded second-vertex depth
product, fused first-vertex addition, fused third-vertex addition, division, and
fused depth scale/offset. Separate 1,048,576-case execution finds 233,584 float
differences and 111,060 quantized depth24 differences. Explicit native order
agrees across targets. These synthetic inputs do not identify the actual
22 pixels or their depth decisions.

## Provider overlay and build

`tools/static_recompiler/azahar_reference/azahar_native_arithmetic.patch` changes
only the public audio gain ramps, stereo downmix, fog LUT/color blends, and
barycentric/depth expressions. It preserves conversions, clamps, multiplication
order, source data, the static CPU provider, and the address replacement ABI.
It changes no compiler flags or comparison tolerance.

`tools/static_recompiler/build_browser_arithmetic.py` copies the three selected
provider sources and two archives into a new ignored output. It applies the
overlay, uses the preserved compile recipes, verifies every unaffected archive
member, and reuses the sealed gameplay link recipe. Nine build commands pass.
Fourteen unaffected audio members and 39 unaffected video members preserve.
All declared protected inputs preserve. The public builder's first module is
152,434,828 bytes, SHA-256
`3d95c0e023660f6058935ede30a449501a0c1de843398cbfd4ff5bd2c2522076`.
Linking does not establish replay equality.

Run from the primary repository after sourcing its development environment:

```sh
python build/root_browser_gameplay_session/tools/static_recompiler/build_browser_arithmetic.py \
  build/root_browser_gameplay_session/build/browser_gameplay_module_final \
  build/root_webassembly_platform/build/pinned_platform_final \
  build/root_browser_gameplay_session/build/browser_native_arithmetic_module_repeat \
  --emsdk build/port_tools/emsdk --node /opt/homebrew/bin/node
```

The output must be absent. Its manifest binds the source overlay, complete link
inputs, commands, archive member changes, and module hashes. Baseline providers,
original movie/profile, and failed capture remain intact.

## Actual browser observation and remaining gate

The first 9,000-presentation unchanged-movie replay completed in T3-owned
tab `tab_1`. It used the earlier audio/fog-only candidate, SHA-256 `326eb1de`,
with no depth patch. The native file chooser selected the preserved local dump.
The local server served no dump and generated no game pixels.

At renderer frame 2079, passive streamed-source observation reads left -5128
and right -3628 at sample frame 1,099,311, matching the original capture.
The prior browser value was right -3627. The observer copies the two source
samples before forwarding the unchanged packet to the existing audio player.
The subsequent complete exported PCM agrees at every channel sample.

All 9,884,160 PCM channel samples, HID/audio timing, bottom RGBA/framebuffer,
movie/profile delivery, actual Canvas identity, all source-packet identities,
and consumption of 4,942,080 stereo frames pass. CPU0 executes 4,313,017,982
guest instructions; CPU1 zero; both fallbacks zero. The top RGBA retains the
exact previous mismatching hash, with 22 different pixels. Warm 8,999 intervals
take 2,049.132 seconds, 4.3916 presentations/second. Audio is correct but slow
execution causes underruns; synchronized continuous sound is unverified.

Comparison SHA-256 is
`ec47ee9378da1f77b1867f911a590222d9b781dd9967f543612e8585e64a326e`.
The first local checker invocation refused relative paths; the wrapper now
resolves them before calling the unchanged strict input/movie comparators.
No expected input, tolerance or captured file changed.

An additional isolated quaternion-normalization diagnostic tests 1,048,576
synthetic inputs against native object sites 0x865c/0x8664/0x866c. Native original
and explicit native-order normalized-result digests both equal
`d5634ba0901e84d1`; Wasm original equals `51badfcf0b02362b`, while explicit
native order agrees with native. Separate rounding changes 129,684 lengths and
467,452 normalized components. This expression is outside the current patch;
actual differing-pixel operands and lighting activation remain unverified.
Two initial tiny native builds selected incompatible command-line SDK stubs.
The successful run pins the Xcode compiler/linker and MacOSX26.5 SDK already
recorded in the audio diagnostic. No machine settings changed.

The first server closed after exports. The public combined depth/audio/fog
build `3d95c0e0` now runs the same movie in the same T3-owned tab, through a new
server on port 8767. Only one browser replay is active. The first differing
rendered frame, actual pixel operands, playable speed, goal and Section 7 remain
unverified. Submission stays held until the comparison gap closes.

Private evidence resides under this worktree's ignored `build/` directories:
`graphics_arithmetic_resume`, `graphics_depth_resume`, `graphics_normalization_resume`,
`browser_arithmetic_module_candidate`, `browser_depth_module_candidate`,
`browser_native_arithmetic_module`, and `browser_arithmetic_server_9000`.
No game-derived payload or gameplay image belongs in Git.
