# Portable floating-point evidence

Root/portable-floating-point freezes integrator base 9d5d6885d35de24604832527c90a68b493ac30d3. It started at cd5f5515 and fast-forwarded its own unpublished branch to include the accepted browser binding. Root owns runtime/port arithmetic, its build integration and numerical verification tools. One helper supplied NativeFloatingPoint.c and isolated wasm verification. Pro retains class-layout ownership. No matching build input, rank, ledger or Factory file changes. No symbol claims or new exact credit.

## Change and provenance

The existing bit-oriented API now calls integer-only Berkeley SoftFloat 3e through its ARM-VFPv2 specialization. It maps explicit guest rounding, DN/FZ and cumulative exceptions, uses before-rounding tininess and separately rounded multiply-accumulate operations. It saves and restores all four C11 thread-local software state variables. Unsupported arithmetic exception delivery still refuses execution; raw move/absolute/negate retain their existing bit behavior.

The [author's archive](https://www.jhauser.us/arithmetic/SoftFloat-3e.zip) is 729637 bytes, SHA256 21130ce885d35c1fe73fc1e1bf2244178167e05c6747cad5f450cc991714c746. Its BSD three-clause license was read. All downloaded source stays unmodified under ignored build/port_tools. The public manifest seals 55 dependency C files, seven headers and COPYING.txt. Configuration rejects modified or missing official source. The target copies the verified notice beside its archive for binary distribution.

The generic platform uses C11, SOFTFLOAT_FAST_INT64=1, THREAD_LOCAL=_Thread_local and INLINE_LEVEL=0. Emscripten requires wasm32/pthreads. The native builder and instrument_timing.py link this archive instead of compiling the former host arithmetic implementation. The latter checks both copied adapter and header hashes against the linked target. Existing generated conversions and comparisons keep their original flags and remain outside this helper's numerical certification. Matching ARMCC inputs and global flags are unchanged.

## Original-instruction and native integration checks

The actual public native target passes 88192 direct result/FPSCR comparisons with selected original instructions in the local hashed EU executable, executed through Unicorn 2.1.4. Four rounding modes, DN/FZ, clear/seeded cumulative flags, NaNs, overflow, subnormal input/output, before-rounding tininess and separately rounded multiply cases are included. No mismatch.

A fresh compilation of all sealed timed generated C sources links the final arithmetic archive. It takes 685.564 seconds on this run and preserves 47 generated source seals. The library SHA256 is de893bdf0777c9b27ed3f3153d35c958b65888a6fcfc3114d8da1c133979f1be. Its conservative linked coverage remains 15874 functions / 2437712 unique function instruction bytes, excluding literal pools, fallback sites and the priority replacement. New wasm game-byte credit is zero.

The unchanged public verify_floating_point_execution.py passes 88064 generated-entry checks: all 32 floating-register words, FPSCR, all integer registers/next PC and NZCVQ agree with original one-instruction execution. verify_execution.py passes 188695 startup instructions to SVC 0x21 with 1384448 writable bytes equal, 4105 priority-source replacement cases and 2048 bounded integer cases. No interpreter runs.

The public target passes 1672 software-state preservation and 20000 concurrent RN/RZ checks, with four distinct TLS variables. The frozen helper also passes 36 native null/trap/selector refusals, 24 raw-trap controls and two missing/empty TLS compile refusals. The final native arithmetic library imports only stderr, TLS bootstrap, abort, bzero and fprintf. NativeFloatingPoint.c has zero calls or includes for the removed host rounding/exception/math APIs.

## Actual wasm target

The public verify_floating_point_webassembly.py compiles the actual arithmetic CMake target at Release optimization and runs it in Node 24.19.0 using pinned Emscripten 6.0.10. The runner reads original instruction words privately, generates independent Unicorn results and independently decodes the returned binary stream. All 88192 cases / 1058304 result-and-FPSCR bytes match. The same 1672 state and 20000 concurrent checks pass. See tools/static_recompiler/FLOATING_POINT.md for reproducible public commands.

Public-run archive SHA256 02d6cbe88d275154172a3c594464f298e3b678f46d7044a9b96eea9580695ea0; module SHA256 037d93bca1df883fdda69c030e9ac3e73f92446db9293c0f7dc0b1eb4451082d; result-stream SHA256 daafe6e6962d910e51470a5c7c5ae61a1daadb32587e7ae3bbf893f155eed213. This is a synthetic arithmetic API test. It executes no translated guest entries.

Independent helper verification additionally passes 36 actual wasm null/trap/selector refusals, six raw-trap completion controls and four first-divergence/malformed-record controls. The four software variables carry actual TLS symbols and .tbss/.tdata segments. The arithmetic archive has no numerical floating-point instructions or host math/rounding imports. A separate INLINE_LEVEL=5 proposal returns byte-identical results. Parent verifies all 261 frozen scratch paths and the final source hashes in receipt b51994b481e0ac0a41a307fe54e2eeeaeed1fec6e43048bbd13815b1f72ba25d.

After the license-copy/header documentation change, all 56 native arithmetic object hashes preserve. Darwin archive packaging dates differ; the final timed library links the final archive. The wasm archive, objects and linked helper module preserve exactly.

## Untouched Azahar replay

Two fresh native replays use the same recorded 360-presentation movie, initial console/save snapshot and raw diagnostic state plan as the unchanged stock reference. Both pass compare_state_observation.py, including its entire movie/platform comparison. They preserve 1802 HID polls, 4583 GPU events/ticks, 545 PICA lists / 8642912 bytes, 691200 RGBA bytes, 345600 framebuffer bytes, 1350 audio blocks / 216000 stereo sample frames / 864000 PCM bytes and 12 raw RAM observations at renderer frames 120, 240 and 461.

Each CPU-0 run executes 393977876 guest instructions; CPU 1 executes zero. Both report zero interpreter/JIT fallback. Elapsed native windows are 68.203 and 51.696 seconds. These are observed run times, not controlled performance measurements. State-event SHA256 preserves a22ddcf25ec79442c153f58c4057a1d3e33d7959fbe7c09d6fd08fe7c25c1da8. Comparison SHAs are 2236fa26d8934e81be3a6cc4e697282d85c0c607b72eead10228d6d0adbef52a and 26118fd96707083ff2263cfcf736324bd2b663f4511ff183cf7b46ad2e80e196. The sealed host source matches this branch. Neither the oracle nor its executable is rebuilt or modified for these replays.

## Receipts and limits

Local final receipt: build/root_portable_floating_point/build/final_verification.json. Numerical public receipt: build/root_portable_floating_point/build/public_webassembly_verification/result.json. Generated-entry reports and final library seals: build/root_portable_floating_point/build/portable_timed_translation. Native comparison reports: build/root_portable_floating_point/build/native_replay_1_comparison.json and native_replay_2_comparison.json. Downloaded tools, generated game code, fixtures, movies, state, captures and binaries remain ignored.

This family recompiles the sealed timed sources and verifies the linked result. It does not rerun full ARM-to-C discovery or rebuild the timing sidecar. Their unchanged source/library/sidecar seals are checked before use.

All 13 single-precision selectors have selected original-instruction probes. Original double probes cover add/subtract/multiply/divide/move; other double selectors need original probes. Exception delivery, conversions/comparison exceptions and short-vector sequencing remain unsupported or unverified. Browser platform linkage, an actual browser frame and World 1-1 entry/goal remain open. Diagnostic raw state equality does not establish Section 7 player/camera/timer/coin semantics. Other rank-O functions still need portable address-keyed ABI adapters.

Final public implementation and verification source hashes:

- `runtime/port/NativeFloatingPoint.c`: `9d9269ad002c736513980178be4cbe4a001a429cf4582efafef2890915c8b203`.
- `runtime/port/NativeFloatingPoint.h`: `8f2020c3ba1e8c380e9c0c0dfd52e3cb99322d3b49dba497a7d4b118b7408194`.
- `runtime/port/floating_point/CMakeLists.txt`: `fc9eaf670294f4447efb490d7bc824d2c48e165479d99d66312c211fc4db5220`.
- `runtime/port/floating_point/platform.h`: `249f7d134297301e1f11d0e54091b7989c48dc35b9148c379e7db32139200adf`.
- `runtime/port/floating_point/softfloat_sources.json`: `efe25245f2fdb81e20a77ce1b36cffc582ee2f8c573cf4e0f165d24d2f4057a6`.
- `tools/static_recompiler/build_floating_point.py`: `7c9f6cd9cdbffbafb0c442c3605a9b7d03498e8272677ba5edb4a855f60ec521`.
- `tools/static_recompiler/instrument_timing.py`: `8783fe78536fde8916b4037fc91759748686c78ca62de15072629749a4c6836d`.
- `tools/static_recompiler/verify_floating_point_webassembly.py`: `478fcd0e16820d642528d2c048a8462aeae6c4da02f327e411de2eaa0f5dac9e`.
- `tools/static_recompiler/verify_floating_point_webassembly.c`: `5397d3c12597bf88059b20ebf72bdffe7508fbd821fbfb16f3c5ab90c53f579e`.
- `tools/static_recompiler/verify_floating_point_environment.c`: `fbd09ea38ceb314450eba895ba0b934b5a5b54fb05818d24861d4674559d5107`.
