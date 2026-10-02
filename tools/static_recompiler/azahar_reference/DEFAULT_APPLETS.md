# Default headless frontend applets

The headless capture frontend must register interfaces for HLE applets, as the normal public Azahar frontends do. Without a Mii selector, fresh save setup reaches `HLE::Applets::MiiSelector::Start` and asserts on its null frontend. `azahar_default_applets.patch` calls the pinned upstream `Frontend::RegisterDefaultApplets(system)` before loading the guest. It adds no game code, avatar data or keyboard implementation.

The upstream registration installs its existing default software keyboard and Mii selector. The selector immediately returns the existing public standard Mii through the ordinary HLE applet result. The keyboard uses the upstream default implementation. These configured frontend choices belong to the reference and native runs equally. They are not recorded HID button choices or a new interactive browser applet UI.

Apply after the accepted audio patch and optional replay watchdog patch:

```sh
. ./development_environment.sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_default_applets.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_default_applets.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture azahar_static_execution -j 1
```

The patch independently applies with and without the watchdog. [REPLAY_DURATION.md](REPLAY_DURATION.md) describes the longer bound used by the observed setup path. [INPUT.md](../INPUT.md), [AUDIO.md](../AUDIO.md) and [NATIVE_PLATFORM.md](../NATIVE_PLATFORM.md) provide the pinned tool and sealed translation builds. Save all owner-derived recordings under ignored build storage.

The observed setup script contains a neutral frame-zero state and four-frame A pulses at absolute renderer frames 360, 500, 620, 740, 860, 980, 1120, 1280 and 1440. Use the same initial snapshot and base ticks for the failed and configured stock recordings. Capture 1500 natural presentations after the first top-screen submission. With applets registered it reaches file selection; slot A shows World 1 and is saving. This does not establish entry into World 1-1.

The distinct general movie comparator accepts replay of legitimate recorded channels, including an A-only setup path. The earlier input/audio milestone comparators retain their explicit all-channel demonstration requirements. This comparator still requires original typed movie identity/clock/order, delivered HID bytes/ticks, complete raw GPU/PICA/pixels, audio events/ticks and PCM to agree. It does not prove the game's semantic state or complete a level.

```sh
ROOT_PORT_CAPTURE_WALL_SECONDS=600 ROOT_PORT_CAPTURE_PRESENTATION=1500 \
ROOT_PORT_CAPTURE_INPUTS=1 ROOT_PORT_CAPTURE_AUDIO=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_static_execution \
  build/platform_timed_translation/translated.dylib \
  build/root_port_reference/owned_dump.3ds build/root_port_world_one/native \
  build/root_port_world_one/reference/input_movie.ctm \
  build/root_port_world_one/reference/initial_user_state
python tools/static_recompiler/compare_movie_replay.py \
  build/root_port_world_one/reference build/root_port_world_one/native \
  --movie build/root_port_world_one/reference/input_movie.ctm \
  --report build/default_applets_comparison.json --preview-directory build/applets_preview
```

Reference recording uses the same capture flags and the script through ROOT_PORT_INPUT_SCRIPT, with no movie arguments. Replay leaves script and recording-clock/snapshot options unset and passes the original movie and its own pre-CPU snapshot. [The evidence note](../../../project/default_frontend_applets_evidence.md) identifies the final source and receipts. Interactive Mii/keyboard choices, hardware applet accuracy, World 1-1 state/goal replay and browser execution remain unverified.
