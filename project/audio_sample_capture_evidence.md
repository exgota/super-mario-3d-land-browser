# Native audio sample capture evidence

## Ownership and frozen inputs

Root/audio-sample-capture starts at main `f3034bb6f1dfb12f700989f28cf7e89246f3a774`, after accepted input integration `f58c0b956812f60840aa0bb55dc1b77297491602`. Root owns the optional source-only Azahar audio observer, comparator and documentation. The single helper independently audited HLE generation, implemented the comparator and verified its contracts. No Game/lib/config/map changes, ranks, ledger changes or exact-function claims belong to this submission.

The observer applies last after the accepted capture, deterministic-I/O, static-execution, rendering and input patches to official Azahar `662d412123305a9f4be94dd3dc73ddf91a18c55e`. Dynarmic `e77b1ba0b7da7cbe93021b01a663acfe7c4dd516` retains the separately accepted ARM64 FPCR correction. All downloaded sources, builds, movies, snapshots, audio and images remain ignored. No Nintendo source or SDK input was used.

| Final source | SHA-256 |
|---|---|
| Audio patch | `acc2d3e54e6a6f2d22b921ca81368d2e7aabc93fad1a843facf27e6303f5d928` |
| Audio comparator | `30ffa2d310564a744a769ab786afc530f33766a61502dc798cd6852c5e7ee1af` |
| Resulting CMake source | `15b959eca05b82819b53dcfe86e58981b774943c02502d63ef4a6d1c4c884a48` |
| Resulting headless source | `89b2adfa401f159b18cb3d207731231a788274fe4b1f1a6e2450ef714b8eec66` |
| Resulting DSP interface | `1555510ea814fbadf683698c4cf1dbc4be8a34bf0e321d1d369f744190c04e95` |
| Audio header | `44e9b7548e09f7b14a7a626be65b1510c98fd2b121061d656b669a02b29da601` |
| Audio implementation | `c65c3fccf9c47617584a452cb066ba03656eff70ff2ac93dfac910c84a38d3de` |

The full ordered patch sequence passes independent temporary-index application to the pinned source. Its resulting tree is `29fc1fd98dede81513d0f711811277184b8e68ea`. The built files agree with those index blobs. Receipt: `build/audio_patch_sequence_verification.json` in this branch's worktree. Both stock and native executables build successfully with one compile slot. A fresh dependency download and clean rebuild on another machine remain unverified.

## Observer semantics

The optional `ROOT_PORT_CAPTURE_AUDIO=1` tap copies each original 160-pair HLE frame after the sink check and before immediate submission or FIFO push. It serializes interleaved signed-16 stereo samples explicitly as little-endian bytes. It never drains or alters the existing null-sink FIFO, never adds samples and stops at the selected natural GPU presentation. Realtime scaling and stretching are disabled. Any other flag value is refused before guest loading. The active observer refuses the single-sample LLE path and bounds the payload to 64 MiB.

Each event retains sequence, emulated ticks, renderer frame, nominal rate, channel count and contiguous sample/byte offsets. The nominal rate is 32728 Hz; the HLE tick interval corresponds to 32728.498046875 sample frames per second. Equal ticks are legal for overdue callbacks. No host-duration normalization occurs. The footer describes complete generated output, with no tail flush or silence after capture.

The comparator checks schema, types, order, timing, offsets, extents, footer and presentation boundary before requiring exact raw event bytes and PCM. It also requires complete input/render checks, nonzero varying samples in both channels and no audio or Movie errors. Optional WAV output wraps unchanged samples in a standard PCM16 header. Preview and report paths must be absent children of ignored build storage.

## Observed execution

The original accepted menu movie and its initial snapshot are replayed through stock, native, stock repeat and native repeat. All four exit successfully. Selected presentation 360 occurs at renderer frame 461. The input path visibly advances from Welcome to the StreetPass information page, as previously verified; this is menu audio evidence, not World 1-1 gameplay.

| Check | Observed result |
|---|---|
| HLE blocks | 1350 |
| Stereo sample frames | 216000 |
| PCM bytes | 864000 |
| Nonzero samples per channel | 77571 |
| Range per channel | -10428 through 12493 |
| RMS per channel | 1217.7890544331651 |
| Different left/right pairs | 0 |
| Input polls preserved | 1802 |
| GPU events/ticks preserved | 4583 |
| PICA payload bytes preserved | 8642912 |
| Screen/framebuffer bytes preserved | 691200 / 345600 |
| Native CPU0 guest instructions per replay | 393977876 |
| Native CPU1 guest instructions | 0 |
| Interpreter/JIT fallback executions | 0 |

Every sample and audio event/tick agrees across stock/native/repeats. PCM SHA-256 is `474c6170eb8aa8f7a604f1a4caaff03170714546bad3e95563094624646bf923`; audio event SHA-256 is `47ea300647c7d8d7094fe3a7aff9784ca552cbd789f748b128f01eaa79615a29`. The last audio event is at tick 2991885025, before presentation tick 2991994001. Capture completion is at tick 2992001208. No audio is appended beyond the selected presentation.

Stock runs took 37.206 and 31.288 seconds; native runs took 46.183 and 57.381 seconds on the recorded M4 host. These are individual wall times, not a sustained performance estimate. Final stock executable SHA-256 is `a87f64fb43d5632a82e042161d85b1ac45a749bcc640292d62705b340751dcfd`; native executable SHA-256 is `767bf3e9bc0bab18d8a1b8ad7eff9a50e85d6f0b339503ef5609b23aad2e67e5`. The existing sealed library is `e79538983be80ccd0b730fc3985162a6fef299f94bab7bcb8a3a1905541e6131`; its schedule is `fbe0bdfdf8479deacac0c113c8d975b0094abe9e2eb8cfa7ce5cbcf9d6a9c10c`.

## Receipts and reproduction

Follow [AUDIO.md](../tools/static_recompiler/AUDIO.md) for patch application, build, capture, comparison and independent WAV decoding. Historical captures are under primary `build/root_port_audio/`. This branch's ignored `build/` contains:

- `final_native_audio_comparison.json`, `final_stock_audio_repeat_comparison.json` and `final_native_audio_repeat_comparison.json`: every exact sample/event and prior input/render comparison passes on the frozen comparator.
- `input_render_preservation_comparison.json`: enabling audio preserves the previously accepted input/GPU/PICA/pixel stream.
- `audio_capture_verification/fixtures_verification.json`: all 66 independent contract controls pass. They exercise malformed schema, ordering, truncation, timing, boundary, sample differences, inherited input/render failure and preview constraints.
- `audio_capture_verification/final_handoff.json` and `actual_wav_decode.json`: independent helper source identity, actual comparisons and exact PCM decoding.
- `audio_configuration_refusal.json`: invalid flag returns 2 before guest loading.
- `native_audio_preview_probe.json` and full decode logs: native WAV is stereo PCM16 at 32728 Hz, 6.599853 seconds; complete independent decoding exits 0 with no error.

These observations support port milestone 4 as bounded native HLE audio output. Recognizable content and live speaker playback were not assessed. Identical menu channels do not prove later stereo separation. Both executions share public HLE DSP approximations, so parity does not establish hardware DSP accuracy. Browser/WebAudio playback, long runs, World 1-1 state replay and the complete rank-O native-source ABI registry remain unverified. Recompiled coverage stays 2437712 instruction bytes. No matching credit follows.
