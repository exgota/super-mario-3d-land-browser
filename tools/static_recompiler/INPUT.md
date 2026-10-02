# Deterministic native input

The reference records scripted buttons, circle-pad movement, touch and releases through Azahar's existing movie calls. Native execution replays that unmodified movie and its pre-CPU user snapshot. A separate observation records the HID ring entries immediately before the service signals the game. Strict comparison requires every delivered input byte and tick, the movie's typed records, GPU events, PICA bytes and rendered pixels to agree.

Input is indexed by the software renderer's absolute frame counter, starting at process load. HID polls about 234 times per second, while presentation runs about 59.831 times per second. A state change takes effect at the first HID poll whose renderer frame reaches its selection, and remains held until the next change. The observed poll ticks are the replay contract. Do not schedule by Movie::GetCurrentInputIndex, which converts the pad count to an approximate frame count.

## Build

Follow [RENDERING.md](RENDERING.md) and [NATIVE_PLATFORM.md](NATIVE_PLATFORM.md), including the pinned public Azahar source, native library and documented ARM64 floating-point correction. Apply the input patch after the presentation patch:

```sh
. ./development_environment.sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_input_capture.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_input_capture.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture azahar_static_execution -j 1
```

## Record

Create a local ignored script with seven decimal columns: absolute renderer frame, button mask, circle X, circle Y, touch X, touch Y and touch validity. Comments begin with `#`. Frames start at zero and increase strictly. Buttons use the ordinary lower twelve HID bits; A is 1. Circle axes range from -154 through 154. Touch coordinates range from 0 through 319 and 0 through 239; validity is 0 or 1. The script is bounded to 4096 states and frame 1000000. The observed menu control used:

```text
0 0 0 0 0 0 0
360 1 0 0 0 0 0
364 0 0 0 0 0 0
400 0 154 0 0 0 0
404 0 0 0 0 0 0
440 0 0 0 160 196 1
444 0 0 0 0 0 0
```

Use the approved local dump copy and an absent ignored output directory:

```sh
ROOT_PORT_CAPTURE_PRESENTATION=360 ROOT_PORT_CAPTURE_INPUTS=1 \
ROOT_PORT_INPUT_SCRIPT="$PWD/build/menu_input_script.txt" \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds build/root_port_input/reference
```

The reference writes its movie, pre-CPU user snapshot, canonical input-script receipt and input observations. Injection occurs after device filtering and before the existing movie record call. The normal HID service derives circle direction bits and button edges, updates its ring buffers and signals the guest. No additional movie call is made. Script injection is recording-only. The remote ArticBase path is refused while observing input because its direction-bit convention differs.

For a causal neutral control, record another run from the same pre-CPU state and fixed timing base. Set `ROOT_PORT_RECORD_INITIAL_USER_STATE` to that snapshot and `ROOT_PORT_RECORD_BASE_TICKS` to the recorded configuration's nonnegative base ticks before recording both runs. These options are refused during playback. Use a second script containing only `0 0 0 0 0 0 0` and the same final presentation selection. Compare the configurations and console/save-file hashes. Diagnostic logs differ and do not supply game state. Fresh movie IDs are random; compare their typed input payloads and actual outputs.

## Replay and verify

Leave all three recording options unset during playback. Observe the movie-delivered HID values:

```sh
ROOT_PORT_CAPTURE_PRESENTATION=360 ROOT_PORT_CAPTURE_INPUTS=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds build/root_port_input/reference_replay \
  build/root_port_input/reference/input_movie.ctm \
  build/root_port_input/reference/initial_user_state
ROOT_PORT_CAPTURE_PRESENTATION=360 ROOT_PORT_CAPTURE_INPUTS=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_static_execution \
  build/platform_timed_translation/translated.dylib \
  build/root_port_reference/owned_dump.3ds build/root_port_input/native \
  build/root_port_input/reference/input_movie.ctm \
  build/root_port_input/reference/initial_user_state
python tools/static_recompiler/compare_input_capture.py \
  build/root_port_input/reference build/root_port_input/reference_replay \
  --report build/input_stock_comparison.json
python tools/static_recompiler/compare_input_capture.py \
  build/root_port_input/reference build/root_port_input/native \
  --report build/input_native_comparison.json --preview-directory build/input_preview
```

Repeat stock and native playback into separate absent outputs. When comparing two replay directories, pass `--movie` with the original reference movie. Reports and previews must be absent paths under the invoking checkout's ignored `build`. Never commit movies, user state, framebuffer data or previews.

The comparator validates the complete input schema, sequence, ring indices, button edges, bounds, typed movie order, program identity, revision, clock, pad count and exact delivered values. Every script state must actually reach an observed poll. Success also requires a nonzero press and release for all three channels, completed input/rendering outcomes and no Movie error. It retains raw input/event/timing equality. A later HID poll after movie completion throws instead of silently using live device state.

## Observed scope

The frame-360 button pulse advances the Welcome dialog to StreetPass information. At final renderer frame 461, the neutral control still shows Welcome. Their configurations and all console/save files are equal; only diagnostic logs differ. Two native replays and two stock replays match 1802 delivered HID polls, 4583 raw GPU events/ticks, 545 PICA lists/8642912 bytes and both rendered/raw framebuffer streams. Each native run executes 393977876 guest instructions with zero interpreter/JIT fallback. [The evidence note](../../project/input_record_playback_evidence.md) identifies final source hashes and receipts.

This proves bounded native input delivery and a visible menu response. Physical devices, browser input, audio output, World 1-1 movement/state replay and long runs remain unverified. The accepted-source ABI registry remains incomplete beyond priority. This port family adds no matching credit.
