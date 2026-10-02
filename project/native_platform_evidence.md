# Native static platform checkpoint evidence

This family owns the port CPU adapter, resumable generator correction, guest arithmetic integration, generation-time timing and strict GPU comparator. It changes no Game/lib source, matching configuration, map cells, ledger, Factory definition or game input. All translated code, binaries, reference movies/state and captures remain ignored. It adds no exact-function claims or milestone completion.

The public dependency and owner-input provenance is unchanged from the accepted static recompiler and Azahar packages. Reproduction is documented in tools/static_recompiler/NATIVE_PLATFORM.md. All three source-only Azahar patches apply in order to a temporary index initialized from the pinned clean revision; build/native_platform_patch_report.json records this check. No source mutation of the downloaded checkout was needed for that check.

The previous map-Pool discovery treated placement hints as relocation ranges and omitted executable startup fallthrough at 0x001007C8. This family follows binary flow and literal references instead, with no map edits. Every instruction gets a resumable generated entry. Linked coverage is 15,874 generated functions and 2,437,712 unique map function instruction bytes, excluding literal pools, fallback sites and the native source replacement. There are 29 untranslated instruction sites; the observed native startup and GPU run execute none.

The final timed library is e0f53fa6edd3bb52a5f0a60f39f2d18b7efa6b2655c6d181782ae5a64dcc4fe8. Its sealed manifest and execution_report.json are in build/floating_point_timed_native. Original and native startup each execute 188,695 instructions; registers/flags, the first supervisor boundary and 1,384,448 writable bytes agree. 64 bounded integer samples across 2,048 cases and all 4,105 priority replacement cases pass. The old coarse budget count of 251,587 was not a dynamic instruction count.

The same final library passes 86,016 original arithmetic and 2,048 raw-bit generated-entry floating-point cases under Unicorn ARM11MPCore 2.1.4. Every case compares all single-register words, FPSCR, integer registers, next PC and flags. Result: zero mismatches, recorded in floating_point_execution_report.json. The separately accepted helper evidence describes supported arithmetic, short-vector oracle limits and conversion/exception gaps.

Observed native platform execution uses the same pinned HLE platform, software GPU, movie and pre-CPU user snapshot as the stable stock reference. The final traced run completes in approximately 2.07 seconds, executes 226,970,847 guest instructions and reports zero interpreter/JIT fallback. It reaches the first top-screen SetBufferSwap, which is a command-stream boundary, not a visibly presented frame. The host executable SHA-256 is 6631019ddd2d477f62ae0ddea71cac13c494f3b04c704ff8e7e38a4300fe3169. Runtime source seals below describe this observed build.

The stable stock reference has 528 raw events, 348 GSP commands, eight PICA lists totaling 79,936 bytes, 68 direct register writes, 102 VBlanks and one top-screen swap. Its event hash is ed7a39625518e559be6c0bc8f5a50d05f088161bc9256b6a8a169eab114f9859. Two clean replays match exactly. The final native run has identical command metadata/counts and no Movie errors. Strict comparison fails: 16 payload words and 473 event tick values differ. Native event hash is 5c406a6b20c1be61f70ab3814ad4291559ef807de65ac4b7da3379fa75da1228. The first two PICA payloads match exactly; the remaining six contain the viewport differences. Native runs with and without the optional write observer agree byte for byte.

Ignored primary evidence: build/root_port_reference/static_execution_3_receipt.json, static_execution_3_memory.jsonl and fixed_io_replay_report.json. Worktree evidence: build/native_comparison_3.json and build/native_platform_source_receipt.json. The comparator rejects timing or payload differences; its metadata-only diagnostic does not relax acceptance.

Filtered writes identify native inverse viewport values at guest addresses 0x0038D03C, 0x0038D1C4 and 0x0038D23C with FPSCR 0x63000010. The nearest-rounded values are two integer units above the reference's packed values. Controlled original/native divide and integer-pack sequences agree for dimensions 240, 320 and 400 under all four modes. Actual stock state is still being investigated. No arithmetic adjustment or reference normalization is included.

Timing advances per instruction and defers the supervisor instruction cost, whereas stock Dynarmic charges basic blocks and defers the whole current block across HLE supervisor handling. Exact scheduling remains unverified. Conversions, comparison exceptions, short-vector sequencing, dynamic guest code and single stepping remain limits. Only the clean exact-source priority function is registered for address replacement; main's other rank-O adapters remain outstanding.

Final source seals:

- `runtime/port/AzaharStaticExecution.cpp`: `985a9161cd40a13a47ed9fc810e00034f0dbb7e7744a4c8223f393d849476f68`
- `runtime/port/StaticArmBackend.cpp`: `e17c1bdf290952b20c137349d68a1fa77be69133b7bd7bd9cf4e35060f967bfe`
- `runtime/port/StaticArmBackend.h`: `b97b112acb894749b19dd0d2d470219a164840a0998d1b72544faab30afc8d84`
- `runtime/port/GuestMemoryTrace.cpp`: `a40c50ff902c811cdf950a73afcd2516c4217269074983765e5828450dc9112c`
- `runtime/port/GuestMemoryTrace.h`: `9b9cd3ebc50b68c7f3bca3867de916dfedcdf7018f3730fd137721c62e373d03`
- `runtime/port/NativeTiming.c`: `1d0bdd4039d4e00e72810cfb7888945da48f83089336e37eff97faaca40391f3`
- `runtime/port/NativeTiming.h`: `c79b1f145d1edc54fe83279af8dc4ec8f5b16ea5f19423926c4ce6f22f1dc46e`
- `tools/static_recompiler/src/main.rs`: `01470fb49cabc3573a575e5a23d34038baeb1d8ac1759b5df1a9f2de2c988932`
- `tools/static_recompiler/instrument_timing.py`: `b2bd5a62f65ce0f634992aaacfc2e17886a51bc8bd4afb85e941fef2e1170bc8`
- `tools/static_recompiler/rewrite_floating_point.py`: `c9700077d0e168f1db2549dbcf9ca26c451002568fde75a4248c67ad2b7fc372`
- `tools/static_recompiler/verify_execution.py`: `a9b7c88ab47d5791d6e4a797e0742e2c86b02a18f533567585e4a5a34607af89`
- `tools/static_recompiler/compare_gpu_capture.py`: `06e6b0470a6bbbb7977c74afdcf6387bb3bd91f7a1959bb969528e21aeb25696`
- `tools/static_recompiler/azahar_reference/azahar_static_execution.patch`: `8ccdc21246c3cac1a8521eb5ffe910c015261754a98ecd164249b1e5c7ca9b6a`

Correction recorded 2026-10-02: the independent word audit found 16 differing PICA words in this historical run. The earlier count of 18 was incorrect. The subsequent rounding and block-scheduling families repair those words and timestamps; this original checkpoint remains a failed comparison.
