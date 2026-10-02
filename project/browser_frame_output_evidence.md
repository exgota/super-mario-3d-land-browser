# Browser frame output evidence

Root/browser-frame-output freezes integrator main 03cdd6b489a2ca436f968142163a4e90e2d6f5f1. Initial implementation commit 98a2a2a6 owns browser frame slots, input/entry integration, browser builder/worker/page, bounded preview verification and documentation. All port code outside runtime/port/browser, accepted platform/provider archives and existing differential comparators stay unchanged. No matching build inputs, rank cells, ledger or Factory files change. No claims and zero new exact bytes.

## Observed behavior

The application CPU samples public RendererSoftware::Screen on ordinary HID polling after completed software swaps. Two fixed slots own paired top-left/bottom RGBA arrays. Each screen is bounded to 1 MiB; release/acquire states protect metadata and pixels. Producer and consumer ring cursors preserve publication order. Busy slots drop a new distinct frame without waiting or overwriting a lease. The worker synchronously acquires, checks extents, copies ordinary buffers and releases before asynchronous hashing or page messaging. One preview acknowledgment is outstanding. The page paints those actual buffers into the incumbent canvases.

Frame/tick identity retains uint64 precision. Software output is already landscape row-major, with display width equal to ScreenInfo.height and display height equal to ScreenInfo.width. No flip, crop, interpolation, decoder-edge repair, synthetic reference frame, raw guest layout, renderer mutation or guest-timing adjustment occurs. The selected final capture is separate because it can stop before another HID callback.

Browser module build/browser_frame_module/build_manifest.json passes. Both new native bridge objects compile with -Wall -Wextra -Werror. The entire accepted Node wasm reproduces byte for byte, SHA256 6eddf4240cb41e04b75a955d4eda64021676b2c8737f2389e58aa375218cddd8. The new browser wasm is 152394753 bytes, SHA256 fc4ed97ba81cde1f6b65e7057370347baea818a9592f0fa2ca0a9f02396dc449. All 75 protected builder inputs remain unchanged. HEAPU8 is explicitly exported for reading owned slots; its shared buffer is never sent to the page.

Corrected Chrome result build/browser_frame_stream_final/result.json passes. It displays 564 acknowledged paired samples. Producer totals are 580 unique frames, 565 publications, 15 full-slot drops, zero unavailable pairs, 1689 duplicate polls, zero frame gaps and zero geometry errors. One publication remains unconsumed at normal SDK exit. The partition 580 = 565 + 15 + 0 holds. Every copied frame and tick matches an original HID observation; no metadata is inferred from an outer-thread renderer read. All eight initial guest files and 24 directories match. Host diagnostic logs remain original, separate and unnormalized.

The independently retained running canvases at renderer frames 162 and 500 match the copied-slot dimensions/extents/SHA256 values. The paired images change between those checkpoints. The final selected capture is presentation 480, renderer frame 581. Derivation from that same validated capture gives stock targets 61 and 399, not a hardcoded bootstrap offset.

Fresh stock runs use the original full movie and complete exported initial tree, unchanged stock executable/dump, clean ROOT_PORT environment and original wall/payload/input/audio bounds. Full native run exits normally with code 0 in 45.55 seconds. Checkpoints exit normally with code 0 in 5.12 and 38.87 seconds. Before/after original file hashes and empty-directory inventories match. The unchanged rendered loader validates complete stock outcomes. New compare_browser_frame_checkpoint.py requires direct paired RGBA byte equality at exactly renderer 162/presentation 61 and renderer 500/presentation 399. Both pass. Prefix images do not earn full movie/input/audio/GPU parity; the unchanged full movie pad-count gate is not weakened or given a truncated movie.

Fresh full final stock comparison passes every input/audio/GPU event and tick, PCM, PICA payload, raw framebuffer and RGBA byte. Both runs deliver 2272 HID polls. The browser observes 1760 audio blocks, 281600 stereo sample frames and 1126400 PCM bytes. The static CPU reports zero interpreter/JIT fallback. The independent retained-image assertion is separate from complete final replay.

## Controls and retained failures

First-lease controls on actual native slots refuse invalid indices, screen 1, stale pixel generation and stale release without freeing the valid lease. Full-slot pressure occurs in the actual successful run and is counted. An actual stopped preview already displaying paired output returns failure with input/run controls enabled; a fresh session completes. Default title60 still passes the unchanged strict rendered comparator, original final canvas hashes, full initial tree, wrong-size rejection and stop/retry recovery.

An actual late acknowledgment is withheld at renderer frame 570. Native execution reaches normal shutdown; worker phase enters validating and the acknowledgment times out. It remains failed, with zero manifest/completion or late preview events and enabled recovery controls. The original failure carries immutable producer totals. This is a deliberate failed capture, with no success credit. Its result is build/browser_frame_acknowledgment_failure/result.json.

Eight copied invalid fixtures are refused: top RGB byte, bottom alpha byte, truncated RGBA, extra screen payload, wrong renderer frame, incomplete outcome, retained canvas extent and retained frame identity. The positive actual checkpoint passes before controls. Original browser/stock/oracle inputs remain unchanged. These controls are under build/browser_frame_checkpoint_controls/.

First run build/browser_frame_stream/ displays 560 samples and reaches a normal complete export, then its independent audit fails on a syntax error in the retained-canvas export expression. That failure remains preserved and earns no complete verification credit. The corrected verifier and worker run under fresh paths.

Read-only bridge review also finds two asynchronous failure paths in the earlier worker: a caught preview rejection could resume final export, and an asynchronous digest could publish after failure. The final worker checks phase before post-digest publication and after awaiting preview completion. The real acknowledgment control proves terminal failure during exit validation. A forced failure while digest promises remain pending, actual heap growth, unsupported renderer/geometry, multiple consumers and physical mobile devices are not exercised.

## UI and scope

One batched root inspection opens four valid running/completed desktop/mobile PNGs. Sizes are 1440 by 1080 and 390 by 1200, with loaded IBM Plex and no horizontal overflow. One changed-target detector returns []. Fresh finish review disposition ship covers the sampled-frame extension at those sizes. Ordinary documentation comparison preserves DESIGN.md and .impeccable/design.json byte for byte. Earlier evidence-scope/component-inventory drift is reported and left unchanged. Public game rasters, private movies, snapshots, generated translations/modules and proof images remain excluded from git.

The optional live A plus frame-output combination also passes. It displays 559 acknowledged samples, with 580 unique observations, 560 publications, 20 full drops and one unconsumed publication. A real pointer hold records 15 held HID polls and a release in the new 2272-poll CTM. Fresh stock replay exits normally with code 0 in 37.00 seconds and the unchanged full movie comparator matches every input/audio/GPU/tick/PICA/framebuffer/RGBA byte. That profile is independently scoped from the readonly movie checkpoints. Sustained frame rate, synchronized continuous sound, speaker content, mobile hardware performance, persistent saves, complete rank-O adapters and World 1-1 semantic state/entry/goal remain open. Milestone 6 remains in progress; recompiled_bytes remains 2437712. This port family adds zero matching bytes and zero exact-byte throughput.

## Rerun and file seals

All paths below are beneath /Users/exgota/super-mario-3d-land-browser/build/root_browser_frame_output/. Source the primary development_environment.sh before Python. The public runtime/port/browser/README.md gives the complete module build and local server contract. This run serves the original complete movie from build/root_browser_live_button/build/browser_live_pointer_final_server/capture_d35e624c9aa940018434c861e3fa1a43 in the primary checkout, with --observe-audio --frame-output --presentation-limit 480. Current local preview is http://127.0.0.1:8793. Use fresh output directories for every rerun.

verify_browser_frame_output.py runs the actual owner File, intermediate stop/retry, two bounded independently displayed samples and complete movie comparison. Full commands and original command outputs live in build/browser_frame_stream_final/commands.json. The ignored build/run_stock_browser_frame.py takes that result, sample index -1/0/1 and an absent output path; -1 is the full selected boundary. Its receipts bind exact argv/environment, original movie/dump/stock/initial tree, source/module inputs, derivation and actual normal process exit. A playback capture does not write another CTM; this runner identifies the original movie from the bound full comparison and readonly input manifest. Never substitute an empty or newly recorded movie.

| File | SHA256 |
| --- | --- |
| build/browser_frame_module/build_manifest.json | 10a0686de1aa7f2183c70d13862899755089e9792487c649b75075ca898ab5a8 |
| build/browser_frame_stream_final/result.json | 464e9fb463b7f3f930639b74dc8974556a0b5f9e7670d964bb46b4043dddc543 |
| build/browser_frame_stream_final/full_movie_comparison.json | e489e95cb54bbe6c511a43e3809c8470c2cd3a71e34fa867e058d71f246049bb |
| build/browser_frame_stream_final/metadata_and_snapshot_check.json | a18489ee2af9af1c5a2d3fd43932cbc9e339224bd9e151dc579058202c64df4e |
| build/stock_frame_stream_final.receipt.json | e98938a48b0acc760f57d32e6d38c8ec936ea08e0e79945b2ac56e1372892e08 |
| build/stock_frame_stream_final_comparison.json | 64f184e829b586499da6e8111fde3d522459913c0d6fb2962af97294d166d133 |
| build/stock_frame_checkpoint_0.receipt.json | cdd75593b09981a508eccc0d38570c890d741e2130976548c05f018326c9e7bc |
| build/stock_frame_checkpoint_0_comparison.json | 4a402a767a6edc0ee1a113fe8cc2699e016bfef7c322ee357b24c9f424a623d9 |
| build/stock_frame_checkpoint_1.receipt.json | 1b180b35719b7c961f70dd3348f467f5bfc5fe8ce4e1919f4c9b6f7db1a4f12c |
| build/stock_frame_checkpoint_1_comparison.json | 78fcfc3cf56b6bc3b4c7c557fe3e8a1c4175f3feb04a2b8549a320988c8f1f22 |
| build/browser_frame_acknowledgment_failure/result.json | d21b827c765b77908d537d004964c1673fbb75fe3699ba8f9bf4e94aadf090a9 |
| build/browser_frame_checkpoint_controls/result.json | 26bc8e88d5ed1ee4ebcd917b14f91f02d92bbbb0fc19370ce676f56f5c68c443 |
| build/browser_frame_baseline/result.json | aea200983a449bb1876ceeca7b94289d919714c97ba16957ab6b9f4a0b7d726c |
| build/browser_frame_stream_final/finish_review.md | e252ed45b4407a9eb77603c94250e9c2250aff4788522f733deda95fb74e6d0b |
| build/browser_frame_stream_final/documenter_review.md | 4f3c790b07db0f2f04f15d9c3430a36385db1676eccc7e76519e529bf2602829 |
| build/run_stock_browser_frame.py | 60da43b848b7c20bbeb2c797660e4ec48c56b4f8e1d1273fec7caba90da9344d |
| runtime/port/browser/BrowserFrameOutput.cpp | b176be7201f6650377164229ef37cdae18f0d49047e158329646ed60d0e01d2a |
| runtime/port/browser/BrowserFrameOutput.h | 0928e86b08022078659b0f676a583b26b9c0ff704f47185feba5df81229120e6 |
| runtime/port/browser/BrowserButtonInput.cpp | 2cf862c098c8bd1bf1ac8744ef2d17f5601a19be411a64ac8e11b77e73f891f6 |
| runtime/port/browser/BrowserExecutionEntry.cpp | 6a3f9259b8056491c22f90a5440f61136131797a2a635691a6162a54463dd492 |
| runtime/port/browser/BrowserCaptureWorker.mjs | ab721248ef90a570cc27d56a8a04a7eef9e2f39c6bf28b64f50191cc1c1b3db2 |
| runtime/port/browser/BrowserCapturePage.mjs | 05706a24466cd3ed3e5ea3634dc328e367d4e92362da37e9d8abfad9e9f2d89b |
| tools/static_recompiler/build_browser_execution.py | e129409b1505a18f4522396b9a9efbfae233a774426261cbc9c365212526f39a |
| tools/static_recompiler/verify_browser_frame_output.py | 4bbbbbbcfbd6900a7a2b8834e7aeb4dbccc4bcfe9eb281c6417d2f73b633a2cf |
| tools/static_recompiler/compare_browser_frame_checkpoint.py | 47363951571b75857ec0c4aa0fb493178879ec1f5672e7a53b1bfaa8cf7756d8 |
| build/browser_frame_live_pointer/result.json | c56aed35a5654a1743f8fa4677a1c5e41f4c41fb0c2bbf6f0448e4a97c37e214 |
| build/stock_frame_live_pointer.receipt.json | bec2f4c68e1db5d204d04888c125e796e82b7f0dca8562e0c8535c696e2ee3c8 |
| build/stock_frame_live_pointer_comparison.json | d814274fdd5db19be232e163305c230c91e046388d6a1b2a484f40835ceda692 |
| runtime/port/browser/BrowserButtonInput.h | 16a90522625616480ba3ddc6396d9eaa61459e4053131d8a459c2ed9b66e7c55 |
