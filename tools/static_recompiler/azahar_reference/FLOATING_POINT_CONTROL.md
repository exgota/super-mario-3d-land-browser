# Correct ARM64 host floating-point control after guest VMSR

The pinned public Dynarmic ARM64 backend changes the guest FPSCR descriptor after VMSR without updating host FPCR. Its Run/Step prelude loads host control only on entry. A later division in the same Run therefore uses the former rounding mode despite reporting the new guest FPSCR. The one-line patch writes the already-masked new control value to host FPCR at the same guest register update.

This is a host implementation correction, independently checked against the original ARM instructions. Historical reference artifacts remain preserved. It changes neither the original executable nor native static-recompiler arithmetic. All GPU command bytes now agree; native event timing still differs and milestone 1 remains incomplete.

## Source and licenses

Azahar is pinned at 662d412123305a9f4be94dd3dc73ddf91a18c55e. Its initialized Dynarmic submodule is pinned at e77b1ba0b7da7cbe93021b01a663acfe7c4dd516. The changed source file is public, with a 0BSD header. LICENSE.md describes this scoped correction and the combined Azahar derivative's GPL obligations.

The primary source locations are the [guest FPSCR writer](https://github.com/azahar-emu/dynarmic/blob/e77b1ba0b7da7cbe93021b01a663acfe7c4dd516/src/dynarmic/backend/arm64/emit_arm64_a32.cpp), [Run/Step prelude and dispatcher](https://github.com/azahar-emu/dynarmic/blob/e77b1ba0b7da7cbe93021b01a663acfe7c4dd516/src/dynarmic/backend/arm64/a32_address_space.cpp), and the existing [A64 host control writer](https://github.com/azahar-emu/dynarmic/blob/e77b1ba0b7da7cbe93021b01a663acfe7c4dd516/src/dynarmic/backend/arm64/emit_arm64_a64.cpp). No leaked source, Game source change or game payload is included.

## Reproduce the bounded original-instruction check

Use two absent ignored output executable paths. First build the original pinned two-patch reference using BUILD.md's source/dependency/configuration commands, before applying this floating-point correction. The following probe build is tested on macOS arm64 with the system fmt installation. It reads every guest instruction word, including its terminating supervisor call, from the local approved code.bin. Host control observation uses standard fegetenv and Darwin fenv_t.__fpcr, with no inline assembly.

```sh
. ./development_environment.sh
python -m pip install unicorn==2.1.4
azahar_source="$PWD/build/port_tools/azahar_reproduction"
azahar_build="$PWD/build/port_tools/azahar_reproduction_build"
reference_directory="$PWD/tools/static_recompiler/azahar_reference"
mkdir -p build/floating_point_control_validation
c++ -std=c++20 -O2 \
  -I"$azahar_source/externals/dynarmic/src" \
  -I"$azahar_source/externals/dynarmic/externals/mcl/include" \
  -I/opt/homebrew/include \
  tools/static_recompiler/verify_azahar_floating_point_control.cpp \
  "$azahar_build/externals/dynarmic/src/dynarmic/libdynarmic.a" \
  "$azahar_build/externals/dynarmic/externals/mcl/src/libmcl.a" \
  -L/opt/homebrew/lib -lfmt -Wl,-rpath,/opt/homebrew/lib \
  -o build/floating_point_control_validation/original
```

The patch names the file through its parent Azahar tree. Apply it in the pinned Dynarmic submodule with three path components stripped. It passes a clean temporary-index check initialized at that exact submodule revision.

```sh
git -C "$azahar_source/externals/dynarmic" apply --check -p3 \
  "$reference_directory/azahar_floating_point_control.patch"
git -C "$azahar_source/externals/dynarmic" apply -p3 \
  "$reference_directory/azahar_floating_point_control.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build "$azahar_build" --target azahar_gpu_capture -j 1
c++ -std=c++20 -O2 \
  -I"$azahar_source/externals/dynarmic/src" \
  -I"$azahar_source/externals/dynarmic/externals/mcl/include" \
  -I/opt/homebrew/include \
  tools/static_recompiler/verify_azahar_floating_point_control.cpp \
  "$azahar_build/externals/dynarmic/src/dynarmic/libdynarmic.a" \
  "$azahar_build/externals/dynarmic/externals/mcl/src/libmcl.a" \
  -L/opt/homebrew/lib -lfmt -Wl,-rpath,/opt/homebrew/lib \
  -o build/floating_point_control_validation/corrected
python tools/static_recompiler/verify_azahar_floating_point_control.py \
  build/floating_point_control_validation/original \
  build/floating_point_control_validation/corrected \
  --report build/floating_point_control_validation/report.json
```

The verifier hashes the approved original executable before execution and checks the complete 96-case matrix. It composes three original register-only instructions in local emulated memory: VMSR, VDIV and VMOV. Unicorn ARM11MPCore runs exactly those instructions independently of the native helpers or Dynarmic implementation. All 16 initial/requested rounding pairs, divisors 240/320/400 and continuous/split Run cases are checked. The original host fails 22 continuous results, has zero split-result or guest-FPSCR differences, and reports 36 stale host control values. The correction has zero result, guest status or host-control differences and zero fallback across every case.

The actual local check compiled a separate corrected emission object and replaced it only in an ignored copy of the original archive. The settled shared source/archive and original capture executable remain restored. The complete clean build described above has not been rerun separately. Exact local arguments and hashes are preserved in build/root_port_viewport/floating_point_control_verification_commands.json and corrected_capture_link.json.

## Verify complete capture without normalization

Use the owned dump copy, existing input movie and its initial user snapshot together. Record and replay using BUILD.md. Replaying the same historical input pair with this corrected executable twice produces two new absent output directories. Preserve the earlier two-patch outputs. Compare untouched gpu_events.jsonl bytes and every pica_command_list_*.bin payload exactly, using BUILD.md's checker or compare_gpu_capture.py once the native platform family has landed.

Observed corrected directories are build/root_port_reference/reference_updated_floating_point_control_0 and _1. They match each other exactly: 528 events including ticks, 348 GSP commands, eight PICA lists and 79,936 payload bytes, 68 direct register writes, 102 VBlanks, one top-screen swap and zero movie errors. The raw event SHA-256 stays ed7a39625518e559be6c0bc8f5a50d05f088161bc9256b6a8a169eab114f9859. Exactly 16 historical payload words change; every corrected payload byte matches the unchanged native static_execution_2 run. This exact count supersedes the earlier erroneous estimate of 18.

Native and corrected reference command metadata agrees, but 473 event tick values differ. Strict comparison still fails. The first timing difference is event 5, VBlank, native minus reference -1; the observed range is -2390 through +10402. No acceptance criterion was relaxed, and the native arithmetic was not changed to imitate the old oracle defect.

floating_point_control_provenance.json seals the patch, public probe, verification runner, pinned revisions and aggregate observations. Detailed reports remain ignored under build/root_port_viewport, build/root_port_reference and build/root_azahar_static_platform/build. Scope is the observed scalar rounding transition and startup GPU prefix on macOS arm64. Other controls, exception delivery, complete CPU equivalence, visible rendering and gameplay remain unverified.
