# Bounded replay duration evidence

Root/replay-duration starts from main `f6539c9732ebe3426a420dc8725eccaa771f7d2b`, including accepted audio observation. Root owns the source-only host watchdog option and docs. No matching inputs, ranks, ledger or exact claims. The single helper independently reviews final source, checks reverse patch application and finds no new defect. Downloaded tools and every guest-derived artifact stay ignored.

Final patch SHA-256: `7fc6757b850aab4e8b935d0a67c841daadc0faeb83245574d5e78da8faf37822`. Resulting headless source SHA-256: `c037fda02ccaffba69b8c7bbad724d0aaba3b5444f3c115de62c8fffbb83a91a`. All seven source patches apply independently through a temporary index to official Azahar `662d412123305a9f4be94dd3dc73ddf91a18c55e`. Resulting tree `f83d2483b52379caf4698bc02cb9de19aead2d29` supplies the exact built headless blob. Both targets rebuild with one compile slot. Fresh dependency download and another-machine reproduction remain unverified.

`ROOT_PORT_CAPTURE_WALL_SECONDS` accepts decimal values 1 through 3600, with an unset default of 120. It changes the cooperative outer host deadline and records the effective value. CPU instruction accounting, guest ticks, fixed clock, HID/movie order, audio and renderer semantics remain unchanged. [REPLAY_DURATION.md](../tools/static_recompiler/azahar_reference/REPLAY_DURATION.md) provides application/build/use commands.

## Final-source controls

| Check | Observed result |
|---|---|
| Invalid syntax/range on both backends | 22 refusals, exit 2 before capture-directory creation |
| One-second stock/native deadline | Exit 1, wall_time_limit, incomplete GPU/input/audio |
| Upper bound 3600 and leading-zero 000120 on both | 4 successful first-swap captures, effective values correct |
| Unset default stock replay | Exit 0, value 120, exact prior audio/input/GPU/pixels |
| Extended native replay at 300 seconds | Exit 0, value 300, exact prior audio/input/GPU/pixels |

All 28 parser/deadline controls pass. One-second process times are 1.313 seconds stock and 1.839 native, including startup/shutdown outside the deadline. Default stock replay takes 39.296 seconds. Extended native takes 103.883 seconds while a longer stock investigation runs concurrently; these are individual elapsed times, not sustained performance estimates.

Both full replays preserve 216000 stereo sample frames, all audio event/tick bytes, 1802 HID polls, 4583 GPU events/ticks, 8642912 PICA bytes and screen/framebuffer streams. Native executes 393977876 CPU0 guest instructions with zero interpreter/JIT fallback. The stock executable SHA-256 is `a062fdd74c41ab6656b9993cc82cf141e723563e14b4b9cffe7dd5aa6fd2b061`; native is `dfdca89c9708027cd75136c5eaf5d0e3dc9cd213712453eec362732ca6dfa412`. Translated library, schedule and comparator are unchanged.

Local receipts in this branch's ignored build: `replay_duration_build.log`, `replay_duration_patch_sequence.json`, `replay_duration_controls.json`, `replay_duration_valid_controls.json`, `reference_default_receipt.json`, `native_extended_receipt.json`, `duration_default_preservation.json`, `duration_extended_native_preservation.json`, and `replay_duration_helper_review/replay_duration_review.md`. Captures are in primary `build/root_port_duration/`. The full comparison uses the original accepted `build/root_port_input/reference_scripted_360/input_movie.ctm` and its initial snapshot.

## Limits and next investigation

The default configuration gains one metadata field, so historical raw configuration files are not identical. Existing validators still check original identity, clock, base ticks and sink. The cooperative watchdog cannot interrupt one RunLoop call, startup or shutdown. A selected boundary completed within the last slice wins. Invalid-option refusal excludes an independently requested native memory trace opened before the common frontend.

The extended 1500-presentation setup investigation runs for 196.032 seconds, beyond the old deadline, then exits by SIGTRAP in the upstream MiiSelector assertion. The headless frontend has no selector. This partial capture has no successful outcome and supplies no World 1-1 or replay credit. Preserve `build/root_port_world_one/reference_menu_step_3` and this worktree's `build/reference_menu_step_3_*` receipt/logs. Default frontend registration is a separate next patch; the watchdog did not conceal or repair that failure.

An accidental submission at the unchanged base f6539c973 contained no source patch because the local commit guard looked for the native instruction count in stdout rather than stderr. The finished source is committed only after correcting that guard and verifying all final receipts. Do not retry while that submission is pending. No new runtime or matching credit follows an empty submission.

Pro has the ignored `world-one-player-observation.md` question. Player/camera/RNG/timer/coins, the complete World 1-1 route and browser execution remain unverified. Milestones 1 through 4 retain bounded native evidence. Recompiled coverage stays 2437712 bytes; the complete rank-O native-source ABI registry remains open.
