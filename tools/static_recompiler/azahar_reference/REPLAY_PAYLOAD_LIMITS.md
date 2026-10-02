# Bounded PICA replay payload extent

Longer replays can exceed the capture frontend's default 128 MiB aggregate PICA command-data bound. The optional source-only patch `azahar_replay_payload_limits.patch` accepts `ROOT_PORT_CAPTURE_PICA_BYTES`, a positive decimal byte count from 1 through 1073741824. Unset keeps 134217728. The configuration records `pica_payload_limit_bytes`.

The option only bounds copied command bytes. It changes no guest instructions, ticks, input/movie delivery, PICA processing, audio or presentation selection. A command list that would cross the chosen extent is refused before its payload is written. Capture-limit exceptions remain unsuccessful, possibly fatal on the stock callback path; do not credit their partial output. Invalid configuration returns 2 before capture-directory creation or guest loading. The native launcher's independently requested diagnostic trace can open earlier, as before.

Apply after the accepted input/audio, replay-duration and default-applet patches to the pinned official Azahar source. See [BUILD.md](BUILD.md), [AUDIO.md](../AUDIO.md), [REPLAY_DURATION.md](REPLAY_DURATION.md) and [DEFAULT_APPLETS.md](DEFAULT_APPLETS.md) for prerequisites and source provenance.

```sh
. ./development_environment.sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_replay_payload_limits.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_replay_payload_limits.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture azahar_static_execution -j 1
```

For an original movie and its own pre-CPU user snapshot, choose the same natural presentation on stock and native. Output directories must be absent. Generated libraries, movies, snapshots, command data and previews stay ignored.

```sh
ROOT_PORT_CAPTURE_PICA_BYTES=268435456 ROOT_PORT_CAPTURE_WALL_SECONDS=600 \
ROOT_PORT_CAPTURE_PRESENTATION=2200 ROOT_PORT_CAPTURE_INPUTS=1 ROOT_PORT_CAPTURE_AUDIO=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds build/root_port_payload_limits/reference \
  build/root_port_world_one/reference/input_movie.ctm \
  build/root_port_world_one/reference/initial_user_state
ROOT_PORT_CAPTURE_PICA_BYTES=268435456 ROOT_PORT_CAPTURE_WALL_SECONDS=600 \
ROOT_PORT_CAPTURE_PRESENTATION=2200 ROOT_PORT_CAPTURE_INPUTS=1 ROOT_PORT_CAPTURE_AUDIO=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_static_execution \
  build/platform_timed_translation/translated.dylib \
  build/root_port_reference/owned_dump.3ds build/root_port_payload_limits/native \
  build/root_port_world_one/reference/input_movie.ctm \
  build/root_port_world_one/reference/initial_user_state
python tools/static_recompiler/compare_movie_replay.py \
  build/root_port_payload_limits/reference build/root_port_payload_limits/native \
  --movie build/root_port_world_one/reference/input_movie.ctm \
  --report build/payload_extent_comparison.json
```

A larger recording extent does not establish World 1-1, gameplay state semantics or the goal. The existing presentation-count, audio, movie and host-time bounds still apply. [The evidence note](../../../project/replay_payload_limits_evidence.md) records final source, executable identities, boundary controls and unchanged replay checks.
