# Browser original-audio streaming evidence

Branch `root/audio-stream-output` freezes integrator accepted main `ec5515d1946b941d1303c72879a4d3b5c2940a26`. Implementation commit `3d9e7b26b06155363439b0202496049f223d6e0b`. Root owns delivery, explicit action, actual checks and publication. One implementation helper supplied only the consumer/worklet. No reviewer agents. No Game/lib/config/map/ranks/ledger/Factory/oracle/provider changes. Matching claims and bytes are zero. Recompiled instruction bytes remain 2437712.

## Behavior

The optional `--stream-audio` profile tails the unchanged original PCM file read-only. It preserves the HLE mixer, null sink/FIFO, observer, guest timing and linked module. One generation/sequence/offset/hash-stamped packet waits for actual worklet insertion before another is offered. Packets contain 2048 stereo frames, followed by one aligned final tail. The consumer validates signed16 little-endian source and copies caller buffers. A preallocated 65536-frame stereo ring bounds queued source; capacity/acknowledgement have a 30-second deadline. Actual output arrays receive every source pair once, in order, divided by 32768 at a requested and checked 32728 Hz context. Missing source produces counted startup/underrun zeros. No accepted pair is dropped or stretched.

An explicit gesture creates/resumes the owned graph. Natural end waits for actual consumption and context closure. Stop cancels this stream and lets the guest capture continue, with completed-clip playback still available. The optional bounded output witness is disabled normally. It copies actual output channel arrays, including real source zeros and excluding only synthetic starvation zeros. It does not observe speaker/device conversion. Worker phase running denotes the outer worker before SDK exit observation, not an atomic gameplay update boundary. File growth after initial consumption separately demonstrates ongoing production.

## Final source seals

`runtime/port/browser/BrowserAudioFileStream.mjs` SHA256 `02136aa4271e9b8acf62fef7168bfd30da350d279bb93fe75b19e613518f5c8c`.

`runtime/port/browser/BrowserAudioWorklet.mjs` SHA256 `391ecaa5f62cf3921a7f8745285f8fc000ddb98e939c9ce5b88319f849aa18c2`.

`runtime/port/browser/BrowserStreamedAudio.mjs` SHA256 `ecaafa2dbffd1318f41213e2665bf9c3650533b86c8448fb1cf0993b7b9e904a`.

`runtime/port/browser/BrowserCaptureWorker.mjs` SHA256 `34204c0999e9bd8bbddf29b8a1882b11cd5d0fe1edac9add9c79890406028c64`.

`runtime/port/browser/BrowserCapturePage.mjs` SHA256 `9d25f374ae383aa2701d471e972c14b38f21d6ea74a786dfc6fc8c248c3f8225`.

`tools/static_recompiler/serve_browser_execution.py` SHA256 `6540c43760862427df3e2e653bf48607fbd44f0810e831d0e84ae116a881ed2d`.

`tools/static_recompiler/verify_browser_execution.py` SHA256 `c5b7f81b3258fbd3f8bdfd5794665617058f54e0bed3e4376ffe22c6358effaf`.

`tools/static_recompiler/verify_browser_audio_stream_controls.py` SHA256 `d5531de2520295674c14c432718afd4d8c92146f1e42159a479512fd5fcbc0c8`.

## Actual checks

All five new/changed JavaScript modules pass Node syntax checks; the server and both verification programs pass Python compilation. git diff --check passes. The matching source/build inputs and protected oracle paths differ by zero paths from the frozen base. No new ARMCC build or exact-match credit is claimed.

`build/audio_stream_natural_source_final/result.json` SHA256 `6ebd4d7f728644b7926003df6c596c275df5712167282cb3131afc30e65de9b5`, passed true and process exit0. Owned cleanup SHA256 `5800880c111e6387cdfa0dc74707e00c27d30691ecd149a0d312bfbd43411bb0`.

`build/audio_stream_stop_source_final/result.json` SHA256 `b5eaa7c6d9617788671cde8c26b697176f5540b505634f62908e6718b8c43d94`, passed true and process exit0. Owned cleanup SHA256 `532f2ce9feb3fe6ad5c406ea3eeef058653c18c9f3c68afa03a6882947c1bc12`.

`build/audio_stream_controls_initial/result.json` SHA256 `d391b889addaaea8d0e213bfe7f7208c22cd6a8f5cde29e35b3de086fc08ca58`, passed true and process exit0. Owned cleanup SHA256 `ddee750d55ce32643e6d977f0132d65d6686a5a372d5ae9bb6fe7185cfb3b70c`.

`build/audio_stream_default_preservation/result.json` SHA256 `0b7f49f1d0305bbfb6b4866dbd0bda3cfd95440716b4a2e4e9cc35fc1ff98c08`, passed true and process exit0. Owned cleanup SHA256 `cb156f4ceec4aafa3fc8e99cec93a6d91727c496eec8a28e79304564ad6c2c9d`.

Natural final source: HeadlessChrome154 consumes all 216000 original stereo frames/864000 PCM bytes through 106 packets. 105 packets finish delivery while the worker is running; 104 retained consumption reports precede SDK exit, with file extents below the final extent. Actual output Float32LE SHA256 `8bc1ba0c7bc591c9fcdacf79373054ba68ffb66e7b3a11b7ccd3d8c9f04e753c` equals independently derived bytes for every original PCM pair. All source drains and the context closes. High-water mark 2048 frames. Original 1350 HLE blocks/events/ticks, 1802 HID polls, 4583 GPU events/ticks, 545 PICA lists/8642912 bytes, 691200 RGBA and 345600 framebuffer bytes agree exactly with stock Azahar. Both canvas hashes agree with exported original RGBA, and CPU fallbacks are zero. All 24 initial directories/eight guest files preserve; raw differing pre-CPU host logs remain separately retained. Completed AudioBuffer conversion, equal-rate offline output and explicit recorded play/stop/natural end pass.

The natural context reports 1215872 startup-silence and 1282688 underrun frames over 82.948912 context seconds. Original source duration is about 6.6 seconds. This host does not produce synchronized continuous sound. These operational counts include loading and concurrent work and are not a performance benchmark.

Stop final source: the actual Stop sound gesture occurs after 4096 consumed/accepted source frames while guest capture remains running. The context closes, no completion receipt/witness is manufactured, and the guest still finishes with strict original audio/input/GPU/PICA/pixel equality. Completed recorded playback remains usable.

Independent real graph: 131077 channel-distinct source frames pass independent Float32LE conversion/order checks across ring wrap. High-water mark 65536; longest capacity-wait append 160.805000 ms. Eight malformed packets and wrong final count refuse without source acceptance; caller buffers retain ownership. Concurrent append refuses. Pending append and pending drain cancel; repeated disposal is terminal and owned contexts close. Its consumer/worklet/verifier seals still equal final committed files. No game PCM is used in these controls.

Default preservation: with the stream field absent/false, presentation60 still agrees on 1430 raw events/ticks, 765728 PICA bytes, 691200 RGBA and 345600 framebuffer bytes. Displayed pixels, complete exported inventory and initial guest tree preserve. The default profile creates no stream or AudioContext. All four credited runs are headless and have successful registered-resource absence receipts. No lifecycle source changed; no global/personal browser absence is claimed.

## Inputs and reproduction

Use the primary ignored build/root_browser_touch_input/build/browser_touch_module_final. Build-manifest SHA256 `4f9ad2c2c49161701a39ca926d720dee6f16ad08b2ceef17da22ce968497e5cd`; browser wasm SHA256 `63e495ab072a9a48eb2052344e263bc0df458ac7f63563b7131ffe11c1526b6d`. Linked C++/wasm/provider source is unchanged. The server seals 25 files covering served assets/sidecars and the successful build manifest; receipts verify them after capture. Dump-copy SHA256 `c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976`; schedule SHA256 `fbe0bdfdf8479deacac0c113c8d975b0094abe9e2eb8cfa7ce5cbcf9d6a9c10c`. Never commit the private inputs or generated artifacts.

From this branch worktree, source the primary environment. Outputs must be absent and ignored. Run the server in a separate terminal:

```sh
. /Users/exgota/super-mario-3d-land-browser/development_environment.sh
python tools/static_recompiler/serve_browser_execution.py \
  /Users/exgota/super-mario-3d-land-browser/build/root_browser_touch_input/build/browser_touch_module_final \
  build/stream_audio_server \
  --block-schedule /Users/exgota/super-mario-3d-land-browser/build/root_native_block_scheduling/build/native_block_schedule_full/block_schedule.bin \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_input/reference_scripted_360 \
  --dump-sha256 c83f9175208b3d60898d3a6d60451bee3108521e409cace362f63b46dda8c976 \
  --dump-bytes 536870912 --stream-audio --presentation-limit 360 --wall-time-seconds 180 --port 8807
python tools/static_recompiler/verify_browser_execution.py \
  http://127.0.0.1:8807 build/stream_audio_natural \
  --dump /Users/exgota/super-mario-3d-land-browser/build/root_port_reference/owned_dump.3ds \
  --server-output build/stream_audio_server \
  --reference /Users/exgota/super-mario-3d-land-browser/build/root_port_audio/reference_scripted_360 \
  --movie /Users/exgota/super-mario-3d-land-browser/build/root_port_input/reference_scripted_360/input_movie.ctm \
  --observe-audio --stream-audio --timeout-seconds 360
```

Repeat the verifier into a fresh output with --stop-stream-after-frames 4096 for consumed-source cancellation. Run `python tools/static_recompiler/verify_browser_audio_stream_controls.py http://127.0.0.1:8807 build/stream_audio_controls` for the independent graph. Default preservation follows runtime/port/browser/README.md using presentation60 and its paired rendered reference. Missing ignored modules/providers must be rebuilt through existing browser/native documentation.

## Retained failures and limits

build/audio_stream_guest_initial preserved complete raw/worklet checks but failed because recorded playback stayed disabled after stream end. The page now restores that button; final source runs include actual recorded replay. build/audio_stream_guest_final passed before the final verifier extension and receives no final-source verifier credit. The first Stop test acted before source consumption because an asynchronous browser wait returned too early. build/audio_stream_stop_final remains retained but supplies no consumed-source Stop credit. The final check uses a synchronous actual-player counter plus an independent required 4096-frame assertion. No oracle was relaxed.

build/audio_stream_natural_final passed raw/source checks but its cleanup gate refused a process that exited between OS identity reads. It remains failed. Next admission recovered that exact abandoned registration with fresh absence proof and zero signals using the unchanged policy. The new natural final-source run has a separate successful ordinary cleanup receipt. No lifecycle patch or reviewer was introduced.

Unverified: physical speakers/device resampling/content recognition; Safari/mobile or unsupported context rates; realtime production and audio/video synchronization; user-induced suspension and transport-timeout fault injection; continuous sessions/saves; World1-1 entry/goal and Section7 gameplay semantics; complete rank-O source adapter coverage. Milestone5 remains in progress. Root proceeds to native World1-1 entry recording and routes layouts to Pro. All captures, screenshots, PCM/movie/user trees, compiled modules and downloads stay ignored. Matching throughput for this port family is zero bytes/hour, including integration time.
