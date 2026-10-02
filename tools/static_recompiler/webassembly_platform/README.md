# Pinned WebAssembly platform

This family builds the actual public Azahar system, memory, HLE services,
software PICA renderer, Teakra DSP and headless capture host around the accepted
statically compiled ARM backend. It links the sealed translation and portable
arithmetic archives. Generated game code, dumps, timing sidecars, dependency
checkouts, binaries and captures stay in ignored `build/`.

`source_revisions.json` pins Azahar, its 15 required Git submodules, standalone
MIT robin-map, ten accepted reference patches, four platform patches and the
LibreSSL hook. The builder clones committed Git objects into a new source tree.
An optional local public Git cache saves downloads; dirty cached files are not
copied. Gitlinks, patch hashes, source seals and license hashes must agree.
The existing native oracle checkout is never modified.

The platform option defaults off. When enabled, it requires Emscripten,
GENERIC architecture and software rendering. It rejects unsupported frontend,
accelerated graphics, audio device and network frontend options. It excludes
guest ARM CPU interpreters/JITs, PICA shader JITs, native dynamic loading,
FFmpeg and GameMode. A missing static CPU registration throws. PICA shader
interpretation and Teakra DSP are required platform components and remain.

Four typed zero returns use `std::size_t{0}` to preserve their value on wasm32.
Thread naming uses the actual Emscripten diagnostic API. Native alternatives
remain behind their existing platform guards. These source branches are
reviewed here; no new option-off native build is certified by this family.

LibreSSL keeps its real ChaCha implementation and `getentropy` acquisition.
The Emscripten hook allocates zeroed state and uses a checked pthread mutex.
Mutex or entropy failure aborts. Fork detection is excluded only because
Emscripten does not implement process fork. Original copyright/permission
notices remain, and the builder copies pinned Azahar, LibreSSL, robin-map and
SoftFloat license files beside the output. This is not TLS or online-service
acceptance.

## Build and audit

Run from the root branch checkout after sourcing its normal development
environment, or source the primary checkout's environment if the worktree
does not have `.venv`. Set the following paths to your existing ignored tools
and sealed translation output. The output directory must be absent.

```sh
port_emsdk=/absolute/path/to/ignored/emsdk
port_node="$port_emsdk/node/24.19.0_64bit/bin/node"
port_cmake=/absolute/path/to/ignored/cmake
port_translation=/absolute/path/to/sealed/translated_archive
port_translation_verification=/absolute/path/to/successful/translation_execution/result.json

python tools/static_recompiler/build_webassembly_platform.py \
  "$port_translation" build/pinned_platform \
  --emsdk "$port_emsdk" --node "$port_node" --cmake "$port_cmake" \
  --translation-verification "$port_translation_verification"

python tools/static_recompiler/audit_webassembly_platform.py \
  build/pinned_platform/compiled build/platform_audit.json \
  --llvm-nm "$port_emsdk/upstream/bin/llvm-nm"

python tools/static_recompiler/verify_platform_entropy.py \
  build/pinned_platform build/platform_entropy \
  --emsdk "$port_emsdk" --node "$port_node"
```

The tested toolchain is official Emscripten 6.0.10 revision
`d6c521a7f05449857c76bd99e396895583cf2083`, SDK checkout
`e566f7bdcc7735f44037911c24b87a58a3c93145`, Node 24.19.0 and CMake 3.31.10.
The builder compiles serially at O1 with real pthreads and wasm exceptions.
Every command, status, elapsed time and log remains in its output directory.

The audit checks the configured source inventory, undefined symbols of four
actual adapter/platform archives and the final wasm import section. Inventory
entries are not a count of compiled units. It requires one shared wasm32
memory with a 1 GiB initial allocation and 2 GiB maximum. These memory and
18-worker settings reproduce the tested host. They are not measured browser
requirements. No audit alone proves guest execution.

The entropy check links the actual new crypto archive. Two runs each generate
65,536 concurrent 64-byte samples and check the SHA256 `abc` known vector,
real successful entropy requests, and the 257-byte range refusal with its
buffer preserved. It forces both a C entropy-source failure and an actual
Node `randomFillSync` failure. Both must refuse before successful completion.
No statistical quality, browser Web Crypto, allocation/mutex failure or TLS
protocol certification follows.

## Bounded first GPU replay

The executable is a finite Node filesystem capture host. `NODERAWFS=1` lets it
read an owned dump and sidecar directly; the link permits only Node and worker
environments. Its no-argument invocation must return 2 from the actual usage
check. Browser builds need a separate worker/filesystem transport.

Supply an approved owned dump copy, the unchanged revision-two timing
sidecar, the same recorded movie and initial user directory as the corrected
Azahar reference. The capture and comparison outputs must be absent.

```sh
ROOT_PORT_CAPTURE_WALL_SECONDS=120 "$port_node" \
  build/pinned_platform/compiled/bin/Release/root_port_webassembly_capture.js \
  /absolute/path/to/block_schedule.bin \
  /absolute/path/to/owned_dump.3ds \
  "$PWD/build/wasm_first_swap" \
  /absolute/path/to/input_movie.ctm \
  /absolute/path/to/initial_user_state

python tools/static_recompiler/compare_gpu_capture.py \
  /absolute/path/to/corrected_azahar_first_swap \
  build/wasm_first_swap --report build/wasm_first_swap_comparison.json
```

Keep an external wall timeout around guest execution and hash input/reference
files before and after. The comparator accepts only complete raw event streams,
including ticks, and every PICA payload. Its timing-stripped comparison is a
diagnostic and never changes acceptance. Confirm zero ARM interpreter/JIT
fallback in the backend's final counters.

Node first-swap parity is a bounded execution check. It does not establish a
browser frame, browser inputs/audio, World 1-1 or Section 7 semantic state.
Complete rank-O source adapters beyond the reviewed priority conversion also
remain open. The native recompiled byte count and matching ARMCC build do not
change.
