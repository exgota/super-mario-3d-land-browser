# Bounded replay wall time

Longer menu and level recordings can need more host time than the headless frontend's 120-second default. The optional source-only watchdog patch accepts `ROOT_PORT_CAPTURE_WALL_SECONDS` from 1 through 3600 decimal host seconds. Unset retains 120. The capture records the chosen value as `wall_time_seconds` in its configuration. This controls the outer host deadline only; it does not scale emulated ticks, CPU budgets, HID/movie scheduling, audio or software presentation.

Apply after the accepted audio patch to the pinned Azahar source:

```sh
. ./development_environment.sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_replay_duration.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_replay_duration.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture azahar_static_execution -j 1
```

Follow [AUDIO.md](../AUDIO.md) for the existing pinned build and original movie/snapshot. Set the option equally for both replay commands, for example:

```sh
ROOT_PORT_CAPTURE_WALL_SECONDS=300 ROOT_PORT_CAPTURE_PRESENTATION=360 \
ROOT_PORT_CAPTURE_INPUTS=1 ROOT_PORT_CAPTURE_AUDIO=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_static_execution \
  build/platform_timed_translation/translated.dylib \
  build/root_port_reference/owned_dump.3ds build/root_port_duration/native \
  build/root_port_input/reference/input_movie.ctm \
  build/root_port_input/reference/initial_user_state
```

A deadline cannot interrupt one `System::RunLoop` operation or shutdown work. Elapsed process wall time can therefore exceed the chosen bound. Expiration retains `wall_time_limit`, exit 1 and incomplete outcomes. It supplies no successful replay credit. Invalid, empty, signed, fractional, whitespace, zero, overflowing and out-of-range values return 2 before capture-directory creation or guest loading. The native launcher may open an independently requested memory trace before entering the shared frontend; this option does not change that existing launcher behavior.

The presentation count, command/audio payload bounds and original-movie EOF protections remain unchanged. A longer host deadline alone does not authorize missing level-state observations or prove a level complete. [The evidence note](../../../project/replay_duration_evidence.md) records final-source controls and preservation checks. All movies, captures, samples and generated guest code stay ignored.
