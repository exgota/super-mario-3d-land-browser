# Root port runtime state

## Ownership and submissions

Root/audio-sample-capture starts from main f3034bb6f1dfb12f700989f28cf7e89246f3a774. Root owns the optional source-only HLE PCM observer, comparator and documentation. Its single helper completed independent contract and actual capture checks. No matching inputs, ranks, ledger changes or claims. Final audio evidence is project/audio_sample_capture_evidence.md. Submission follows the final source-only commit; inspect .integrator/results/ for the verdict before any retry.

Root/input-record-playback 4acb6b4ec was accepted at f58c0b95. Root/rendered-frame-capture 55540caf4 was accepted at c57bceef. Native block scheduling, platform and FP control are accepted. Calendar placement was rejected as nonexact; root now works only on runtime. Only the integrator moves main, ranks and the ledger.

## Observed port scope

Milestones 1, 2 and 3 have bounded native GPU, title-logo pixels and causal menu-input evidence. Milestone 4 has bounded native audio evidence: stock/native/repeats match 1350 HLE blocks, 216000 stereo sample frames, all audio events/ticks, 1802 HID polls, 4583 GPU events/ticks, 8642912 PICA bytes and both screen/framebuffer streams. Each native replay executes 393977876 CPU0 guest instructions with zero interpreter/JIT fallback. Final-source receipts are under build/root_audio_sample_capture/build; AUDIO.md provides reproduction.

The WAV preview independently decodes as 6.599853 seconds of stereo PCM16 at 32728 Hz. Listening and live speakers were not assessed. Shared HLE parity does not prove hardware DSP accuracy. World 1-1 state replay and browser execution remain open. Recompiled coverage is 2437712 instruction bytes. The rank-O source replacement registry beyond the priority conversion remains incomplete. No matching credit follows runtime observations.

## Next tasks

1. Read the audio submission verdict and report it. Continue independent World 1-1 work while it waits.
2. Record menu entry and a World 1-1 input path through the original movie interface. Request player/camera/RNG/timer/coin observation layouts from Pro; do not reconstruct classes in this lane.
3. Compare fixed-frame game state and whole-level input replays, then build browser execution and WebGPU/WebAudio presentation.

No owner question blocks independent work. Downloaded tools and every owner-derived library, capture, image, audio sample, snapshot and movie stay ignored. Run at most one helper. Keep unsupported execution fail-closed.
