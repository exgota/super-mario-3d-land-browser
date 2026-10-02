# Reproduce the Azahar first-swap reference

These commands build a source-only, non-Qt software frontend at official Azahar commit `662d412123305a9f4be94dd3dc73ddf91a18c55e`. Run them from the SM3DL repository root. The source, tools, build, owner inputs and output must remain ignored. The patch contains no game payload. Azahar is GPL-2.0-or-later; retain its license and upstream provenance when distributing this source derivative.

The current machine has Xcode AppleClang 21.0.0, macOS 27.0 arm64, Homebrew fmt 12.2.0 and OpenSSL 3.6.4. Those system libraries are required by this configuration. The commands below describe the already-observed build in a separate reproduction directory. This separate full rebuild has not been run. Source hashes and observed execution metadata are in `source_provenance.json`. Pinned dependencies are in `dependency_revisions.txt`. Historical local build logs remain under ignored `build/root_port_reference/`.

## Source, tools and pinned dependencies

```sh
. ./development_environment.sh
python3 -m pip install --target build/port_tools/azahar_reproduction_python_packages cmake==3.31.10 ninja==1.13.0
git clone --filter=blob:none --depth 1 --no-checkout https://github.com/azahar-emu/azahar.git build/port_tools/azahar_reproduction
git -C build/port_tools/azahar_reproduction fetch --depth 1 origin 662d412123305a9f4be94dd3dc73ddf91a18c55e
git -C build/port_tools/azahar_reproduction checkout --detach 662d412123305a9f4be94dd3dc73ddf91a18c55e
git -C build/port_tools/azahar_reproduction submodule update --init --depth 1 --recursive externals/boost externals/cryptopp externals/oaknut externals/dynarmic externals/inih/inih externals/nihstro externals/faad2/faad2 externals/soundtouch externals/teakra externals/enet externals/lodepng/lodepng externals/library-headers externals/dds-ktx externals/xxHash externals/zstd
reference_directory="$PWD/tools/static_recompiler/azahar_reference"
git -C build/port_tools/azahar_reproduction apply --check "$reference_directory/azahar_capture.patch"
git -C build/port_tools/azahar_reproduction apply --check "$reference_directory/azahar_deterministic_io.patch"
git -C build/port_tools/azahar_reproduction apply "$reference_directory/azahar_capture.patch"
git -C build/port_tools/azahar_reproduction apply "$reference_directory/azahar_deterministic_io.patch"
```

Initialize only the listed dependencies. The capture patch allows this restricted dependency set when `ROOT_PORT_CAPTURE_MINIMAL_DEPENDENCIES=ON`. The disabled GUI, audio-device, networking frontend and Vulkan dependencies are unnecessary. `dependency_revisions.txt` records both initialized revisions and omitted upstream gitlinks. Submodule lines prefixed with `-` were intentionally not initialized.

## Configure and build

```sh
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  -S build/port_tools/azahar_reproduction \
  -B build/port_tools/azahar_reproduction_build \
  -G "Unix Makefiles" \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_PREFIX_PATH=/opt/homebrew \
  -DOPENSSL_ROOT_DIR=/opt/homebrew/opt/openssl@3 \
  -DROOT_PORT_CAPTURE_MINIMAL_DEPENDENCIES=ON \
  -DENABLE_QT=OFF -DENABLE_QT_TRANSLATION=OFF \
  -DENABLE_SDL2=OFF -DENABLE_OPENAL=OFF -DENABLE_CUBEB=OFF \
  -DENABLE_LIBUSB=OFF -DENABLE_WEB_SERVICE=OFF -DENABLE_SCRIPTING=OFF \
  -DENABLE_GDBSTUB=OFF -DENABLE_TESTS=OFF \
  -DENABLE_ROOM=OFF -DENABLE_ROOM_STANDALONE=OFF \
  -DENABLE_OPENGL=OFF -DENABLE_VULKAN=OFF \
  -DENABLE_SOFTWARE_RENDERER=ON -DENABLE_BUILTIN_KEYBLOB=OFF \
  -DENABLE_LTO=OFF -DCITRA_USE_PRECOMPILED_HEADERS=OFF \
  -DDYNARMIC_USE_PRECOMPILED_HEADERS=OFF -DCITRA_WARNINGS_AS_ERRORS=OFF \
  -DUSE_SYSTEM_FMT=ON -DUSE_SYSTEM_OPENSSL=ON -DUSE_SYSTEM_ZSTD=OFF
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture -j 1
```

Use one compile job and monitor free space. Stop below 5 GiB free disk. The original ignored build watchdog recorded these limits locally; it is not required by the source package. The original source plus output and tool installation used approximately 924 MiB. The final executable links Apple's Foundation, CoreFoundation, CoreGraphics and AppKit frameworks. Bundled zstd is necessary because current Homebrew zstd does not provide Azahar's required seekable header.

## Verify owner input and capture

Only the already-provided legally owned dump is permitted. The input movie and pre-CPU user snapshot are generated locally. This package includes neither. In another worktree or clone, create a read-only copy in ignored `build/root_port_reference/` and verify the approved hashes before invoking the frontend.

The verified local copy is `build/root_port_reference/owned_dump.3ds`, an APFS clone of the owner's EU Rev 2 dump, made read-only. Do not fetch game data. Original and copy SHA-256 are `c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976`. The matching project executable SHA-256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. The ignored historical `owner_input_postrun_manifest.json` verified both after the observed emulator executions. Recheck these identities locally:

```sh
python3 - <<'PY'
from pathlib import Path
import hashlib

inputs = [(Path('build/root_port_reference/owned_dump.3ds'),
           'c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976'),
          (Path('data/ver/eu/code.bin'),
           'e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64')]
for path, expected in inputs:
    digest = hashlib.sha256()
    with path.open('rb') as stream:
        for block in iter(lambda: stream.read(1024 * 1024), b''):
            digest.update(block)
    actual = digest.hexdigest()
    print(path, actual)
    if actual != expected:
        raise SystemExit('Input identity differs from the approved EU version')
PY
```

The commands below use the reproduction executable built above. The historical local executable is `build/port_tools/azahar_build/bin/Release/azahar_gpu_capture`. Output directories must be absent. Recording creates a fixed clock movie and a snapshot of initial console/save state before CPU execution. Playback must receive both.

```sh
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds \
  build/root_port_reference/reference_validation_0
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds \
  build/root_port_reference/reference_validation_1 \
  build/root_port_reference/reference_validation_0/input_movie.ctm \
  build/root_port_reference/reference_validation_0/initial_user_state
```

This configuration selects the stock Dynarmic CPU, software GPU, old 3DS, fixed initial clock, no input devices, HLE DSP, null audio sink and no frame limiter. Upstream deterministic async is enabled, ordinary async FS is disabled, and the separate I/O correction retains simulated read latency instead of subtracting host elapsed time in deterministic mode. The correction does not change CPU or GPU instruction semantics. Normal async mode remains unchanged.

Frame zero is the complete startup prefix through the first `GPU::SetBufferSwap(screen_id==0)`. It includes setup, direct GPU register writes, bottom-screen events and VBlanks. It does not establish a visibly presented game frame. GPU dispatch is synchronous in this software path. Initial and actually followed chained PICA list bytes are captured before processing; the chain hook was not reached in the observed startup.

## Exact comparison

```sh
python3 - <<'PY'
from pathlib import Path
import json

root = Path('build/root_port_reference')
reference = root / 'reference_validation_0'
replay = root / 'reference_validation_1'
reference_payloads = {path.name: path.read_bytes()
                      for path in reference.glob('pica_command_list_*.bin')}
replay_payloads = {path.name: path.read_bytes()
                   for path in replay.glob('pica_command_list_*.bin')}
events_equal = (reference / 'gpu_events.jsonl').read_bytes() == \
               (replay / 'gpu_events.jsonl').read_bytes()
payloads_equal = reference_payloads == replay_payloads
movie_errors = 'Movie <Error>' in \
               (replay / 'user/log/reference_capture.log').read_text()
complete = json.loads((replay / 'gpu_events.jsonl').read_text().splitlines()[-1])
result = {'events_equal': events_equal, 'payloads_equal': payloads_equal,
          'movie_errors': movie_errors, 'capture_outcome': complete}
print(json.dumps(result, indent=2))
raise SystemExit(0 if events_equal and payloads_equal and not movie_errors and
                 complete['complete'] and complete['pica_lists'] > 0 else 1)
PY
```

The already-observed record `reference_fixed_io_0` and playbacks `reference_fixed_io_1` and `reference_fixed_io_2` all match exactly: 528 events, 348 GSP commands, eight PICA lists, 79,936 PICA bytes, and zero Movie errors. Full event SHA-256 is `ed7a39625518e559be6c0bc8f5a50d05f088161bc9256b6a8a169eab114f9859`. See `fixed_io_replay_report.json` and the three run receipts. Fresh recordings choose another timing base; a new record/replay pair must match each other and need not share that historical hash.

## Register the strict native CPU backend

Link the parent-owned native executable with `root_port_headless_capture`. Register `RootPortCapture::CpuBackendFactory` before calling `RootPortCapture::RunHeadlessCapture(argc, argv)`. The callback receives `Core::System&`, `Memory::MemorySystem&`, CPU core index and shared timer, and returns a `shared_ptr<Core::ARM_Interface>`. `System::Init` uses it for every CPU core; the stock oracle registers no factory. The frontend otherwise shares the exact settings, capture convention, movie and initial state above.

`ARM_Interface::Run` must return when timer downcount expires or rescheduling is requested. Update the timer with instruction costs matching `Core::TicksForInstruction`. Route services through `Kernel::SVCContext(system).CallSVC(immediate)` and guest memory through `Memory::MemorySystem`. The native backend must stop on unsupported instructions or missing addresses. A successful native capture requires explicit zero interpreter/JIT fallback evidence and exact reference comparison. Registration alone does not establish strict native execution.
