# Rendered-frame capture evidence

Root owns `root/rendered-frame-capture`, based on main `b93bd187bc0a7e7bd5e91294f80e9eaf5a122ca1`. The isolated family is the optional presentation observer for the pinned public Azahar derivative and its strict comparator. No `Game`, `lib`, build configuration, map, rank or ledger input changes. No function claims or matching credit. Source-only files are submitted; generated code, libraries, movies, snapshots, raw GPU data and images remain ignored.

The observer is supplied under the existing Azahar patch's GPL-2.0-or-later terms. It reads the existing software renderer immediately after natural VBlank presentation. It does not alter rendering or guest state. The default first-swap boundary is preserved. Selection 1 through 3600 counts presentations after the first top-screen submission, with no new public capture format replacing the original format.

Final public source SHA-256:

| File | SHA-256 |
| --- | --- |
| tools/static_recompiler/azahar_reference/azahar_render_capture.patch | 7d265e2cfba1dff071ae34d13e28bf76f8f414c6ecebe837f7d361d41ab8e23c |
| tools/static_recompiler/compare_rendered_capture.py | 49ef688c02c8f0b4f8a619213054f6d72348a31433b09e3c88027984de344c3f |

The public source patch sequence applies cleanly in an independent temporary Git index at Azahar `662d412123305a9f4be94dd3dc73ddf91a18c55e`. Capture, deterministic I/O, static execution and presentation patches produce tree `553d4741927c6eaa3b97f3593cafa27570ed978a`. `build/render_patch_sequence_verification.json` records that check. The separate accepted Dynarmic ARM64 FPCR correction is applied at `e77b1ba0b7da7cbe93021b01a663acfe7c4dd516`. Both headless targets built successfully with one compile job. Existing ignored source/dependencies were used; a full fresh download/rebuild has not been run for this branch.

Observed final executable SHA-256: stock `50a532d9439badb5a181d846d350d253fc6f2ee019ae0461774a5253a0234efb`, native `1ab98bccd4e77e984e836533a243571581fda3f0d3efff8d0da0d1d55e5eb39b`. The unchanged sealed native library is `e79538983be80ccd0b730fc3985162a6fef299f94bab7bcb8a3a1905541e6131`; schedule sidecar is `fbe0bdfdf8479deacac0c113c8d975b0094abe9e2eb8cfa7ce5cbcf9d6a9c10c`. Both derive from the accepted scheduler branch, with 2,437,712 linked original instruction bytes. Native accepted-source binding coverage is still incomplete beyond the priority function.

Local receipts below are relative to `build/root_rendered_frame_capture/`, unless a primary-checkout path is stated. Capture inputs and outputs are under primary `build/root_port_rendering/`. Their movie and snapshot are paired per fresh reference recording. Original owner dump remains untouched; the approved ignored copy and executable identities are those in BUILD.md.

With selection unset, `reference_default_boundary` passes the existing strict first-swap comparator against the historical corrected reference. `build/default_boundary_preservation.json` records untouched raw event/timing and PICA equality. Selection 1 passes native/stock/repeat comparisons but shows black screens. The 95,999 of 96,000 black top pixels and 76,799 of 76,800 black bottom pixels do not establish visible rendering. One corner pixel on each comes from the pinned stock decoder's existing out-of-extent read. Both raw first-presentation framebuffers are all zero. Those captures remain preserved.

Selection 60 produces a visible top-screen title logo during a dark fade. Root inspected the native and stock PNGs locally. The top has 742 RGB colors, dominant share 0.60953125, and disabled LCD fill. The bottom remains effectively a single background color with the stock decoder corner artifact. The actual screen metadata remains exact, including raw RGB565 framebuffer bytes.

`reference_presentation_60`, its stock replay, and two native replays all match. Final strict reports are `build/presentation_60_native_comparison.json`, `build/presentation_60_stock_repeat_comparison.json` and `build/presentation_60_native_repeat_comparison.json`. They require all 1,430 untouched event bytes and ticks, 95 PICA lists/765,728 bytes, both screen metadata records, 691,200 RGBA bytes and 345,600 active raw framebuffer bytes to agree, completed software-presentation outcomes, and no movie errors. Raw event SHA-256 is `4a6e8037214e8522da26353c040c015e7a10e8622dabbaa01f89f89f797591e3`. Presentation index 59 occurs at renderer frame 161, VBlank index 160, tick 1,647,653,201; first submission tick is 1,383,266,184. The same-input pair, not that absolute tick, is the reproducible invariant.

Native stderr receipts record CPU0 343,255,755 guest instructions and CPU1 zero, with zero interpreter/JIT fallbacks in both runs. The runs took 20.204 and 21.242 seconds. Stock record/replay took 4.564 and 5.107 seconds. Timing includes local host execution and is not a real-time performance claim. The source observer and comparator do not execute an alternative guest interpreter.

The helper's independent comparator audit passes 28 controls covering pixel, framebuffer, PICA, tick and metadata divergence; incomplete/exception outcomes; movie errors; JSON/screen/extent/format/path/symlink refusals; independent hardware height; alpha-only diagnostics; output protection; and independent PNG chunk/CRC/filter decoding of eight images. `build/rendered_capture_verification/synthetic_verification_report.json` seals its final source hash. Synthetic controls validate the comparator, not game rendering. Root's actual native/stock comparison establishes the bounded visible scene.

[RENDERING.md](../tools/static_recompiler/RENDERING.md) supplies commands to rebuild, record, replay, compare and inspect. Port milestone 2 has bounded native software-rendering evidence. Browser/WebGPU presentation, input, audio output, World 1-1 state replay, stereo, later-scene timing and the complete accepted-source ABI registry remain open. No matching or level-completion claim follows.
