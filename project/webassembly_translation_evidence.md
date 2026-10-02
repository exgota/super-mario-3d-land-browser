# WebAssembly translation evidence

Root/webassembly-translation freezes branch base c6fd1ec793ff52820710dac7231bf7199ceb8d2e. Root owns runtime/port/webassembly_translation and the associated build/verification tools. The source family contains no matching build input, Game/lib/config/map/rank/ledger/Factory change, symbol claim or exact credit. One helper investigates the reduced public platform only in ignored scratch. Pro retains guest class-layout ownership.

## Build and replacement gates

The new Emscripten wasm32 target compiles all 37 sealed timed generated C units, entries.c, NativeTiming.c and replacement.cpp. It declares the accepted integer guest arithmetic target as a consumer link dependency. The final executable links both actual archives. C/C++ standards are C11/C++20. Generated instructions use module-local O1 and disabled floating-point contraction, matching the verified native optimization level. Pthreads propagate to consumers. Matching ARMCC flags stay unchanged.

The builder verifies the sealed native library and all 47 source files, accepted arithmetic/timing implementation and header identities, the reviewed replacement registry and the registered source body. It freezes integrator main 234d719a0a73219d5dd6ad7d345a1ac86cfe6dea and requires the priority symbol at 0x0010766C to have rank O and the exact committed source hash. Full rank-O source adapters beyond this function remain incomplete.

The initial and final fresh archive builds produce the same SHA256 aaf2b83bed6f83bf933678afef510fdec13c7c9802550f5a7ae9d8d7c5a49db6. The final builder includes frozen-main registration checks. Its own receipt records archive-only scope, runtime_verified false and zero execution credit. Final source manifest: build/root_webassembly_translation/build/translated_archive_final/build_manifest.json. Native input library SHA256: de893bdf0777c9b27ed3f3153d35c958b65888a6fcfc3114d8da1c133979f1be.

## Actual linked entry execution

The public verify_webassembly_translation.py links an actual Node wasm32 executable against that archive and the actual accepted arithmetic target. It calls the generated entries by guest address, reads back all 642664 actual linked addresses, requires nonnull strictly ordered function pointers and compares every address with the sealed entries.c declaration. Priority lookup must equal the actual source-override function pointer. The host interface asserts 120-byte Context and 8-byte Entry. These are compiler-generated host layouts, not ARMCC guest class layouts.

All 92297 original-execution cases pass with zero differences: 88192 selected floating-instruction cases and 4105 actual priority source replacement cases. Floating cases compare all 16 integer registers/next PC, NZCVQ/GE/Thumb, all 32 single-register words and FPSCR. Four rounding modes, DN/FZ, seeded cumulative flags and separately rounded multiply cases are included. The priority comparison follows AAPCS, excluding caller-saved integer registers and arithmetic flags while checking return value, callee-saved registers, SP, return PC and preserved floating state. No memory service or CPU interpreter callback is allowed.

Original instruction words come only from the local approved EU executable, SHA256 e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64. Unicorn 2.1.4 executes the original ARM11 samples independently. Requests, reference streams, actual results and all generated code remain ignored. The independent decoder compares 18459400 result bytes and records the first difference.

Actual table address-stream SHA256 1ffeaeff8881f18befb106a12e96cb272d393d6bb0ec17d3447d4313af46bd54. Actual result-stream SHA256 464fe960cbdd8b7bd5498fd2a0ade5a79f0810303dc53d4ef860400cf217e30a. Module SHA256 d489744d2c3b36d46bc7fbd24467940b0274cf45b4045101029a23c25cdcf788, 140033657 bytes. Arithmetic archive SHA256 02d6cbe88d275154172a3c594464f298e3b678f46d7044a9b96eea9580695ea0. Actual receipt: build/root_webassembly_translation/build/translation_verification_final/result.json.

Linking the complete table retains 15874 translated functions / 2437712 conservative original function instruction bytes, excluding literal pools, fallback sites and the priority replacement. This is linked wasm coverage. The selected cases above are the executed scope. Native coverage and exact coverage gain no bytes. The dashboard retains its existing native recompiled_bytes total.

## Verification controls

The first runner compilation refused an unused static vfp_vector helper in the sealed generated header. A diagnostic exemption surrounds only that header include; Wall/Wextra/Werror remain enabled for the actual runner. No generated header or global compiler flag changes. The failure stays in build/root_webassembly_translation/build/translation_verification/command_1.log.

The final link warns that pthread memory growth may slow non-wasm code. Memory growth remains enabled for the real shared-memory target; this is no browser performance claim.

Six actual controls pass. A partial request refuses with status 6; a missing
entry refuses with status 7. Forced memory and interpreter callbacks abort with
status 1 and no success output. An existing result file refuses with status 4
and retains its exact SHA256. Altering one reference word causes the independent
decoder to report the first difference at case 0. The original successful
result stream remains unchanged. Receipt and failure logs:
build/root_webassembly_translation/build/translation_refusal_controls/result.json.

## Reproduction and limits

tools/static_recompiler/WEBASSEMBLY.md gives public cross-compile and verification commands. The toolchain is the previously accepted official Emscripten 6.0.10/Node 24.19.0 closure. Official SoftFloat source/header/license pins and its BSD notice are inherited from the accepted arithmetic target. Neither dependency source nor any game payload is committed.

This family recompiles already sealed timed C sources. It does not repeat original ARM-to-C discovery or rebuild the timing sidecar. The native preserved module and prior complete recorded-menu Azahar replays remain untouched. No native replay is rerun for this wasm-only family. Matching build inputs are unchanged.

Final audit: build/root_webassembly_translation/build/final_verification.json
checks all 15 public implementation/helper seals, all 47 translated source
seals, all 40 translation compile commands and six controls. Both archive builds
agree. Protected matching/oracle trees have zero branch changes. Verification
receipt SHA256 96827c58cd797aab90a028576a770bd84820a8f8aa018e14789a8e06dae8550b.

No browser host/page/frame, translated startup replay, full platform link, complete rank-O source registry, World 1-1 entry/goal or Section 7 semantic gameplay acceptance follows. Original double arithmetic probes still cover add/subtract/multiply/divide/move. Exception delivery, conversions/comparisons and short-vector sequencing remain outside the numerical certification.

Final public implementation and verification source hashes:

- `runtime/port/webassembly_translation/CMakeLists.txt`: `7a905f581e73dbf4ec1a760446b1f8659215412f34d29a05cb1b9d36c3d529ab`.
- `tools/static_recompiler/build_webassembly_translation.py`: `e7ea130048d05a9efc3676ac1a9d49ac1019f174c2e83a14ef75590085c86861`.
- `tools/static_recompiler/verify_webassembly_translation.py`: `d011fec856a7620f575ccf9dc7d9a4baec7f65b31a9de560dc26475e0c342c79`.
- `tools/static_recompiler/verify_webassembly_translation.c`: `0fce65a32ea4354ebdbbaf430ebf5f40d9eca15916fc57bdc5c73c5ee17529c7`.
