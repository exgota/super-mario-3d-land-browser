# Browser circle-pad recording evidence

## Change and scope

Root/browser-circle-pad starts from integrator main `4f593070365eb3de481b99553d1f3f44270a4a76`. Implementation `cc3661ea` adds an explicitly selected browser `Input::AnalogDevice`, coherent paired position publication, finite circle-only recording, held direction controls and a saved-HID verifier. The matching ARMCC sources, Game/lib/config/map/ledger/Factory, accepted CPU/platform providers, address replacement interface and existing differential comparators are unchanged. There are no matching claims, rank edits or newly recompiled bytes.

The public provider is pinned Azahar `662d412123305a9f4be94dd3dc73ddf91a18c55e` plus previously accepted instrumentation/platform patches. `src/core/frontend/input.h` defines the unit-circle two-float tuple, positive x right and positive y up. `src/core/hle/service/hid/hid.cpp` retains scale 154, float rounding, three-sample integer averaging and derived direction masks; movie recording saves those delivered averages. No guest class layout was inferred or altered.

The native adapter packs biased coordinates and active state in one lock-free atomic word. Bounds/radius checks precede mutation; compare-exchange and closure share the same word. It selects and restores only the CirclePad input profile. The outer worker reads owned diagnostics, never Core/renderer state, and makes no native calls after normal SDK exit. An inactive A sampler supplies the accepted frame callback in circle-only mode. Explicit A input remains inactive unless separately requested. Poll/sample diagnostics are separate atomic observations; actual HID and CTM records certify delivery.

The UI retains the committed design, fonts and screen geometry. Four held directions support pointers, focused arrows, Space/Enter, diagonals, opposing-direction cancellation and release on pointer/key/focus/visibility/session loss. Diagonals use legal `(±108, ±108)` pairs. Preview frames remain bounded samples. Sound playback uses the completed captured clip.

## Build and protected inputs

All private paths below are relative to `/Users/exgota/super-mario-3d-land-browser/build/root_browser_circle_pad`, unless a primary-checkout path is named. No generated module, game data, frames, movie, PCM or downloaded provider is committed.

`build/browser_circle_pad_module_final/build_manifest.json` passed, SHA256 `7faeb826ac11063ee22a02b5ec1af8fb33bc1b894f6fb42db042b6eb34795c2e`. It seals 77 unchanged inputs. The new circle object compiled with `-Wall -Wextra -Werror`. The entire original Node provider closure reproduced wasm `6eddf4240cb41e04b75a955d4eda64021676b2c8737f2389e58aa375218cddd8` before browser linking. Browser wasm is 152401317 bytes, SHA256 `8ae1fac4a8fe02af8f999e66b1e2dbc72f775e52e52fd32089bd1321d6dff0ff`.

The final native source seal is `1b418b58b2d7f4027295cd1e56bb5ccf74c58bf71b043806b2bc5d930ea412c9`. The earlier `build/browser_circle_pad_module` links only a historical source revision; it earns no current execution credit. Both builds happened to produce identical wasm. Source seals distinguish them.

Builder regeneration, local serving and the public verifier are documented in `runtime/port/browser/README.md`. Source syntax, Python compilation and `git diff --check` pass. This no-build-input submission does not claim a new ARMCC full-map check.

## Final-source browser recording and stock replay

`build/browser_circle_pad_capture_verified/result.json` passed, SHA256 `fc04c2f594fee1a88650464075c4db698070d2a07242e5704c6738e524e56e49`. It binds verifier source `cb5bdb47fd5287aff1ac0d4548968383f373d5f8b6a14f0f35bd210131cd6b0d` before/after the actual run. Its own Chrome profile closed normally after validation. The capture is `build/browser_circle_pad_server/capture_f756e3b0dfb24df1b4c76fca9c23ed2d`.

Observed: 2272 HID polls; 2272 pad/circle and 2272 touch movie records; 894 accelerometer records; both completed canvas dimensions/bytes equal the saved original RGBA; complete output file/directory identity; eight initial guest files and 24 initial directories exact, with only the raw host log treated separately. Every interpreter/JIT fallback counter is zero. A and touch remain neutral. Circle cleanup reports exactly 2272 device polls and profile restoration before normal exit.

All four signed cardinals and their releases match contiguous HID ramps. Positive input from neutral is `0,51,102,154`; release is `154,102,51,0`. Negative values preserve truncation toward zero. Upper-right and lower-left plateaus are `(108,108)` and `(-108,-108)`. Partial releases match `(108,108),(123,72),(138,36),(154,0)` and the corresponding negative values. Focus loss ends neutral. Requests remain ordered, with actual status-zero acceptance; those acknowledgments are not the delivery proof.

`build/browser_circle_pad_verified_direction_bits.json` passed, SHA256 `9837cba56f8a2bd34591e6fc7a12d8432136acb7a6eb377d0e6f21253aeb1655`. The private metadata inspector `build/verify_circle_pad_direction_bits.py`, SHA256 `eed88395f6516444ba8d4686a8b2cd2367ec05d8e8d9b3423d22ef1e902ad1fd`, checks actual recorded bits 28 through 31 at seven stable positions. Observed masks are right1, left2, up4, down8, upper-right5, lower-left10 and neutral0. It reads saved data only.

There are 562 copied/acknowledged paired preview frames. Native producer totals: 580 unique frames, 563 publications, 17 full drops, zero unavailable/gaps/geometry errors, 1689 duplicate polls and one unconsumed publication. Invalid/stale lease controls preserve the actual held lease. These are sample counts, with no sustained-rate claim.

Fresh stock Azahar replay of that exact CTM and complete initial tree exited normally 0 after 43.18 seconds; `build/stock_browser_circle_pad_verified.receipt.json` retains arguments, original clock, before/after hashes and timeout status. The unchanged general movie comparator passed in `build/stock_browser_circle_pad_verified_comparison.json`, SHA256 `2a2ad359da1ad04137e20263000b2403230c5f58c12bf5950a7d5890ea5c71cb`. Every HID value/frame/tick, audio event and original PCM, GPU event/tick/PICA list, framebuffer metadata/raw bytes and final two-screen RGBA agrees exactly. Audio includes 1760 blocks, 281600 stereo frames and 1126400 PCM bytes. The boundary is presentation480/renderer581. This certifies platform replay, not game-state semantics.

The private stock runner is the preserved primary `build/root_browser_live_button/build/run_stock_browser_movie.py`, SHA256 `ea884bde5d785b670cde49bf99bdbcf844bdb7e74ccd4fc92bf0e3d82987b3df`. It runs the unchanged pinned stock binary, clears unrelated port flags, protects dump/movie/tree/binary inputs, records normal process exit and uses the original finite boundary. Rerun it with an existing completed recording capture and an absent ignored stock output, then run public `compare_movie_replay.py reference browser --movie capture/input_movie.ctm --report build/new_report.json`. The owning dump and large providers stay local.

## Independent controls and earlier receipts

Real Chrome workers pass 15 prelaunch schema/session/numeric controls in `build/browser_circle_pad_worker_controls/result.json`, SHA256 `05efafa335b7ea35bb30e6f048a912f7942f23d850e1c71949a825502b00f3e7`. Cases include booleans, fractional/nonfinite/string/huge values, signed component overflow, outside-disk vectors, wrong session, missing/duplicate sequence, extra fields, an unsafe clock and a nonboolean profile flag. Empty synthetic Files and an unavailable module URL prevent native/guest launch. A valid neutral request during initialization returns inactive2 before a duplicate sequence refuses. These fixtures do not certify malformed requests during an active guest.

The actual successful guest also records compiled inactive neutral2, out-of-range1, outside-disk1 and active0 before installation; active invalid requests return1 without changing the position. In circle-only mode the A active query is0 and its held setter returns2. An early source-review diagnostic that reset a valid position was removed before all actual recordings. Oversized initial recording clocks refuse before launch. Source review proposes no new oracle behavior.

Earlier actual recordings remain preserved with their own source hashes:

| Receipt | SHA256 | Scope |
| --- | --- | --- |
| `build/browser_circle_pad_capture/result.json` | `66b05ecfd0c8331e68b8b25e776ab3147696911c5383388d4e179d8e9a61b47b` | Actual pointer/keyboard/HID delivery, source8f20da01; mobile held-label PNG malformed |
| `build/stock_browser_circle_pad_comparison.json` | `48c9dc500ab810b91fcc3bbbd98f8c0e2b823db0a2198f29b9f445ffd8ed6fc3` | Full exact stock replay, normal exit0 after42.46 seconds |
| `build/browser_circle_pad_capture_final/result.json` | `aecd7fa3e637b542a20106a0791d9c37f2b8677f883f2b568b6914204ef1caf4` | Settled visual recapture with source2782a261, actual2272 HID polls |
| `build/stock_browser_circle_pad_final_comparison.json` | `853ba3567fad6e90007be46dc6a0eaa14d7e094d9ada88b9245ee4fe17c57cf8` | Full exact stock replay, normal exit0 after46.48 seconds |

The corrected verifier adds explicit paired-screen cardinality/dimensions, timeout partial-output receipts and source identity before/after execution. It was frozen before the separate final-source recording. Historical executions do not acquire the corrected verifier's source seal.

## Existing profiles

Default presentation60 passes in `build/browser_circle_pad_baseline/result.json`, SHA256 `01efdceba1add8fad202fc850c6d56df128e4230b5ee89bb212f7f11610de4f2`. The unchanged rendered-capture oracle confirms both original canvas streams. Wrong-size rejection, Stop during loading and fresh retry pass; complete guest files/directories remain exact. This profile has no live circle flag, no circle controls and no new recording.

Standalone A pointer recording passes in `build/browser_circle_pad_button/result.json`, SHA256 `48a54eb0fea6f9443e8a92a26f2538263fe0a602b6fa7ad95242792108869c93`: 2272 HID polls, 15 held-A polls and a delivered removal, 562 preview samples, full new CTM/export and exact final canvases. Fresh stock replay exits normally0 after33.29 seconds. `build/stock_browser_circle_pad_button_comparison.json`, SHA256 `cabe8d2bfda65f6bb547cbef7d0dced927d4aa8763c75da8d7963cec1ccd8f4c`, passes every original input/audio/GPU/frame channel. This profile leaves the circle bridge uninstalled and does not export circle request fields.

The first playback preservation invocation mistakenly supplied the stock replay folder as the frame verifier's reference; that folder does not contain the required original CTM. The browser itself completed normally and its raw channels agreed, but `build/browser_circle_pad_playback/full_movie_comparison.json` correctly reports `passed:false`, with movie validation error "movie must be a bounded local regular file with input records". Its logs/capture remain preserved and earn no successful verifier credit. The corrected invocation supplies the actual recording folder containing the original CTM and passes in `build/browser_circle_pad_playback_final/result.json`, SHA256 `a4431002bf25a0ea849e1d37c1a7fb7bdef4ee4f4af5bdef56adaeaa193be35b`. Its capture is `build/browser_circle_pad_playback_server/capture_af662312b56e4a369a48d6643ffc63e0`: 562 acknowledged paired samples, independent running canvas observations at renderer162 and500, original full movie parity and exact final canvas bytes. A direct unchanged stock comparison also passes in `build/stock_browser_circle_pad_playback_comparison.json`, SHA256 `a0ccb90c520cbb3915017232c87cb53bee6e58b108978fe027a06b188eeb3256`. Neither live adapter is installed during this playback profile.

## Visual evidence and reviews

Root opened the initial running/completed desktop1440×1080 and mobile390×1200 PNGs once. The mobile held-right label was absent after an immediate resize under pointer capture. No public source path altered that label. Root preserved the malformed original and recaptured in the separate `browser_circle_pad_capture_final` packet after releasing pointer capture, settling layout/fonts and applying keyboard input. Root opened the corrected mobile running image once. It shows the complete label and focus ring. The root cause of the earlier paint loss remains an inference.

The settled control metadata records `Hold right`, white `rgb(255,255,255)` on green `rgb(53,79,64)`, aria-pressed true, enabled110×48, loaded IBM Plex Sans, request2 accepted,1474 device polls and renderer376. Both final viewport receipts have no horizontal overflow and loaded font weights. The fresh finish reviewer issued `disposition: ship` in `build/browser_circle_pad_capture_final/finish_review.md`, SHA256 `b5064e205e9e5e79ffa8aa4445d0a254813b5667e876b8949510627793061a5a`. No public UI edits followed it.

One changed-target detector run is preserved in `build/browser_circle_pad_capture/design_detector.json`. Its caption contrast pairs use the wrong painted background, because captions sit outside the screen wells. Actual secondary/page contrast is5.232:1 and pressed label contrast8.960:1. Disabled colors are inherited inactive controls; the cream ground is the committed visual world. The review required no public style change. Ordinary design documents and sidecar remain unchanged. The fresh ordinary documenter passed in `build/browser_circle_pad_capture_final/documenter_review.md`, SHA256 `53bae3fb228fa99ee18b89852532cb57363b86ce22a675424e5a3ab72e39d51f`; pre-existing startup review references and omitted earlier A/audio component samples remain historical drift, with no unrelated document regeneration.

## Limits and attribution

Milestone6 remains in progress; World1-1/milestone5 remains open. Finite sampled output and live circle input do not establish an unbounded playable session, sustained frame rate, synchronized sound, saves, physical mobile performance, speaker content, player movement or Section7 player/camera/RNG/timer/coins replay. No complete rank-O adapter coverage is claimed.

Clock evidence uses actual approved base tick926190305. Admission at the exact numeric maximum leaves no room for later ticks; arbitrary-clock endpoints are not certified. The unique private factory name and fresh single-session module are assumptions. Foreign factory-name ownership and reuse across uninstall/reinstall with old in-flight calls are not certified. Normal successful capture cleanup is observed; arbitrary error unwinding/abrupt browser termination is not labeled normal shutdown.

The primary ignored public-API, native, worker and verification reviews are `build/root_browser_platform_investigation/browser_circle_pad_boundary.md`, `browser_circle_pad_bridge_review.md`, `browser_circle_pad_worker_review.md` and `browser_circle_pad_verification_review.md`. They are proposals/read-only source reviews, not execution substitutes. Their seals are retained separately from runtime receipts.

Recompiled_bytes remains2437712. Root's accepted matching-byte throughput is0 bytes/hour from2026-10-02T23:00:05Z (accepted frame-output verdict) through2026-10-02T23:47:42.663850+00:00 (circle submission preparation). The attributed frame verdict has no matching claims or matched bytes; this port-only family likewise claims none. Elapsed wall time includes browser, stock, review, preservation and branch preparation. No overlapping ledger minutes are summed. The integrator owns acceptance and all main/rank/ledger changes.
