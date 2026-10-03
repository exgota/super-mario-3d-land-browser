# Local browser execution

This surface runs the actual static ARM port in a dedicated ES-module worker. It mounts the owner's approved dump, timing schedule, movie and complete initial user tree as readonly WORKERFS input. The existing compiled CPU/HLE/PICA/software renderer runs on the SDK's proxy pthread. The page receives real captured RGBA screens, not packaged game images. Every output file and directory is exported to the local server for the unchanged differential comparators.

This is a finite capture. The default profile replays a recorded movie. The optional input/audio profile observes delivered movie input and original HLE PCM, then offers explicit playback of that completed browser-generated sound. Optional live-button, circle-pad and touch profiles record browser input through normal HID. Optional frame output shows completed screen samples while the run executes. Unbounded interactive sessions, sustained frame rate, synchronized audio streaming, persistent saves, World 1-1 semantic replay and complete rank-O source adapter coverage remain separate work. The verified priority source adapter and the existing address-based replacement interface remain unchanged.

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

Outputs must be absent children of this checkout's ignored build/. The builder validates sealed actual execution/audit/entropy receipts, audited provider identities and all four notices. It reproduces the entire verified Node wasm byte for byte from the complete current link closure before allowing the browser link. It recompiles the unchanged port main with its entry symbol renamed, adds a browser entry, bounded input-identity helper and public host-button factory, then links the real archives with the browser-specific filesystem/exports. Original artifacts remain unchanged. The browser entry stops and flushes the logger on the application pthread before normal SDK exit begins. It preserves native destructors and the actual onExit callback. It adds no Node filesystem fallback, alternate ARM interpreter, synthetic CPU/GPU provider or game data.

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

## Live A-button recording

Use a newly built module with the button bridge and add --live-button to the server command. Serving still identifies the original readonly movie and full initial state. Only that movie's base-tick metadata seeds the new recording. The runtime receives schedule/dump/output arguments, with no positional movie or playback snapshot. ROOT_PORT_RECORD_INITIAL_USER_STATE supplies the full readonly starting tree; ROOT_PORT_RECORD_BASE_TICKS supplies its identified decimal clock. The entry refuses a simultaneous live-button and positional playback profile. Both input and audio observations are required, with a finite software-presentation limit.

```sh
python tools/static_recompiler/serve_browser_execution.py \
  build/browser_module build/browser_button_server \
  --block-schedule /Users/exgota/super-mario-3d-land-browser/build/root_native_block_scheduling/build/native_block_schedule_full/block_schedule.bin \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_input/reference_scripted_360 \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --presentation-limit 480 --live-button --port 8787
python tools/static_recompiler/verify_browser_button_capture.py \
  http://127.0.0.1:8787 build/browser_button_verification \
  --dump /Users/exgota/super-mario-3d-land-browser/build/root_port_reference/owned_dump.3ds \
  --server-output build/browser_button_server --input-method pointer --screenshots --keep-open
```

Hold A accepts pointer or focused Space/Enter. Releases cover pointer up/cancel/lost capture, key up, button/window focus loss, hidden document and session end. Input becomes available only after the actual registered device has been polled. The outer worker reads only exported atomics; renderer-frame samples originate on the CPU's ordinary HID path. Requests acknowledge queued held state, never delivered game input. Polls and the actual CTM provide delivery evidence. A short pulse can coalesce between polls. Without --frame-output, screens and recorded sound arrive only after the finite run ends.

The worker validates the new CTM after normal Movie.Shutdown: bounded file extent, header/revision/program/clock, typed record order and padding, pad/touch/header counts and every delivered HID value. Input, audio and GPU outcomes must be complete; Input/Movie/Audio errors refuse completion. Correctly scoped input messages arriving after guest shutdown earn refusal and do not invalidate successful exports. Requests are sequenced and bounded to 4096 per capture.

The verifier also supports keyboard, focus-loss and neutral input methods. It checks actual press/release or strictly neutral polls, CTM delivery, zero CPU fallback, all output files/directories, readonly inputs and displayed canvas bytes. This recording check alone does not certify stock parity. Replay each new CTM and its exported initial_user_state with the unchanged stock azahar_gpu_capture, at the same presentation/input/audio/payload/wall limits, then invoke unchanged compare_movie_replay.py with that new CTM. This general comparator permits unused input channels and exact silence while requiring every input/audio/GPU/tick/PICA/framebuffer/RGBA byte to match. Preserve original movie IDs and raw host logs. Use a separate neutral recording to establish the causal menu response.

## Circle-pad recording

Add `--live-circle-pad` to the finite server command. It selects a new recording with the same identified base clock and initial user tree as live A. It also requires input/audio observations. Its optional version-one `live_circle_pad_capture` field defaults to false. Circle-only recording keeps the A setter inactive; `--live-button` may explicitly enable A alongside it. Playback arguments and a live recording cannot be combined.

```sh
python tools/static_recompiler/serve_browser_execution.py \
  build/browser_circle_pad_module_final build/browser_circle_pad_server \
  --block-schedule /absolute/ignored/block_schedule.bin \
  --reference /absolute/ignored/recorded_reference \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --presentation-limit 480 \
  --live-circle-pad --frame-output --port 8796
python tools/static_recompiler/verify_browser_circle_pad_capture.py \
  http://127.0.0.1:8796 build/browser_circle_pad_capture \
  --server-output build/browser_circle_pad_server \
  --dump /absolute/ignored/owner_dump.3ds --sampled-frame 360 --screenshots
```

Four held controls follow the physical pad's direction. Pointer holds, focused arrow keys, Space and Enter share coherent paired positions. Opposite directions cancel; diagonals use `(±108, ±108)` within the radius-154 disk. Releases cover pointer up/cancel/lost capture, key up, group/window focus loss, hidden document and session end. The controls enable only after normal CPU device polls. Acknowledgments and sampled requested positions certify no delivery by themselves.

The public `Input::AnalogDevice` returns a unit-circle tuple, with positive x right and positive y up. One lock-free atomic word publishes both axes and lifecycle state. Invalid integers, components or radial extents refuse without changing it. Installation saves only the CirclePad profile and restores it after the driver returns. Successful cleanup prints an immutable poll receipt before normal SDK exit. The worker makes no native calls from `onExit`.

HID retains its normal scale, rounding, three-sample average and derived direction bits. The CTM records the averaged delivered position. Cardinal input from neutral ramps `51, 102, 154`; release decays `102, 51, 0`, including the corresponding negative values. The verifier checks signed cardinals, both diagonals, partial release and focus loss against the actual saved HID trace; reconciles every movie pad/touch record and the complete initial guest tree; requires both completed canvas identities/dimensions; and seals its own source before and after execution. Screenshot capture settles the mobile viewport before applying keyboard input. Its command receipts preserve timeout partial streams.

Replay the new `capture_<identifier>/input_movie.ctm` and `initial_user_state` with unchanged stock Azahar at the same finite boundary, then run the existing `compare_movie_replay.py`. Browser recording alone earns no stock parity or game-state claim. `project/browser_circle_pad_evidence.md` records the bounded actual executions, refusal controls and strict stock comparisons. Arbitrary clock endpoints, old calls across reinstall, foreign factory-name collisions, physical mobile performance and player movement are outside that evidence.

## Bottom-screen touch recording

Add `--live-touch --frame-output` to a finite server command. The optional version-one `live_touch_capture` boolean defaults to false. Touch uses the same identified recording clock and complete starting tree as the other live profiles. It requires input/audio observations and a finite presentation. Touch-only mode keeps A and circle setters inactive; those profiles require their explicit flags. Positional movie playback cannot enable live injection.

```sh
python tools/static_recompiler/serve_browser_execution.py \
  build/browser_touch_module_final build/browser_touch_server \
  --block-schedule /absolute/ignored/block_schedule.bin \
  --reference /absolute/ignored/recorded_reference \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --presentation-limit 480 \
  --live-touch --frame-output --port 8800
python tools/static_recompiler/verify_browser_touch_capture.py \
  http://127.0.0.1:8800 build/browser_touch_capture \
  --server-output build/browser_touch_server \
  --dump /absolute/ignored/owner_dump.3ds --sampled-frame 360 --screenshots
```

The verifier defaults to headless Chrome and closes its own test session. `--headed` requests a visible project preview explicitly. Keep at most one visible project preview, preserve active tests, and leave personal Chrome separate, as required by the browser lifecycle policy in `project/BRIEF.md`.

Touch becomes available only after actual CPU polls and a displayed bottom canvas of320×240. Startup can briefly produce shorter images; they keep rendering while touch waits for the complete target. Losing the full target releases a held contact before disabling input. Mouse/touch pointer coordinates map through the current owned canvas rectangle to integer x0..319/y0..239. Initial contacts outside the half-open target refuse. Captured drags clamp to its edges. Positive x is right, positive y is down. The owned canvas has no border, padding or transform; arbitrary external styling is outside that mapping contract.

One pointer owns a contact. Focused arrow keys select a point in eight-pixel steps. Space and Enter hold it, and one held source survives another source's release. Pointer up/cancel/lost capture, key up, canvas/window blur, hidden document, missing target and session completion release their respective sources. A focused crosshair marks the chosen point without changing game pixels. The finite4096-request budget reserves its final sequence for canonical release, disables further touch and lets the capture finish with a retry message.

The public `Input::TouchDevice` returns `(float,float,bool)`. One lock-free atomic word publishes x/y/pressed/lifecycle together. Pressed pixels map to normalized centers `(x+0.5)/320`, `(y+0.5)/240`; unchanged HID scales and truncates them. Release is exactly `(0,0,false)`. Invalid coordinates, pressed values and nonzero released coordinates refuse before mutation. Installation saves/restores `touch_device`, `use_touch_from_button` and `use_touchpad`, disabling both ordinary fallback sources for this explicit input owner. The CPU getter reads atomics only. It accesses no renderer, Core or guest layout. Cleanup prints the final device-poll receipt before normal SDK exit; the worker does not call exports after `onExit`.

The actual-browser verifier requires original HID/movie delivery for all four corners, interior points, a captured outside drag, mixed pointer/keyboard holds, partial keyboard release and focus loss. Each plateau must appear in the saved original poll interval, independently of request acknowledgment. It reconciles every CTM pad/touch record, native closure polls, exported files/directories and unchanged input seals, then requires both completed canvas dimensions/hashes. Desktop/mobile screenshots remain private. A resized desktop viewport does not prove physical mobile behavior.

Replay the new movie and exported `initial_user_state` with unchanged stock Azahar at the same clock and finite boundary. Invoke unchanged `compare_movie_replay.py` with that new movie explicitly. Exact replay is required separately from recording. Arbitrary clock endpoints, factory-name collisions, calls across reinstall, unbounded sessions and semantic gameplay remain outside this family's proof. `project/browser_touch_input_evidence.md` records executed evidence and preserved failed attempts.

## Sampled frames during execution

Add --frame-output to a finite software-presentation server profile. The optional version-one boolean frame_output defaults to false and rejects a GPU-only first-swap profile. It can accompany movie playback or live-button/circle-pad/touch recording. During playback, a neutral public input device supplies the CPU sampling callback; the original movie still supplies every delivered HID value, and the live setter remains inactive.

BrowserFrameOutput uses public RendererSoftware::Screen data on the application's ordinary HID callback after completed SwapBuffers. Top-left screen 0 and bottom screen 2 are copied as one pair. Software bytes already have landscape row-major orientation: display width is ScreenInfo.height, display height is ScreenInfo.width. Frame and sample ticks retain uint64 identity through low/high metadata words and decimal strings. Sample ticks describe the later HID observation, not the preceding presentation tick.

Two fixed slots each hold two 1 MiB screen arrays. Free, Writing, Ready and Reading states use lock-free atomic release/acquire ownership. A producer never changes a leased or unread slot. A busy next slot drops the new distinct frame once; repeated HID observations do not retry it. The worker acquires in publication order, validates metadata/pointer extents in the current exported heap view, copies into ordinary ArrayBuffers and releases synchronously before hashing or messaging. No heap buffer is transferred to the page. One acknowledged preview pair may be outstanding. Metadata receipts are bounded to 8192 pairs; complete raw frame accumulation is excluded.

Actual first-lease controls refuse invalid indices, screen 1 and stale generations without freeing the current lease. Immutable producer totals record unique frames, publications, full-slot drops, unavailable screen pairs, duplicate polls, frame gaps and geometry failures before normal exit. Unique frames reconcile to publications plus full drops plus unavailable pairs. The manifest separately records copied/acknowledged pairs and unconsumed publications. Native bridge calls stop before onExit; an already copied ordinary pair can finish its page acknowledgment during validation. Preview failures prevent final export.

The selected final presentation may stop before another HID callback. Its normal /capture pixels therefore replace the preview as a separate completion view. No frame duplication, timing adjustment, synthetic reference image, renderer/provider mutation or extended SDK runtime is used.

For a full recorded movie, run verify_browser_frame_output.py with the local URL, absent ignored output, --server-output, --dump and --reference containing that movie. It independently retains two actual running canvas pairs, checks their hashes against worker copies and requires the unchanged full movie comparator. --exercise-stop stops after real intermediate output and verifies a fresh capture. --screenshots captures running/completed desktop and mobile views. --thresholds selects two increasing renderer-frame sampling thresholds; the retained actual frames can be later than those thresholds.

Replay fresh stock checkpoints for those actual frames with the original full movie and complete starting tree. Derive each natural presentation from the validated final capture: target = sampled renderer frame - final renderer frame + final presentation. Use unchanged capture bounds/providers and require normal native exit and unchanged inputs. Then run compare_browser_frame_checkpoint.py with the successful sample result, sample index, stock checkpoint and absent --report. It invokes the unchanged rendered loader and requires exact paired original RGBA bytes, dimensions and frame identity. The unchanged full movie comparator intentionally cannot validate a prefix against a full movie's pad count. Do not truncate the movie or weaken that gate. Intermediate image equality and complete final input/audio/GPU/tick/PICA/framebuffer/RGBA parity remain distinct checks.

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
