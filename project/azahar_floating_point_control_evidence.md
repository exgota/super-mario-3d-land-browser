# Azahar ARM64 floating-point control correction evidence

Root owns this source-only reference-host correction on root/azahar-floating-point-control. It changes no matching build input, ranks, ledger, Factory source, original executable or native translated arithmetic. It adds no exact-function claim. Pinned public Dynarmic e77b1ba0b7da7cbe93021b01a663acfe7c4dd516 runs within pinned Azahar 662d412123305a9f4be94dd3dc73ddf91a18c55e. The modified file has public 0BSD provenance; aggregate Azahar distribution remains GPL.

The original host changes guest FPSCR after VMSR but keeps host FPCR at its Run-entry rounding mode. The source patch updates that host register with the already-masked guest value. A clean temporary submodule index accepts it. Source-only reproduction, licensing, bounded checks and capture comparison are documented in tools/static_recompiler/azahar_reference/FLOATING_POINT_CONTROL.md.

The published C++ probe reads all guest instruction words from the local approved executable and contains no inline assembly or original guest opcodes. The parent Python runner verifies the approved SHA-256 and executes the same three register-only instructions under independent Unicorn ARM11MPCore 2.1.4. Across 96 cases, the original host has 22 continuous-run result mismatches, no split-run or guest-FPSCR differences, and 36 stale host-control values. The corrected host has zero result, status or host-control differences and zero interpreter fallback. Parent report: build/floating_point_control_report.json in this worktree. Exact compiled probe arguments and hashes: primary build/root_port_viewport/floating_point_control_verification_commands.json.

The candidate host was linked from an independently compiled corrected emission object in an ignored copied archive. Shared upstream source/archive and the settled capture executable are preserved. Corrected capture executable SHA-256 is 5257c550a77eea2688a2873f304074417e5d3382b0799cc1d7ad39c473ce96d1. A separate complete fresh build is still unverified.

Two corrected reference replays use the same owned dump, historical movie and initial user snapshot in absent output directories. They match all 528 raw events/ticks and eight PICA lists totaling 79,936 bytes, with no Movie errors. Event SHA-256 stays ed7a39625518e559be6c0bc8f5a50d05f088161bc9256b6a8a169eab114f9859. Exactly 16 historical PICA words change. Every corrected byte and all command metadata match the unchanged FP-integrated native run. The old 18-word estimate was wrong and is superseded by the full uint32 audit. All original artifacts remain untouched.

Detailed reports: primary build/root_port_viewport/corrected_capture_analysis.json and old native worktree build/updated_floating_point_control_replay_comparison.json (strict pass), updated_floating_point_control_native_comparison.json (strict fail from timing), and updated_floating_point_control_historical_comparison.json (expected 16-word difference). Exactly 473 native event ticks still differ, from -2390 through +10402; first difference is event 5, VBlank, delta -1. No comparator or acceptance rule was weakened. Milestone 1 remains in progress. Rendering and gameplay are unverified.

Final source seals are recorded in floating_point_control_provenance.json:

- patch_sha256: `f1c34a0c1bc64b756be9177ba0cd8b4ab671ac80f8af967fb3c797f0871c95f0`
- original_emitter_sha256: `15e1ec18fc34c613557d2cba54677a7a8a8d114afc0607f0098a4653fe79eb53`
- corrected_emitter_sha256: `2638a077165b689268d10a1ae72549857a24585920d10704d2a41e9bd4e08395`
- probe_source_sha256: `92c5c7abc8c49ab5d037c8d66011946818fa7c349fb87004baa162086a67ad0d`
- verifier_sha256: `018d8b471c790c29b0f1ebecd9e383f8c55cf399d63cb940e0add5e25300dfdd`
