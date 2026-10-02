# Local browser execution

This surface runs the actual static ARM port in a dedicated ES-module worker. It mounts the owner's approved dump, timing schedule, movie and complete initial user tree as readonly WORKERFS input. The existing compiled CPU/HLE/PICA/software renderer runs on the SDK's proxy pthread. The page receives real captured RGBA screens, not packaged game images. Every output file and directory is exported to the local server for the unchanged differential comparators.

This is a finite recorded replay. The optional input/audio profile observes the delivered movie input and the original HLE PCM, then offers explicit playback of that completed browser-generated sound. Continuous gameplay, live browser input, synchronized audio streaming, persistent saves, World 1-1 semantic replay and complete rank-O source adapter coverage remain separate work. The verified priority source adapter and the existing address-based replacement interface remain unchanged.

## Build

Use the accepted platform/translation builders first, with their successful actual verification receipts. From the browser checkout, source the primary checkout's development environment, then:

```sh
python tools/static_recompiler/build_browser_execution.py \
  /Users/exgota/super-mario-3d-land-browser/build/root_webassembly_platform/build/pinned_platform_final \
  build/browser_module \
  --platform-verification /Users/exgota/super-mario-3d-land-browser/build/root_webassembly_platform/build/final_source_verification.json \
  --emsdk /Users/exgota/super-mario-3d-land-browser/build/port_tools/emsdk \
  --node /Users/exgota/super-mario-3d-land-browser/build/port_tools/emsdk/node/24.19.0_64bit/bin/node
```

Outputs must be absent children of this checkout's ignored build/. The builder validates sealed actual execution/audit/entropy receipts, audited provider identities and all four notices. It reproduces the entire verified Node wasm byte for byte from the complete current link closure before allowing the browser link. It recompiles the unchanged port main with its entry symbol renamed, adds a browser entry and bounded input-identity helper, then links the real archives with the browser-specific filesystem/exports. Original artifacts remain unchanged. The browser entry stops and flushes the logger on the application pthread before normal SDK exit begins. It preserves native destructors and the actual onExit callback. It adds no Node filesystem fallback, alternate ARM interpreter, synthetic CPU/GPU provider or game data.

The initial profile inherits the verified 18-thread pool, 8 MiB stack and shared memory bounds of 1 GiB initial/2 GiB maximum. These are desktop test bounds, not a mobile memory or realtime-performance claim. Emscripten 6.0.10 is the tested SDK. Snapshot directory installation depends on that SDK's actual WORKERFS.createNode hook; do not assume this internal API survives an SDK upgrade.

## Serve and verify

Run a local server with the movie and snapshot paired to the selected reference capture:

```sh
python tools/static_recompiler/serve_browser_execution.py \
  build/browser_module build/browser_server \
  --block-schedule /Users/exgota/super-mario-3d-land-browser/build/root_native_block_scheduling/build/native_block_schedule_full/block_schedule.bin \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_rendering/reference_presentation_60 \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --presentation-limit 60 --port 8765
```

Open the printed localhost URL, select the approved owned EU .3ds copy and run the preview. The server binds only 127.0.0.1 and serves isolation headers. It does not serve the dump. Replay sidecars stay local and are fetched as readonly browser Files. For the first GPU swap instead of a screen presentation, use --first-swap and its paired first-swap reference snapshot.

For a mechanical real-browser check, use the pinned official Playwright CLI (0.1.22), available through npx, and an installed Chrome:

```sh
python tools/static_recompiler/verify_browser_execution.py \
  http://127.0.0.1:8765 build/browser_verification \
  --dump /Users/exgota/super-mario-3d-land-browser/build/root_port_reference/owned_dump.3ds \
  --server-output build/browser_server \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_rendering/reference_presentation_60 \
  --exercise-controls --headed --keep-open
```

The verifier selects the actual File, waits for actual shutdown, checks the complete exported tree and unchanged inputs, invokes the existing strict comparator and hashes both displayed canvas buffers against the original RGBA payloads. With --exercise-controls it first rejects a wrong-sized file, stops an active download and verifies a clean new capture. Desktop/mobile screenshots stay ignored; actual PNG dimensions, document geometry and loaded fonts are checked and recorded. The mobile review viewport is 390 × 1200, including the optional sound action, so both screens are captured without a short-viewport scrollbar crop. Use --first-swap in both server and verifier for the GPU-only boundary. The server's first-swap movie/snapshot comes from reference_fixed_io_0; the verifier's corrected floating-point oracle is reference_updated_floating_point_control_0. Each run receives a fresh capture identifier. The verifier closes its own browser unless --keep-open is explicit.

## Recorded input and sound

The version-one configuration accepts optional boolean input_capture and audio_capture options, absent options defaulting to false. The server supplies both explicitly. --observe-input captures movie-delivered HID; --observe-audio also enables input capture and the existing HLE observer. Both require a software-presentation boundary. No recording script or live device state replaces the readonly movie. Capture settings preserve the original null sink, audio event ticks and sample counts.

For the verified menu movie, use the original input capture for serving the movie/snapshot and the original stock audio replay as the comparison oracle:

```sh
python tools/static_recompiler/serve_browser_execution.py \
  /Users/exgota/super-mario-3d-land-browser/build/root_browser_execution/build/browser_module_submission \
  build/browser_audio_server \
  --block-schedule /Users/exgota/super-mario-3d-land-browser/build/root_native_block_scheduling/build/native_block_schedule_full/block_schedule.bin \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_input/reference_scripted_360 \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --presentation-limit 360 --observe-audio \
  --wall-time-seconds 360 --port 8782
python tools/static_recompiler/verify_browser_execution.py \
  http://127.0.0.1:8782 build/browser_audio_verification \
  --dump /Users/exgota/super-mario-3d-land-browser/build/root_port_reference/owned_dump.3ds \
  --server-output build/browser_audio_server \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_audio/reference_scripted_360 \
  --movie /Users/exgota/super-mario-3d-land-browser/build/root_port_input/reference_scripted_360/input_movie.ctm \
  --observe-audio --exercise-controls --headed --keep-open --timeout-seconds 600
```

The audio profile invokes unchanged compare_audio_capture.py, including exact HID/movie/render checks and varying PCM. Input-only uses unchanged compare_input_capture.py. These are the existing menu-observation policies; they are not general acceptance policies for every possible silent or single-channel input sequence. A replay directory may lack its own movie, so pass the original --movie explicitly.

The worker validates complete input/audio outcomes, event timing, contiguous sample offsets, PCM extent and the selected presentation before exporting. The page copies only acknowledged actual PCM chunks, at most 64 KiB each. It exposes sound only after successful capture shutdown, with a 64 MiB PCM bound. Signed-16 little-endian pairs map directly to two Float32 channels by division by 32768, at the original nominal 32728 Hz. No gain normalization, gap filling, channel mixing, synthetic samples or reference audio is used.

The explicit Play recorded sound gesture creates/resumes the AudioContext and starts one completed clip. Stop and natural end return the action to its ready state. A new file, run or failure disposes the previous clip. Browser output can resample to the device's rate; original PCM and source-buffer equality do not claim bit-exact speaker output. The verifier checks every original PCM-derived Float32 sample, equal-rate OfflineAudioContext output, no autoplay, explicit play/stop and natural completion. This is completed-clip playback, not synchronized continuous audio or proof of recognizable speaker output.

## Identity and completion

preRun mounts and configures the environment before C++ initialization. Full byte counts and SHA256 are checked through real C stdio/LibreSSL in 64 KiB requests before guest execution. Snapshot files and all directories have distinct inventories; empty directories are preserved through mounting, recursive capture and export. The unchanged host rewrites log/reference_capture.log before snapshot recording, so its raw host wall times/paths are preserved and reported separately from console-state identity. No log bytes are normalized.

callMain returns proxy-launch status, not capture success. Completion requires onExit(0), a complete validated GPU outcome, zero recorded ARM interpreter/JIT fallback, all acknowledged exports and final worker shutdown. The page owns a watchdog and AbortController before sidecar downloads. Stop/failure terminates only that session; late asynchronous work cannot fail a later one. A forced stop never counts as orderly C++ shutdown.

Run the independent C ABI refusal controls against the same local server:

```sh
python tools/static_recompiler/verify_browser_input_identity.py \
  http://127.0.0.1:8765 build/browser_identity_verification
```

This opens its own browser, mounts tiny synthetic readonly Files, invokes the actual compiled stdio/LibreSSL helper and checks exact/empty files, bad lengths/hashes/paths and denied writes. It loads no owned dump or guest main. Terminating that fixture worker earns no game-capture shutdown claim.

Exports use one acknowledged ordinary buffer at a time, at most 64 KiB, never the shared heap or original input Files. Bounds cover file/directory counts, depth, bytes, logs, event lines and encoded metadata. Output chunks must match the server's exact manifest offsets and sizes. All capture writes are under ignored build/. Byte-exact decompilation credit remains zero for this port family.
