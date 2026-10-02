# Static native platform execution

The strict native backend links locally translated game code to the pinned Azahar HLE platform and software GPU. Guest instructions execute in compiled C, with no interpreter or JIT fallback. Azahar remains the independent reference CPU. Generated source and libraries, owner inputs, movies, user state and GPU captures stay in ignored directories.

Port milestone 1 passes the complete startup prefix through the first top-screen SetBufferSwap. Two fresh native runs match all 528 untouched reference events, including ticks, and all 79,936 PICA bytes. They execute 226,978,430 guest instructions with zero interpreter/JIT fallback. This boundary does not establish a visibly presented frame; rendering is milestone 2. The independent reference uses the documented public ARM64 FPSCR correction in BUILD.md.

## Generate and validate

First follow `azahar_reference/BUILD.md` to obtain the pinned public Azahar source, dependencies and reference executable. Use one compile job. Run these commands from the port checkout, with local approved code.bin/exh.bin and the usual development environment:

```sh
. ./development_environment.sh
python tools/static_recompiler/build_port.py --output build/platform_translation
python tools/static_recompiler/verify_execution.py build/platform_translation
python tools/static_recompiler/build_native_block_schedule.py \
  build/platform_translation build/port_tools/azahar_reproduction \
  build/port_tools/azahar_reproduction_build build/platform_schedule/block_schedule.bin
python tools/static_recompiler/instrument_timing.py \
  build/platform_translation build/port_tools/azahar_reproduction build/platform_timed_translation
cp build/platform_schedule/block_schedule.bin build/platform_timed_translation/block_schedule.bin
python tools/static_recompiler/verify_execution.py build/platform_timed_translation
python tools/static_recompiler/verify_floating_point_execution.py build/platform_timed_translation
```

In a worktree, pass the primary checkout's ignored Azahar path instead, and select the existing local Rust installation through CARGO, CARGO_HOME and RUSTUP_HOME as described in README.md. Every output directory must be absent. Generation reads a frozen main map without changing it. Map Pool hints are not relocation records: executable discovery follows binary control flow and literal reads. Every generated instruction is resumable.

Timing generation invokes the unchanged pinned cost function and Dynarmic A32 frontend. The version-two sidecar retains stock block cycles, predicates, full descriptors and terminal trees. Runtime follows these records, keeps logical visited descriptors and the eight-entry return stack, and exposes completed-block charges at SVC and Run return. It does not decode guest opcodes. The native timing ABI is revision 2; old timed libraries are refused. The private table SHA256 and source seals belong in the build receipt. Scalar arithmetic and square root use the independently checked guest FPSCR helpers. Compiled helper source and generated source are sealed in the build manifest.

## Build the platform adapter

Apply the native platform source patch after the reference capture, deterministic-I/O and Dynarmic FPSCR correction patches. The reference executable registers no native backend.

```sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_static_execution.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_static_execution.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  -S build/port_tools/azahar_reproduction \
  -B build/port_tools/azahar_reproduction_build \
  -DROOT_PORT_SOURCE_DIRECTORY="$PWD" \
  -DROOT_PORT_TRANSLATED_DIRECTORY="$PWD/build/platform_timed_translation"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build --target azahar_static_execution -j 1
```

Reuse the other configuration flags in BUILD.md. The platform adapter implements Azahar's ARM_Interface, thread register contexts, callback memory, TLS, timer costs and supervisor services. Missing translations and explicit interpretation sites throw. The optional adapter and its linked Azahar derivative follow Azahar's GPL-2.0-or-later distribution obligations. The public ARM-to-C generator is separately pinned under MIT.

## Capture and compare

Record a fresh reference and replay it once using the commands in BUILD.md. Give native execution both that recording's movie and its pre-CPU user snapshot. A missing snapshot changes console identity and invalidates the comparison.

```sh
build/port_tools/azahar_reproduction_build/bin/Release/azahar_static_execution \
  build/platform_timed_translation/translated.dylib \
  build/root_port_reference/owned_dump.3ds \
  build/root_port_reference/native_validation \
  build/root_port_reference/reference_validation_0/input_movie.ctm \
  build/root_port_reference/reference_validation_0/initial_user_state
python tools/static_recompiler/compare_gpu_capture.py \
  build/root_port_reference/reference_validation_0 \
  build/root_port_reference/native_validation --report build/native_platform_comparison.json
```

The library extension is .so on Linux. The observed build and run are macOS arm64; Linux has not been tested. Preserve failed captures. The comparator's success requires untouched event bytes including ticks, all payload bytes, completed boundaries and no movie errors. Its metadata comparison is diagnostic only and cannot turn a mismatch into success.

The boundary is the complete startup prefix through the first top-screen SetBufferSwap. It does not prove a visibly presented game frame. Native execution prints its guest instruction count and zero interpreter/JIT fallbacks when the CPU is destroyed.

For bounded debugging, set ROOT_PORT_MEMORY_TRACE_PATH to an absent ignored JSONL file and ROOT_PORT_MEMORY_TRACE_VALUES to comma-separated uint32 values before executing. Both are required together. Matching 32-bit writes record the responsible guest instruction, integer/VFP registers and FPSCR without changing the guest memory operation. Leave tracing disabled for ordinary parity runs.

## Remaining limits

Timing now reproduces the checked startup path, including all raw event ticks. The table covers the two scalar FPSCR modes used there. Unsupported modes, IT/big-endian state, interpreter/exception/check-bit terminals and unrepresented operations are refused. Logical cache visitation does not model stock cache-capacity eviction. The certified priority source adapter refuses a slice ending inside its atomic body; it can stop before entry. Other source bindings need their own resumable scheduling contracts. Short-vector sequencing, conversions and comparison exception behavior need additional checks. Dynamic guest code and instruction-cache invalidation are unsupported; single stepping is refused.

The native source registry currently contains only the clean rank-O priority adapter at address 0x0010766C. Other rank-O functions on main still need portable ABI adapters. Class and pointer layout questions go to the Pro queue. This registry is incomplete and the dashboard's recompiled count is conservative linked instruction coverage, not matching credit.
