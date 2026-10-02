# Default frontend applet replay evidence

## Frozen ownership and source

Root/default-frontend-applets starts from main `5cbdf4bfaa20e953b644cfb525e31bf00e3e5378`, including accepted audio and bounded watchdog tools. Its unpublished branch advanced by fast-forward from `041c04bab9df11de4242b064301f428f2e723a59` before its first commit. Root owns source-only frontend registration, the general movie comparator and docs. Its one helper owns only that comparator and ignored verification; the separate state sampler remains an unintegrated scratch proposal. No matching inputs, ranks, ledger or claims belong here.

The final applet patch SHA-256 is `b36824e1151c2289cb6c1f4a1fc220764e66f6671907b459f7d52444842d57cc`. It adds one header include and the existing `Frontend::RegisterDefaultApplets(system)` call before guest loading. Official Azahar `662d412123305a9f4be94dd3dc73ddf91a18c55e` uses the same registration in its public Qt, Android and libretro frontends. It supplies existing default Mii and keyboard interfaces, with no copied avatar/game data or new HLE implementation. Configured choices are shared equally by reference/native execution. Interactive frontend choices and hardware applet behavior remain unverified.

Independent temporary-index application succeeds both after the six accepted capture/input/audio patches and after the seven-patch sequence including the watchdog. Without watchdog, tree `091b51ec72997193f8e6725a32fe6f57c4b792db`, headless SHA-256 `bcb472db5cc54f98b3f8d4d9286238a29f23d830b559adedc0d2325509362fee`. The observed extended build has tree `a423fb2378fcb6069937a544014f44ae7b88b685`, headless SHA-256 `bc640bcaea0df0ab37fedadc773e3b96dc96df6dbb87c281f7c73f5944c2e8d3`. Built source agrees with the resulting blob. Both targets rebuild with one compile slot.

Final general comparator SHA-256: `739a407d7965e7fffd4cfed32cef9ba31bc7866a8f8b1ed960e50967f74434c0`. The existing input/audio/render comparators are unchanged. They retain their explicit all-channel milestone evidence policy. The distinct general movie replay validator accepts the legitimately recorded channels and exact silent audio, while retaining strict original typed movie identity/clock/order, script delivery, complete boundaries, raw input/audio/GPU/PICA/pixel equality and Movie/Audio error refusal. It supplies no semantic game-state or level-completion claim.

## Actual applet crossing

The previous same-state 1500-presentation stock recording SIGTRAPs in `MiiSelector::Start` at its null frontend assertion, VBlank1471. It remains ignored failed evidence. With the registration, the identical 19-state A-only script completes on stock and its original movie replays completely on native. Final renderer frame is1601, presentation index1499, tick8100489041. Root views file selection with slot A showing World1 and Saving. No World1-1 level has loaded in this bounded evidence.

| Comparison | Exact result |
|---|---|
| Delivered HID polls | 6261 |
| GPU events, including raw ticks | 23987 |
| PICA lists / bytes | 9269 / 71901072 |
| Screen RGBA / raw framebuffer bytes | 691200 / 345600 |
| HLE blocks / stereo sample frames | 5247 / 839520 |
| PCM bytes | 3358080 |
| Different left/right pairs | 311291 |
| Native CPU0 instructions | 587516481 |
| Native CPU1 instructions / interpreter/JIT fallback | 0 / 0 |

All events, payloads and complete outcomes agree; Movie/Audio errors are zero and every first-divergence index is null. Stock takes164.635seconds and native185.037seconds on the recorded M4, with the short stock preservation control overlapping native execution. These are individual wall times, not sustained performance estimates. Stock watchdog300 and native600 do not change emulated ticks; both complete at the same observed guest boundary.

Movie106579bytes SHA-256 `e8e2de5f7384417a125b6ce1214e7267298323097bd0bedc5ac8dab9d9d3ceff`. Input events SHA-256 `6c24b5d1ce92b06957727ed6191b43d7f6f9d2774c1555a9467d3b90e2fa4902`. GPU events SHA-256 `07a16ca38d256594ecfd988bbb26b9f17ec4a299ad68a67de1b014cc1a196e52`. Audio events SHA-256 `27517ff7405fe58ece44a89b668b3c2281e34574363e375f073cdbcf27217ebb`; PCM SHA-256 `ff7c309077ff942b8ac72635800b6f371fcb23759e7e2630c27bad06b1350ac6`. Sound contains distinct channels and25.651430seconds at nominal32728Hz, but listening/live playback was not assessed.

Stock executable SHA-256 `ea63177825e3c27269baa66071267e9efef7aa100623a0845c0f087bfbbc6962`; native `e46a24aa61798524cc16fab1b658d6fe4c4a6136d83a2f822dd9e411a53640b4`. The sealed translated library and block schedule remain unchanged. Default registration also preserves the accepted360-presentation menu capture exactly through the existing full audio/input/render comparator, including216000stereo sample frames,1802HID polls and prior GPU/PICA/pixels.

## Controls and reproduction

All69independent final-comparator controls pass. They cover A-only, neutral and held-button records; silent/constant/one-silent-channel PCM; identity/type/order/clock/schema/extent/boundary failures; raw event whitespace and timing differences; payload/sample divergence; incomplete outcomes; error logs and preview/report constraints. Independently decoded PNG pixels match the fixture's input bytes. These controls validate the comparator's contract, not actual gameplay state.

[DEFAULT_APPLETS.md](../tools/static_recompiler/azahar_reference/DEFAULT_APPLETS.md) gives source application, script convention, native replay and strict comparison commands. Actual source-derived captures are in primary `build/root_port_world_one/reference_default_applets_1500` and `native_default_applets_1500`. This branch's ignored build contains `default_applets_build.log`, `default_applets_patch_sequence.json`, stock/native receipts/logs, `reference_applets_input_validation.json`, `default_applets_preservation_comparison.json`, `default_applets_render_comparison.json`, `final_parent_applets_comparison.json`, and `movie_replay_verification/final_handoff.json` with69controls and final-source actual results.

Fresh tools/dependency download and another-machine reproduction remain unverified. Keyboard use was not reached. World1-1 entry, player/camera/RNG/timer/coins, the goal route and browser execution remain open. Pro owns the queued player layout question; proposed generic sampling code has not compiled or executed. Milestones1through4 keep bounded native evidence; no milestone5or matching credit follows. Recompiled coverage stays2437712bytes and the complete rank-O source ABI registry remains incomplete. Every movie, snapshot, sample, image, payload and generated guest library stays ignored.
