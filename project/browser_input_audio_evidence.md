# Browser recorded input and audio evidence

Root/browser-input-audio freezes integrator main 134f9708dc8b2f231ed7d6dce2d10d9f24b9d76e, including accepted browser execution cd0530e73 at 814da912. Root owns the browser worker/page, additive observation configuration, playback helper and verification/documentation. No Game/lib/config/map/rank/ledger/Factory or protected oracle changes. No claims, zero matching credit, recompiled_bytes remains 2437712. Only the integrator moves main.

## Preserved execution and observation boundary

The accepted browser wasm remains 152386477 bytes, SHA256 2d6121b281486b460b68220892dd89046af96bd31ffa1dd47073c1474ff082d2. Its original complete-provider Node reproduction and all 71 protected builder inputs are unchanged. The browser entry and C file identity source agree with the accepted builder seals. No C++ entry, CPU translation, scheduler, Azahar observer, provider/archive, shader, DSP implementation or matching build input is rebuilt or changed in this family.

Version-one options add optional boolean input_capture/audio_capture, defaulting to false only when absent. Explicit malformed values are refused. Audio requires input observation and both require a software presentation. preRun sets the existing capture environment before guest initialization. The original movie and initial user tree remain readonly and fully identified. No live input or recording script replaces the movie. The null audio sink remains authoritative for capture; WebAudio starts only after capture shutdown.

The worker validates complete raw input and audio outcomes before export. It checks schema, sequence, safe integer extents, presentation timing, contiguous 160-frame stereo PCM blocks, final counts, null sink and exact payload size. Files travel through the existing acknowledged 64 KiB ordinary-buffer protocol. The page collects only its own actual exported PCM, bounds it to 64 MiB, and exposes sound after orderly completion. A failed or aborted capture earns no playback or replay claim.

## Actual Chrome observations

Paths below are under this checkout's ignored build/. The server uses primary build/root_port_input/reference_scripted_360 for the original movie/snapshot; the independent oracle is primary build/root_port_audio/reference_scripted_360. Unchanged compare_audio_capture.py and its inherited input/render comparators require exact raw bytes and ticks. The native audio observer's original stock captures remain unmodified.

browser_input_audio_submission/result.json passes on final submitted execution/verification source. Its result SHA256 is a1b694310a5f60faee2b2eb3fb44d8e66bb9b65b4a2de6d784a41107a1885a76; comparison.json is e31dfb9a89157c0ab4f3bfa900e91a97636668cc3b0663c4a6e39ff967cbad0f. Actual capture: browser_input_audio_submission_server/capture_cb84d122132f49409039050e43b1769f.

| Observation | Result |
| --- | --- |
| Delivered HID | All 1802 polls, movie delivery and ticks exact |
| HLE audio | All 1350 blocks, 216000 stereo sample frames, 864000 PCM bytes and audio events/ticks exact |
| GPU/PICA | All 4583 raw GPU events/ticks and 8642912 PICA bytes exact |
| Screens/framebuffers | Both 691200 RGBA bytes and 345600 original framebuffer bytes exact; both displayed canvas hashes agree |
| Static CPU | 393977876 guest instructions on CPU0, zero CPU1 instructions, zero ARM interpreter/JIT fallback |
| Completion and initial tree | Actual SDK exit0, all exports acknowledged, worker shutdown, all 24 initial directories and eight guest files exact |
| Recovery | Wrong-sized file rejected, stopped download leaves enabled controls/hidden screens, fresh valid replay succeeds |
| Browser signal | Both actual AudioBuffer channels and equal-rate OfflineAudioContext output match every original PCM-derived Float32 value |
| Explicit playback | No initial AudioContext/source, explicit start/stop/start/natural ended; context time advances, source ends, action returns to ready |
| Independent stereo control | Four signed stereo pairs with distinct channels and byte-order-sensitive values pass through the actual offline graph exactly; this synthetic control earns no guest execution credit |

PCM SHA256 is 474c6170eb8aa8f7a604f1a4caaff03170714546bad3e95563094624646bf923. Both actual menu channels are identical, with Float32 little-endian SHA256 7cc8d7166201c61d3482e148863f67cfbbf68ebdaa373b9c358a9420e753a0b1. The separate distinct-channel control protects against swaps that dual mono cannot reveal. Top-screen RGBA SHA256 is f42ce86bc09e18c1879515ef39d76396a6a341182166d9a6cbb7f577ea934856; bottom-screen SHA256 is f5978923d5a594990ae994603207995c986475386f45b205963602ba14385ada.

Signed-16 little-endian pairs map directly to Float32 by division by 32768 at 32728 Hz. No normalization, synthesized silence, gap filling, gain or channel mixing occurs. The actual live AudioContext runs at 44100 Hz and naturally resamples the source clip; raw PCM/offline parity does not claim device-output equality. Nominal duration is 6.5998533366 seconds. Recognizable sound and actual speaker fidelity were not assessed.

The full raw host log remains separately retained. As in accepted browser startup, logging initializes/truncates its diagnostic log before recording the initial snapshot. Its wall times and host paths are not labeled console state and are not normalized. Every guest file and empty directory remains included.

## Capture history, review and reproduction

The first browser_input_audio_first run completes exact raw audio/input/render capture and actual WebAudio checks, then the verifier rejects a 375-pixel mobile screenshot requested at width390. That partial failure and its logs remain intact. browser_input_audio_final then passes the full suite with valid 1440 × 1080 and 390 × 1200 captures. After stricter boolean refusal and playback lifetime guards, browser_input_audio_submission passes on the final source with the independent stereo graph control. Both final screenshot dimensions, document geometry and loaded fonts are checked mechanically. No screenshot or audio payload enters Git.

runtime/port/browser/README.md provides the exact server, original movie/oracle and real-browser verification commands. The accepted browser module may be regenerated through its existing sealed builder. Reports/servers require absent directories under ignored build/. The final verifier source SHA256 is 9ebf3e781260cbd199f5516f06ebec14d2ac9698f0a6fef93b6cad74a66aefdf. Original compare_audio_capture.py remains SHA256 30ffa2d310564a744a769ab786afc530f33766a61502dc798cd6852c5e7ee1af.

Fresh final visual review returns ship at the recorded operating-surface scope: browser_input_audio_submission/finish_review.md, SHA256 7f915df31dcddcb66bd837273d1a2964e3cd3a40bbba05f96577369c24a810fa. Motion and keyboard execution remain unscored. The extension keeps the incumbent palette, typography, screen geometry and file/run flow. DESIGN.md and its sidecar remain unchanged; their component inventories omit this optional sound extension, while the surface/product/evidence documents describe it. Interface fonts retain their official IBM/OFL provenance; no new raster ships.

The shipped documenter's ordinary-extension comparison is browser_input_audio_submission/documenter_review.md. It confirms palette/type/radius/focus/geometry preservation and verifies actual capture/comparator seals. It preserves DESIGN/sidecar and reports their older inventory/evidence references, without treating an optional component as a new visual world. The surface brief now registers BrowserCapturedAudio.mjs among its related targets. The completed-clip extension is documented in Product, this note and the runtime README.

Fresh default-profile preservation passes on the same final source. browser_baseline_gpu/result.json is SHA256 a51e4711942aa755f353b6f066cbef1eb568e0476aabecdc3c7de15f7b031c08; its unchanged strict comparison matches all 528 first-swap events/ticks and 79936 PICA bytes. browser_baseline_frame/result.json is 2d3a92396d709866e5f8c924be31f096cb81474dab9cb2939fdd6ddb315a1e69; title presentation60 still matches all 1430 events/ticks, 765728 PICA bytes and both original/displayed screens/framebuffers. Observation flags remain off, with no captured audio or autoplay. All three final capture input inventories were rehashed after execution with zero changes.

Final source seals: BrowserCapturePage.mjs c45952a725a174c9c0478422649f8a72d551aeceed0f840238428bf2ed95a089; BrowserCaptureWorker.mjs a1865a3c9d38ab96371b1a1e819382433566621c598d6d152d1aaa554427ebfd; BrowserCapturedAudio.mjs f948762ca388515b3844c5f33fdbdabc1e85ad25789a34dd2f4a8961026e4f1b; serve_browser_execution.py 7a064127d05642e18074976763ea3287dd99d6c2152465d36ff0c71e7f4fab98. The final verifier seal above covers all three completed suites.

This supports bounded browser movie input, exact generated PCM and explicit completed-clip WebAudio playback. It does not establish live browser HID, continuously synchronized rendering/audio, speaker or real-hardware DSP accuracy, mobile hardware/performance, saves, complete rank-O adapters, World 1-1 entry/goal or the full Section 7 semantic suite. Port milestone 6 remains in progress and milestone 5 remains open.
