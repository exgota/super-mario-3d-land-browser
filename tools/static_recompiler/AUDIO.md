# Native HLE audio output

The optional audio observer records the pinned Azahar HLE mix before host FIFO submission. Native and stock CPU execution replay the same unmodified movie and initial user snapshot. The comparator requires exact generated stereo PCM, audio events/ticks and the existing input/GPU/pixel checks. This establishes bounded native audio output. Speaker/browser playback and real-hardware DSP accuracy are separate checks.

## Build and capture

Follow [INPUT.md](INPUT.md), [RENDERING.md](RENDERING.md) and [NATIVE_PLATFORM.md](NATIVE_PLATFORM.md) to obtain the pinned public source, accepted input tools and sealed native library. Apply the optional audio patch last:

```sh
. ./development_environment.sh
git -C build/port_tools/azahar_reproduction apply --check \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_audio_capture.patch"
git -C build/port_tools/azahar_reproduction apply \
  "$PWD/tools/static_recompiler/azahar_reference/azahar_audio_capture.patch"
build/port_tools/azahar_reproduction_python_packages/cmake/data/bin/cmake \
  --build build/port_tools/azahar_reproduction_build \
  --target azahar_gpu_capture azahar_static_execution -j 1
```

Record the bounded menu input using INPUT.md. Observe audio while replaying its original movie and snapshot, into absent ignored output directories:

```sh
ROOT_PORT_CAPTURE_PRESENTATION=360 ROOT_PORT_CAPTURE_INPUTS=1 ROOT_PORT_CAPTURE_AUDIO=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_gpu_capture \
  build/root_port_reference/owned_dump.3ds build/root_port_audio/reference \
  build/root_port_input/reference/input_movie.ctm \
  build/root_port_input/reference/initial_user_state
ROOT_PORT_CAPTURE_PRESENTATION=360 ROOT_PORT_CAPTURE_INPUTS=1 ROOT_PORT_CAPTURE_AUDIO=1 \
build/port_tools/azahar_reproduction_build/bin/Release/azahar_static_execution \
  build/platform_timed_translation/translated.dylib \
  build/root_port_reference/owned_dump.3ds build/root_port_audio/native \
  build/root_port_input/reference/input_movie.ctm \
  build/root_port_input/reference/initial_user_state
python tools/static_recompiler/compare_audio_capture.py \
  build/root_port_audio/reference build/root_port_audio/native \
  --movie build/root_port_input/reference/input_movie.ctm \
  --report build/audio_native_comparison.json --preview-directory build/audio_preview
```

Repeat stock and native playback into separate absent directories. Compare both repeats with the same original `--movie`. Recording options and script injection must be unset during playback. The observed build is macOS arm64; Linux and browser builds remain unverified. Every report and preview must be an absent path in the invoking checkout's ignored `build`.

## Observation convention

The tap sits after `DspInterface::OutputFrame` verifies its sink, before `ImmediateSubmission` or `fifo.Push`. It copies the original 160 left/right signed-16 pairs without altering the frame, FIFO or sink. The configured null sink never drains the existing FIFO. Observation preserves every generated frame before that original loss; it does not drain it or request more audio. Realtime audio scaling is explicitly disabled, as is host stretching.

`audio_pcm_s16le.bin` stores explicit little-endian interleaved pairs. Each `audio_events.jsonl` record identifies sequence, emulated ticks, renderer frame, nominal sample rate 32728, two channels, 160 sample frames and contiguous sample/byte offsets. Tick equality is allowed for overdue callback catch-up. The observer freezes when the selected GPU presentation completes. It adds no silence, tail, resampling or stretch flush afterward. Its final outcome records the complete block/sample/byte counts. The one-sample LLE path is refused while the HLE observer is active.

The comparator requires complete audio and software-presentation outcomes, no audio/Movie errors, exact event bytes including timing, exact PCM, nonzero varying samples on both channels and all prior input/render checks. It validates every offset, extent, field type and boundary. Statistics describe the samples and cannot rescue a mismatch.

The optional WAV preview wraps the unchanged PCM using Python's standard library, two channels, two-byte samples and nominal rate 32728. For independent file validation:

```sh
ffprobe -v error -show_entries stream=codec_name,sample_rate,channels,duration \
  -of json build/audio_preview/native_audio.wav
ffmpeg -nostdin -v error -xerror -i build/audio_preview/native_audio.wav -f null -
```

Nominal sample rate differs slightly from the emulated tick ratio, 32728.498046875 sample frames per second. Exact emulated tick comparison remains mandatory. Playback duration uses the declared nominal WAV rate. Owner-derived audio, movie, state and GPU/image files must never enter Git.

## Observed scope

The menu replay generates 1350 HLE blocks, 216000 stereo sample frames and 864000 PCM bytes. Both channels contain 77571 nonzero samples, range from -10428 through 12493, and RMS1217.7890544. Left and right are identical on this path; channel separation in later scenes is unverified. Native/stock repeats match every sample and audio event/tick while preserving1802 HID polls,4583 GPU events/ticks,8642912 PICA bytes and bothscreen/framebuffer streams. [The evidence note](../../project/audio_sample_capture_evidence.md) identifies final source hashes and receipts.

The local WAV probes as stereo PCM16 at32728 Hz,6.599853 seconds, and decodes completely without error. Recognizable content and live speaker playback were not assessed. HLE uses the same public reference approximations for DSP semaphores, limiting and compression; parity does not establish real-hardware DSP equivalence. World1-1 state replay, WebAudio/browser playback, long runs and the complete accepted-source ABI registry remain open. No matching credit follows.
