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

The preserved native/Wasm public `ComputeFragmentsColors` objects were also
executed on 1,048,576 synthetic directional-diffuse inputs. Explicit native-order
normalized quaternion and view input digest agrees, `2e68b3491f53a8aa`.
Seven color cases differ; native output digest is `88d694778b54a241`, Wasm output
is `be7a853dabbb5e47`. Outputs use the unchanged lighting objects. Receipt is
`575c229c475903f47b1d19a25783cda812b5d77d7b0d28f293fe650d7606a40c`.
The fixture required the documented BitField Assign API, extraction of the
retired Wasm object from its preserved archive, and the original common/Boost
logging link dependencies. Failed attempts and the final six passing commands
remain local. This is a synthetic residual lighting difference, not attribution
for the movie's remaining pixels or complete lighting-mode coverage.

The first server closed after exports. The public combined depth/audio/fog
build `3d95c0e0` completed the same movie in T3. It corrects one former pixel;
21 former pixels remain, with no new different pixels. HID/audio timing, PCM,
bottom RGBA/framebuffer and CPU counts still agree. Final Canvas identity,
all source-packet identities and all stereo consumption counts pass. Top RGBA
is `5456cf28ca955265ce7d3d0d1ad585f3b06c87f697d4f88b1480d4b50bb2ab24`.
Comparison is `2283d06974582f881c50ca8712add0e4b14a51068a6150769f03ddc70882624c`.
Warm 8,999 intervals take 2,158.828 seconds, 4.1685 presentations/second.
A shared-host snapshot records 10 cores, load 14.14 and 1711.94 MiB swap used.
No speed change is attributed to arithmetic. World 1-1 was visibly loaded,
and the unchanged movie completed its bridge endpoint. No goal credit.

Read-only inspection decodes 51,972 native-window PICA list payloads, including
lighting/fog enabled at draw commands. The prefix is unobserved and 2,100
interrupt-stop conditions are unknown, so these are decoded command counts,
not complete execution counts or actual pixel attribution.

The same read-only window inspection observes procedural textures disabled at
every decoded draw trigger, receipt
`9d1e9f8fb29d67e9c56acbe4e4819b3b43611e03670ce20c8a73d72449abce91`.
Its unknown-prefix/interrupt limits remain; no procedural-texture patch is added.

The depth browser/server closed after exports. A separate private candidate
adds only the isolated quaternion-normalization order. All three build commands
pass, with unaffected archive-member and protected-input preservation. Wasm is
152,434,842 bytes, SHA-256
`063ccbafacdcbbeaa7a606cd3d052c70bd7b294e0ac3cef3e2686b0b70d3ee7c`.
Its same-movie replay completed. Top RGBA is unchanged from the depth candidate,
with the same 21 pixels differing. All other comparisons and CPU counts pass.
Comparison SHA-256 is
`dea65273b67a824a82510fbf5dce385fb5ac7daf57f4fb82acb9cf9e1fd1ab6b`.
Warm 8,999 intervals take 1,992.011 seconds, 4.5175 presentations/second.
The normalization browser/server closed after exports.

An isolated lighting reduction candidate decreases the synthetic different-color
cases from seven to five, fixing four old cases and introducing two. It is not
used in a browser. Adding native-object-order quaternion cross products and
rotation closes all 1,048,576 synthetic directional-diffuse color cases. Both
targets produce `88d694778b54a241`. Receipt SHA-256 is
`2b14cbfa14a16e3290d02afa9d49eb798d19d4cf1a2b6a9970ff68361b83e682`;
patch SHA-256 is
`98992caa67af1e05738777243573ccc8fd669987d8192a53511300e51058d7e1`.
Native reduction sites 0x4d8/0x4dc/0x4fc/0x500/0xa54/0xa60/0xa9c/0xaa4 and
cross-product sites 0x1e8/0x1f0/0x1f8/0x218/0x220/0x228 establish the orders.
LUT and distance-attenuation arithmetic and broad lighting-mode equality remain
outside this fixture's coverage.

A separate broader 1,048,576-case fixture exercises all eight valid lighting
configurations, six LUT input selections and scales, one/two lights, bump modes,
shadows and attenuation. Normalized quaternion/view, register and texture input
digest agrees, `01ad5a7a794ac969`. Preserved native output is `afbb74156b56cd17`.
The original Wasm object differs in 292 cases, the rotation/reduction candidate
in 40. Fixture receipt SHA-256 is
`218b6dd23c7933cde5b11d51f50534eee63abfe6d860317cc9714d0bc7b6fb21`.
Native sites 0x640/0x6d0/0x1b98 establish distance and LUT interpolation FMA.
Adding only those two expression orders closes every broader fixture color case,
receipt SHA-256
`65b152190e576962d14ae30c2110e26df22b3e926bbcf3a49c8af9766b2b91d1`.
Finite synthetic coverage does not establish actual pixel attribution or all
parameter combinations.
Its separate full module builds in three passing commands with unrelated-member
and protected-input preservation. Wasm is 152,435,097 bytes, SHA-256
`a099324c477fdd9ac07773a556465cf2f205fe2e08d8584b978fc8c74f8bc8bb`.
It now runs the unchanged movie/profile in T3 tab `tab_1`, through port 8770.
Audio was enabled at startup; the first post-enable sampled renderer frame is 14.
One browser replay is active. No pre-first-frame playback claim for this run.

A private full-module candidate adds the earlier rotation/reduction source to the sealed
normalization module. Three commands pass, preserving every unrelated archive
member and protected input. Wasm is 152,435,067 bytes, SHA-256
`d98dc22cccc47eafce2c22ab603c38df7c0a219e2ce4954d666344bde0f3e882`.
Its unchanged-movie replay completed. Top RGBA remains `5456cf28`, the same
21 differing pixels. Full PCM/HID/audio timing/bottom RGBA/framebuffer and CPU
counts pass, as do Canvas identity and all source-packet consumption checks.
Comparison SHA-256 is
`06fea916a8859d8e281f33a21fe2c9ee5bf144b5f45790b87584c94f631e4387`.
Warm 8,999 intervals take 2,047.338 seconds, 4.3955 presentations/second.
World 1-1 and the original bridge endpoint were visibly observed. Its browser
and identity-verified server closed after exports. Both candidate manifests'
2,803 protected inputs still agree. Actual pixel operands, first differing
rendered frame, playable speed, goal and Section 7 remain unverified.
Submission stays held until the gap closes.

Private evidence resides under this worktree's ignored `build/` directories:
`graphics_arithmetic_resume`, `graphics_depth_resume`, `graphics_normalization_resume`,
`graphics_lighting_resume`,
`browser_arithmetic_module_candidate`, `browser_depth_module_candidate`,
`browser_native_arithmetic_module`, `browser_arithmetic_server_9000`,
`browser_native_arithmetic_server_9000`, `browser_normalization_module_candidate`,
`browser_normalization_server_9000`, `browser_lighting_module_candidate`,
`browser_lighting_server_9000`, `browser_lighting_table_module_candidate`,
and `browser_lighting_table_server_9000`.
No game-derived payload or gameplay image belongs in Git.
