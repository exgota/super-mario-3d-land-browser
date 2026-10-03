# Finite capture range and World 1 map replay

Branch `root/gameplay-capture-bounds` freezes main
`1ecf645ef2f06b17cd47cddae2c15f763ae1f2e3`. Root never moves main, ranks or the
ledger. This family changes five source lines in two public capture translation
units through a source-only patch and one rendered-loader range line. Matching
claims, matched bytes and newly translated bytes are zero. The existing native
guest module still translates and links 2437712 function bytes.

The capture ceiling becomes 7200 presentations and the supported aggregate
PICA option becomes 2 GiB. Unset presentation, PICA and wall-time behavior stays
unchanged. The rendered-loader zero-based index ceiling becomes 7199. No event,
tick, payload, screen, input/audio equality or completion criterion changes.
No source game file, matching build input or protected assembly oracle changes.

All private paths below are relative to this worktree unless a primary path is
spelled out. Tools, original images/audio, generated guest code, movies,
snapshots, executables and raw payloads remain ignored and are never pushed.

## Isolated native build and original-range checks

The source patch is
`tools/static_recompiler/azahar_reference/azahar_gameplay_capture_bounds.patch`,
SHA-256 `cfd8c37da375792981712145e280499bd5973c2ae3598bafddc3b49343c0b76c`.
Forward and reverse application against original-derived copies pass. The
original Azahar source, provider build and executable bytes are preserved.

`build/gameplay_capture_provider/build_receipt.json`, SHA-256
`4dba7340b833612492175701d24ac8bf877c4a2567a65f0c86d6d9d8c2689943`,
records return code zero, all 12 stages successful, 5.0894295 seconds and all
2133 reused source/tool/header/object/archive/library identities preserved.
Three affected objects, two copied archives and two isolated frontends are
built. Installed header aliases retain their declared path and byte identity.
Implicit system frameworks/runtime are not a hermetic closure claim.

The isolated stock/native executables have SHA-256 identities
`e8406d44f2173ac3b107458fd5f6e6aa4e3dabe2ac45ea559bcdbf021a907493`
and `d0ade201075ffc5c2a9067e87c26b05df4ced406c484049f816a24e7b62c99e7`.
Both replay the retained original 360 movie and snapshot with unset PICA/wall
options. Full original-loader reports pass, SHA-256 `bac96963d033094d444f1b7011aff66a8d77b89bcfec14b133dbb88edd187c17`
and `9a644dce354aa5c99859bcc35142e2d2f4d498c15f0dab48ddda7c496fb080cb`.
Native CPU0 executes 393977876 instructions, both CPU fallbacks zero. Actual
old/new upper refusal behavior preserves in four cases; receipt SHA-256
`8df26fa33154b96de67ecd6e7d96f673ffa0381fb2ac8352d05b20215fa890df`.
The initial incorrect return-code expectation remains a retained failed result.

## Actual 7200 recording and independent replay

The first recording failed at the 400000-file monitor. Its unchanged receipt,
SHA-256 `d399a8b7f2b054bb6bbb11911c1108ac4bdff6c605c903789b353895bb209649`,
records child return -9 after 1111.045487125 seconds, last renderer6233,
405376 files and 1438582864 bytes. It receives no complete replay credit.
Only its 405337 owned failed PICA payloads were subsequently retired after
individual hashing; all other failure files preserve. Their retained manifest
SHA-256 is `860642d950ba8b908d1e28a5e438f80fd5d5bd7bf7b1f3bf998614496c3a1066`.

A separate retry changes only the monitor to 720000 files. It preserves the
63-state input script and exact earlier 35-state prefix, fixed initial clock,
7200 presentations, 1800-second child/1860-second outer limit, 2 GiB PICA,
2560 MiB monitored total, 128 MiB per file and 5 GiB free floor. The monitor runs
every two seconds and is not a filesystem quota. Only its directly owned child
can be stopped at a crossed bound.

Stock completes in 1373.300610667 seconds, return zero, all original inputs
preserved. Receipt:
`build/gameplay_capture_preparation/reference_gameplay_continuation_7200_file_extent/execution_receipt.json`,
SHA-256 `02c49f8faada354af24681258388fa1cf12dc3d9bd9f05ddd2cb90144eb55d1d`.
Its own original movie is 488037 bytes, SHA-256
`e5abdcebc3270146766b68c81d30bae78280afb3a911433f2bc7807dc97d17a8`.

Native replays only that movie and recorded initial snapshot, with no script
injection. It completes in 1288.546525708 seconds, return zero, CPU0 2922969242
instructions and both CPU fallbacks zero. All protected inputs, original
capture and snapshot files preserve. Receipt:
`build/gameplay_capture_preparation/native_gameplay_continuation_7200/execution_receipt.json`,
SHA-256 `c4ccfa9dac6a11a39b0ef3bcf4afc417d05871f8276b3e0f44abe9c129acd9f8`.

## Range-contract repair and complete comparison

The old full comparer refused both actual presentation indices7199 because its
rendered loader supported only3599. Input and audio already matched. Retained
failed report `native_gameplay_continuation_7200/full_movie_comparison.json`
has SHA-256 `c7b7f891ad96c51e093b3a7832f0b8aea14e07d5c9aa78a5402e676a603bb90e`.
It remains failed. The only loader change raises that supported extent to7199.
Original loader SHA-256 `49ef688c02c8f0b4f8a619213054f6d72348a31433b09e3c88027984de344c3f`;
final loader SHA-256 `e6e9431c24bf7fda4c145720a44709920053e1dab4fd22e84042589611dc6069`.
The full movie comparer itself retains SHA-256
`739a407d7965e7fffd4cfed32cef9ba31bc7866a8f8b1ed960e50967f74434c0`.

`native_gameplay_continuation_7200/full_supported_range_comparison.json`
passes, SHA-256 `913c7dc7980360a3fce50293c09093e5290eee622c31531baa1709dd5e332e2f`:

- 598803 raw GPU events and ticks, 516903 PICA files/1611203488 bytes.
- 28554 delivered HID polls; all63 authored states observed.
- 24734 audio blocks, 3957440 stereo frames/15829760 PCM bytes.
- Both screen metadata, 691200 RGBA and 345600 framebuffer bytes.
- Complete original outcomes, no validation or Movie errors, no CPU fallback.

Final-loader former360 stock/native reports also pass, SHA-256
`9a15fd1623672fb15fb8a12c09bbad197bc3b58b56e82eef873e3d6ee45d213a`
and `1b6949424366784c999ab30a80cb7f3189d5061292563f0e8da70fdec3494856`.
`native_gameplay_continuation_7200/post_comparison_original_preservation.json`
passes, SHA-256 `9e86b557392cef604aad968b127d572f5edbca3d99c860a57e9650b0156c4d51`.
It rehashes all protected inputs, all516944 original stock files/1775106072
bytes and the original snapshot after comparison and authorized root cleanup.
The complete original capture inventory/content seal remains
`e5724574f5f57b51d7c506f201ab0852e3b1feae87ce314e9bacad57da58ffd0`.

Root viewed both stock and native final PNGs outside the original capture.
They show the World1map, Mario at its starting node and four lives. World1-1
entry, controlled-player identity, completed-update state, goal completion,
Section7's full level suite and continuous browser performance remain open.
No milestone is marked done by this family.

Under the owner's later cleanup authorization, finished earlier browser
profiles, duplicate compiled outputs and old3600 capture payloads were retired
after sealing. Their old result/receipt/source/movie/snapshot evidence remains;
rerunning retired comparisons requires regeneration. Current7200 raw captures,
their inputs, provider outputs and the saved-profile boot source remain intact.
Primary `build/root_scratch_reclamation/reclamation_receipt.json`, SHA-256
`de23be3b73bcfa775326b2116973dfb191dd01019846fe895beaf2c57a378d0d`,
confirms542674 files removed, all41470 retained files preserve and free disk
24506277888 bytes, exceeding20GiB. Its individual removal manifest SHA-256 is
`1938ebba58e1fe2d93c5ec9c07283248325fad66b2957978fdd74caee3bfaa4c`.
