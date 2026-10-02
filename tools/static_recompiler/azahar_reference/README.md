# Azahar reference capture

This directory contains project-authored source instrumentation and build documentation for official Azahar commit [`662d412123305a9f4be94dd3dc73ddf91a18c55e`](https://github.com/azahar-emu/azahar/tree/662d412123305a9f4be94dd3dc73ddf91a18c55e). Download and build Azahar under ignored `build/port_tools/`. This directory contains no downloaded emulator checkout, executable, game capture, input movie, console state or game data.

`azahar_capture.patch` adds a non-Qt software capture frontend, a CPU backend registration point and raw GPU instrumentation. `azahar_deterministic_io.patch` is a separate correction to host-I/O timing in the configured reference. Apply both to the pinned source. The ARM64 host also needs the independently verified `azahar_floating_point_control.patch` in its pinned Dynarmic submodule. [FLOATING_POINT_CONTROL.md](FLOATING_POINT_CONTROL.md) explains the original defect, source-only correction and reproducible bounded and capture checks. Historical two-patch captures remain preserved. [BUILD.md](BUILD.md) contains the source, dependency, configure, build, recording, playback and exact-comparison commands. [source_provenance.json](source_provenance.json) contains final source hashes, patch hashes, build configuration, observed results and explicit gaps. [dependency_revisions.txt](dependency_revisions.txt) records the pinned upstream submodules. [LICENSE.md](LICENSE.md) identifies the source patch's GPL-2.0-or-later terms and upstream authors.

## Capture convention

Frame zero is the complete startup prefix from process load through the first `GPU::SetBufferSwap(screen_id == 0)`. The prefix includes initial setup, direct hardware-register writes, bottom-screen operations and VBlanks. It is a submitted game-frame boundary, not proof that a visible image has been presented.

The frontend records every original 32-byte GSP command before `GPU::Execute` processes it, every initial PICA command list before `PicaCore::ProcessCmdList`, actually followed chained lists when the active command buffer changes, buffer swaps, direct GPU register writes, color-fill changes and VBlanks. `gpu_events.jsonl` retains original guest ticks and ordered command metadata. Binary PICA payloads preserve submitted little-endian words. The software GPU executes these submissions synchronously before returning to GSP IPC.

The stock oracle uses Dynarmic, a software renderer, old 3DS configuration, HLE DSP, null audio sink, fixed initial clock, no input devices, no plugins, no right-eye suppression and no frame limiter. The reference input movie is prepared before system loading and started before CPU execution. An initial console/save-state snapshot is copied after loading and before CPU execution. Playback needs that movie and snapshot together. New CFG state alone is insufficient because upstream initializes random console identity.

## Isolated host-I/O correction

The frontend enables upstream `deterministic_async_operations` and disables ordinary `async_fs_operations`. Upstream `HLERequestContext::RunAsync` then executes its operation synchronously. However, `File::Read` still subtracts measured host read duration from its simulated read latency on large ROMFS reads. This changes guest wakeup ticks between runs and breaks typed movie replay.

The separate patch changes one condition: deterministic mode returns the existing full simulated read delay. Normal async mode keeps its existing host-duration subtraction. CPU instruction execution, instruction costs, PICA decoding and GPU register semantics are unchanged. This is a documented correction to the configured Azahar reference. Pristine upstream movie replay is not claimed to be deterministic.

The original default-async recording reached the first top-screen swap with 348 GSP records and eight PICA lists. Repeated movie playbacks produced either 336 records/five lists or 348 records/eight lists, and input-type desync errors. The upstream deterministic setting alone still produced host-dependent ticks and desync. Those failed local trials remain in ignored scratch; none was normalized into an acceptance result.

## Observed reference verification

After applying both patches, one stock recording and two fresh movie/snapshot playbacks each exited successfully. All three produce exactly identical 528-event streams, including the original guest ticks, and byte-identical payloads. Each records 348 GSP commands, eight PICA lists totaling 79,936 bytes, 68 direct hardware-register writes, 102 VBlanks and one first top-screen swap. Movie errors are zero. The common event SHA-256 is `ed7a39625518e559be6c0bc8f5a50d05f088161bc9256b6a8a169eab114f9859`. Recording and playback took 1.517, 1.157 and 0.986 seconds on the recorded host.

Both source patches pass `git apply --check` against clean pinned official source. The original owner dump was rehashed after emulator execution and remains unchanged. Historical result files are local under ignored `build/root_port_reference/`; this package ships source provenance and aggregate observations only. Fresh recordings choose a different base tick, so a new record/replay pair must match each other and need not reproduce the historical event hash.

This validates reference replay. It does not prove port milestone 1. No native static/reference stream comparison has passed in this receipt. Chained-list handling was not reached by the observed startup. Whole-program CPU equivalence, visible rendering, input-driven gameplay, audio, World 1-1, browser execution, later-frame state snapshots and another machine's reproduction remain unverified.

## Native backend boundary

Link a separate native executable to `root_port_headless_capture`. Register `RootPortCapture::CpuBackendFactory` before calling `RootPortCapture::RunHeadlessCapture`. `System::Init` uses the registered `Core::ARM_Interface` implementation for every CPU core; the stock oracle registers no factory. Native and stock executions otherwise share the same capture frontend, settings, movie and initial state.

The native adapter must preserve CPU/VFP/CPSR/TLS state, page-table semantics, timer budgets, service exits and rescheduling. Route guest memory through `Memory::MemorySystem` and services through `Kernel::SVCContext`. A missing translated address or unsupported instruction must stop execution. Milestone evidence requires an actual first-swap capture, explicit zero interpreter/JIT fallback execution and an exact reference comparison. Registering a backend or linking translated functions is insufficient.

Reusing the same HLE and GPU host isolates CPU translation differences. It cannot detect modeling errors shared by both runs. The matching ARMCC build and ranks remain separate from these port checks.

## Verified ARM64 floating-point control correction

Pinned Dynarmic updates guest FPSCR after VMSR but leaves hardware host FPCR at the Run-entry mode. A bounded test of all 16 rounding transitions, three divisors and continuous/split execution finds 22 continuous-run result mismatches against original ARM11MPCore execution. The one-line host FPCR update removes every mismatch across all 96 cases and leaves split execution correct. This changes the host implementation to follow the guest control register. It does not change the original executable or native game arithmetic.

Two corrected same-input first-swap captures match each other exactly. Their raw event/timing bytes remain equal to the historical reference; 16 PICA payload words change. Every one of the resulting 79,936 PICA bytes agrees with the existing native execution. Those historical native runs still differed in event timing. The subsequently accepted native block scheduler now matches all raw event ticks as documented in [NATIVE_PLATFORM.md](../NATIVE_PLATFORM.md); milestone 1 passes. The previously relayed 18-word estimate was incorrect; the full uint32 audit measured 16. [floating_point_control_provenance.json](floating_point_control_provenance.json) records the final source and observed aggregate checks.

## Natural software presentation

The optional `azahar_render_capture.patch` continues to a selected natural presentation after the first top-screen submission. [RENDERING.md](../RENDERING.md) provides native/stock record, replay, exact pixel comparison and ignored preview commands. Presentation 60 displays the title logo and passes two native replays against stock, including every event/tick, command payload and screen byte. The first presentation is black and is retained as plumbing evidence only. Browser/WebGPU presentation and gameplay remain unverified.

## Deterministic nonzero input

The optional `azahar_input_capture.patch` records a bounded held-state script through the existing HID/movie calls and observes delivered HID ring entries. Playback uses the original movie only. [INPUT.md](../INPUT.md) describes the typed-movie/input/pixel comparator and a same-state neutral control. Repeated native and stock replays match all1802 delivered polls and the visible Welcome-to-StreetPass response. Browser devices and World1-1 input remain unverified.

## Native HLE audio observation

The optional `azahar_audio_capture.patch` records original stereo PCM and emulated timing before host FIFO submission. [AUDIO.md](../AUDIO.md) describes native/stock/repeat comparison and lossless local WAV preview. The bounded menu replay matches all 216000 stereo sample frames and audio events while preserving input/GPU/pixel checks. Live speaker/browser playback, World 1-1 and real-hardware DSP equivalence remain unverified.

## Bounded longer replays

The optional `azahar_replay_duration.patch` makes the host watchdog configurable, retaining its 120-second default and existing emulated timing. [REPLAY_DURATION.md](REPLAY_DURATION.md) describes the bounded option, refusal behavior and evidence. Longer World 1-1 replay still requires original movies and every Section 7 state observation.

## Default frontend applet interfaces

`azahar_default_applets.patch` registers the pinned upstream default Mii selector and keyboard for the headless frontend. [DEFAULT_APPLETS.md](DEFAULT_APPLETS.md) explains the configured reference choices, original-movie comparison and bounded file-selection evidence. Semantic level state and browser applet interfaces remain unverified.

## Passive RAM observation

`azahar_state_observation.patch` adds optional bounded RAM reads at existing natural software presentations. [STATE_OBSERVATION.md](STATE_OBSERVATION.md) documents the plan, unavailable-state rules and raw replay comparer. Synthetic memory controls and stock/native platform preservation pass; game field meanings and completed-update alignment remain unverified.
