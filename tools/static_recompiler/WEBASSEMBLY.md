# Typed static CPU binding and WebAssembly archive

`Port::TranslatedFunctionModule` binds a generated `Entry` table and its timing-callback storage directly. `StaticArmBackend` accepts that descriptor and an explicit block-schedule path. The existing native-library constructor uses the same initialization after loading the five existing exports. Failed native construction closes its library handle. Address lookup, memory callbacks, service handling, guest registers, strict refusals and block scheduling remain unchanged.

The descriptor requires Context ABI 4, timing ABI 2, 1 through 1,048,576 strictly ordered entries, nonnull entry functions and nonnull timing-callback storage. Its storage is caller-owned: the table must remain immutable and valid for the backend lifetime, and the writable callback cell must belong to those generated functions. Validation cannot establish arbitrary pointer extent, ownership or lifetime. Default fields initialize safely to zero/null. The sealed native module has 642,664 entries.

`ROOT_PORT_STATIC_MODULE_ONLY` removes native-loader code at compilation. Its native-path constructor explicitly refuses; its typed constructor remains usable. This does not add runtime ARM decoding, an interpreter or a JIT. The existing address-selected priority replacement remains in the generated module. Full rank-O source ABI binding beyond priority is still incomplete.

## Reproduce the archive

Keep downloaded tools, generated declarations, scheduling metadata, modules and all owned game data ignored. The verified tools are:

- Emscripten SDK repository `e566f7bdcc7735f44037911c24b87a58a3c93145`, official `emscripten-core/emsdk`.
- SDK `6.0.10`, compiler revision `d6c521a7f05449857c76bd99e396895583cf2083`, release package `666337b525e673e769121856d175f6f52b8ead64`, embedded Node `24.19.0`.
- Public Azahar `662d412123305a9f4be94dd3dc73ddf91a18c55e`, Boost submodule `6a85c3100499e886e11c87a5c2109eedacea0a61` and fmt submodule `e424e3f2e607da02742f73db84873b8084fc714c`.

Use the [official SDK installation procedure](https://emscripten.org/docs/getting_started/downloads.html), pinned to the versions above. Install and activate inside ignored `build/port_tools/emsdk`; source its environment only in the build shell. No shell startup file or system installation is required. Initialize the pinned Azahar Boost/fmt submodules. Generate `recomp.h` using the accepted native builder and the owner's locally verified executable, as described in [NATIVE_PLATFORM.md](NATIVE_PLATFORM.md).

From this checkout, with the SDK active:

```sh
emcmake cmake -S runtime/port/webassembly -B build/webassembly_static_cpu \
  -DCMAKE_BUILD_TYPE=Release \
  -DROOT_PORT_AZAHAR_SOURCE_DIRECTORY="$azahar_source_directory" \
  -DROOT_PORT_TRANSLATED_DIRECTORY="$translated_directory" \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build/webassembly_static_cpu --target static_arm_webassembly --parallel 1
"$emsdk_directory/upstream/bin/llvm-readobj" --file-headers \
  build/webassembly_static_cpu/libstatic_arm_webassembly.a
"$emsdk_directory/upstream/bin/llvm-nm" --undefined-only --demangle \
  build/webassembly_static_cpu/libstatic_arm_webassembly.a
```

Set the three directory variables to absolute ignored paths before these commands. The CMake target requires Emscripten wasm32 and the actual pinned public/generated headers. It compiles the real adapter, descriptor, block scheduler and memory tracer with C++20, pthreads, wasm exceptions and disabled floating-point contraction. Consumers inherit pthread/exception link options. It does not import native libraries, native CMake caches, Apple frameworks or Homebrew headers.

All four members must report WASM/wasm32/32bit. No undefined `dlopen`, `dlsym`, `dlclose` or `dlerror` symbol may appear. Core timing, MemorySystem and SVC implementations remain unresolved. Generated guest entries and floating-point implementations are not included in this archive. This artifact adds zero newly linked translated bytes and does not complete milestone 6.

## Native preservation

Apply the ordered source patches in [the reference documentation](azahar_reference/README.md), including input, audio, replay duration, default applets, payload limits and passive observation when using those captures. Use the corrected pinned Dynarmic oracle. The updated `azahar_static_execution.patch` includes the descriptor implementation and an optional directly linked native driver. Configure the existing native Azahar build with:

```sh
cmake -S "$azahar_source_directory" -B "$azahar_build_directory" \
  -DROOT_PORT_SOURCE_DIRECTORY="$port_source_directory" \
  -DROOT_PORT_TRANSLATED_DIRECTORY="$translated_directory" \
  -DROOT_PORT_TRANSLATED_LIBRARY_FILE="$translated_directory/translated.dylib"
cmake --build "$azahar_build_directory" \
  --target azahar_static_execution azahar_compiled_execution --parallel 1
```

On other native systems use the builder's matching shared-library suffix. The optional driver links that native library at build time and passes its typed exported table/revisions/callback cell directly. It is a native validation host, not a browser executable or an all-static platform link. The existing driver loads its library at runtime.

Record a stock movie and pre-CPU user snapshot using [INPUT.md](INPUT.md). Replay that original pair with input/audio capture and the same natural presentation boundary and optional state plan, following [AUDIO.md](AUDIO.md) and [STATE_OBSERVATION.md](azahar_reference/STATE_OBSERVATION.md):

```sh
azahar_static_execution "$translated_library" "$dump_copy" "$dynamic_output" \
  "$original_movie" "$initial_user_directory"
azahar_compiled_execution "$block_schedule" "$dump_copy" "$compiled_output" \
  "$original_movie" "$initial_user_directory"
python tools/static_recompiler/compare_state_observation.py \
  "$stock_capture" "$dynamic_output" --movie "$original_movie" \
  --report build/dynamic_module_comparison.json
python tools/static_recompiler/compare_state_observation.py \
  "$stock_capture" "$compiled_output" --movie "$original_movie" \
  --report build/compiled_module_comparison.json
```

All output/report paths must be absent; reports belong to this checkout's ignored build directory. The accepted bounded checks use presentation 360, input/audio enabled and three diagnostic RAM observations. Both paths must preserve every raw GPU event/tick, PICA byte, screen/framebuffer byte, input event, stereo PCM sample and raw state observation. Both must report zero CPU fallback. Matching diagnostic RAM does not prove Section 7 semantic gameplay state or completed-update alignment.

## Link the translated instruction module

`runtime/port/webassembly_translation` is a separate wasm32 static target. It
compiles all sealed timed generated C units, the real entry table, the timing
callback cell and the address-selected source replacement. It links the
accepted integer guest arithmetic target. Generated code, original instruction
fixtures and outputs remain ignored. The adapter archive above and the platform
implementation remain separate dependencies.

Generate and verify a portable timed native module with the commands in
[FLOATING_POINT.md](FLOATING_POINT.md), then cross-compile those sealed sources:

```sh
python tools/static_recompiler/build_webassembly_translation.py \
  "$timed_native_directory" build/webassembly_translation \
  --softfloat-source "$softfloat_source_directory" \
  --emsdk "$emsdk_directory" --node "$node_executable" --cmake "$cmake_executable"
python tools/static_recompiler/verify_webassembly_translation.py \
  build/webassembly_translation build/webassembly_translation_verification \
  --emsdk "$emsdk_directory" --node "$node_executable" --cmake "$cmake_executable"
```

Set these variables to absolute paths. Both output directories must be absent
children of this checkout's ignored `build/`. The builder verifies the native
library and every source seal, accepted arithmetic/timing source identity,
replacement registry and the replacement's rank O/source identity on frozen
integrator main. Its archive receipt explicitly carries zero execution credit.

The verifier links an actual wasm executable with the complete immutable entry
table. It reads back every actual address and compares it with the sealed source
table, requires nonnull ordered function pointers and checks that priority
dispatch resolves to its source override. It compares selected actual guest
entry calls with original ARM execution through Unicorn, including all integer
registers/next PC, NZCVQ/GE/Thumb, all 32 floating words and FPSCR. The source
replacement comparison follows AAPCS: return value, callee-saved registers, SP
and return PC. Caller-saved integer registers and arithmetic flags are excluded
only for that source function. Every unsupported host memory/interpreter
callback aborts. The translated wasm Context is 120 bytes and Entry is 8 bytes;
these are host interface checks, not guest class-layout evidence.

The selected probes cover 88192 floating-instruction cases and 4105 actual source
replacement cases. Linking the complete table preserves the native manifest's
conservative 2437712 function instruction bytes. Only those probes execute in
this check. This does not establish translated startup, a browser host or a GPU
frame. See [the evidence note](../../project/webassembly_translation_evidence.md)
for actual receipts, hashes and limits.

## Remaining browser work

A runnable host still requires a reduced generic Azahar build without ARM CPU fallback, target-compiled platform dependencies, workers/pthreads, owned-file loading, bounded memory, video presentation and audio. [FLOATING_POINT.md](FLOATING_POINT.md) supplies the separate integer-only guest arithmetic target. No browser page or game frame ran in the binding or translated-module families. World 1-1 entry/goal, browser pixels/audio, performance and complete rank-O source binding remain unverified. [The binding evidence note](../../project/webassembly_static_binding_evidence.md) records the actual adapter archive, callback contract, native replay and refusal/resource controls.
