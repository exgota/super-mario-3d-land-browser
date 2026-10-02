# Portable guest floating point

`NativeFloatingPoint.c` keeps IEEE binary32/binary64 values as integer bits. It uses official Berkeley SoftFloat 3e for add, subtract, multiply, divide and square root. The ARM-VFPv2 specialization, generic integer primitives and C11 thread-local state compile from the same sources on native and wasm32 targets. Multiply-accumulate operations perform two separately rounded operations. Host floating-point rounding and exception APIs are absent from this helper.

Guest FPSCR controls rounding, default NaN and flush-to-zero behavior. Arithmetic retains cumulative guest exceptions and restores all four SoftFloat state variables before return. Selected original VMUL instructions establish tininess before rounding. Enabled arithmetic exception delivery is refused. Raw move, absolute and negate operate on bits and preserve FPSCR even with traps enabled. This target does not implement conversions, comparisons or short-vector register sequencing.

## Obtain the public dependency

Read the [author's source documentation](https://www.jhauser.us/arithmetic/SoftFloat-3/doc/SoftFloat-source.html) and the package's `COPYING.txt`. Download only into ignored tool storage:

```sh
mkdir -p build/port_tools
curl --fail --location https://www.jhauser.us/arithmetic/SoftFloat-3e.zip \
  --output build/port_tools/SoftFloat-3e.zip
python3 - <<'PY'
import hashlib
from pathlib import Path
path = Path('build/port_tools/SoftFloat-3e.zip')
assert hashlib.sha256(path.read_bytes()).hexdigest() == \
    '21130ce885d35c1fe73fc1e1bf2244178167e05c6747cad5f450cc991714c746'
PY
unzip build/port_tools/SoftFloat-3e.zip -d build/port_tools
```

The public source manifest pins every used official C file, all included package headers and the license. Configuration refuses missing or modified files. Downloaded dependency source stays unmodified and uncommitted. The target copies its BSD three-clause license beside the built archive. Include that notice when distributing linked binaries. Azahar's separate GPL obligations still apply to an Azahar-derived host.

## Build the same target on both platforms

```sh
. ./development_environment.sh
python tools/static_recompiler/build_floating_point.py \
  build/port_tools/SoftFloat-3e/source build/floating_point_native
```

The native wrapper requires an absent output directory under this checkout's ignored `build`. It uses one compile job and writes source, configuration and archive hashes. The timed module builder requires `--softfloat-source` and records that receipt in its own manifest.

For the pinned SDK in [WEBASSEMBLY.md](WEBASSEMBLY.md), configure the actual arithmetic target through `emcmake`. Keep outputs ignored:

```sh
emcmake cmake -S runtime/port/floating_point -B build/floating_point_webassembly \
  -DROOT_PORT_SOFTFLOAT_SOURCE_DIRECTORY="$PWD/build/port_tools/SoftFloat-3e/source" \
  -DCMAKE_BUILD_TYPE=Release
cmake --build build/floating_point_webassembly --target port_floating_point -j 1
```

All dependency translation units and the adapter receive `SOFTFLOAT_FAST_INT64=1`, `THREAD_LOCAL=_Thread_local` and `INLINE_LEVEL=0`. The little-endian platform header selects no native arithmetic intrinsics. Emscripten compilation and linking require pthread support. The archive supplies arithmetic; a browser host must still link translated code, platform services and its own lifecycle.

## Verify against original execution

Build the timed native module using NATIVE_PLATFORM.md, then run its existing `verify_execution.py` and `verify_floating_point_execution.py` checks. Those checks execute generated instruction entries and compare original registers, flags and memory.

For the wasm arithmetic API, the public runner reads selected instructions from the local hashed EU executable. It generates independent Unicorn results, compiles this exact public CMake target and runs it in Node with pthreads. It independently decodes the returned integer result/FPSCR stream and retains the first divergence:

```sh
python tools/static_recompiler/verify_floating_point_webassembly.py \
  build/port_tools/SoftFloat-3e/source build/floating_point_webassembly_verification \
  --emsdk build/port_tools/emsdk \
  --node build/port_tools/emsdk/node/24.19.0_64bit/bin/node
```

Pass `--cmake` when the pinned CMake executable is outside PATH. All outputs, fixtures and actual results remain ignored. The test also checks four software state variables and concurrent RN/RZ arithmetic. This certifies the arithmetic API in Node. It supplies no translated game-byte credit and does not certify browser graphics, input or audio. Original double-precision samples cover add/subtract/multiply/divide/move; the other double selectors still need their own original-instruction probes.

[The evidence note](../../project/portable_floating_point_evidence.md) records the final source hashes, measured numerical checks and untouched Azahar replay comparisons.
