# Longer bounded natural captures

The existing 3,600-presentation ceiling stops the retained exact reference/native
replay in the opening cinematic. The source-only
`azahar_gameplay_capture_bounds.patch` raises that supported ceiling to 7,200 and
the aggregate PICA payload ceiling to 2 GiB. It changes five lines in two capture
translation units. A larger supported bound does not establish World 1-1 entry,
controlled-player identity, completed-update alignment or Section 7 replay.

The rendered-capture loader's finite presentation-index maximum also changes
from 3599 to 7199, matching the zero-based index of this supported interface.
Its event, tick, PICA, screen, framebuffer and completion requirements remain
unchanged. The full movie comparer and its input/audio equality rules are
unchanged. This is a range-contract repair, with no tolerance or exclusions.

## Existing environment selection

| Existing option | Supported range after this patch | Unset behavior |
| --- | --- | --- |
| `ROOT_PORT_CAPTURE_PRESENTATION` | Positive decimal count, 1 through 7200 | Stop at the first top-screen GPU buffer swap, as before |
| `ROOT_PORT_CAPTURE_PICA_BYTES` | Positive decimal byte count, 1 through 2147483648 | Keep the 134217728-byte aggregate limit, 128 MiB |

The presentation count still selects a natural software presentation after
top-screen submission. It is distinct from renderer frame, VBlank count and guest
ticks. Both the GPU initializer and headless parser accept the same PICA upper
bound. Their existing range errors now say "between 1 byte and 2 GiB". The generic
presentation-range error is unchanged.

Decimal parsing, zero/invalid-input rejection, command copying and the chosen
aggregate payload check retain their existing implementation. A command list
that crosses the selected PICA limit is still refused before its payload is
written. Guest execution, instruction costs, ticks, scheduling, services, movie
delivery, input, audio, passive observations and capture outcome handling are
unchanged. Partial or failed captures remain unsuccessful, including a fatal
exception on the existing stock callback path. No option, diagnostic or output
schema is added. The existing wall-time selection and its default are unchanged.

## Source-only application

Follow the pinned official Azahar source and accepted patch sequence in
[BUILD.md](BUILD.md) through [STATE_OBSERVATION.md](STATE_OBSERVATION.md), including
its preceding rendering/input/audio, replay-duration, default-applet and replay
payload-limit dependencies. Apply this patch last to a separate ignored source
root with that complete accepted closure. Use `git apply --check` before
`git apply`; its paths are `src/root_port_capture/` relative to the Azahar source
root. It needs no generated guest code or game data to apply.

Do not apply it to an existing provider source, build or executable used by
retained evidence. The two modified copies under ignored
`build/gameplay_capture_source/src/root_port_capture/` are source preparation
only, not a complete Azahar checkout or a compiled provider. Root owns assembly
of the full isolated source/build closure and the actual build commands.

The exact original inputs and prepared outputs are:

| Source file | Original SHA-256 | Modified SHA-256 |
| --- | --- | --- |
| `gpu_command_capture.cpp` | `4cb3a0fce9432c92795b4817a7d0f05d8dbb6c120f58c0c9aff82024637b077b` | `cfa20c3df0fe2691b8932f9df82eedd4153713d02e575362de5a0619b940b1c5` |
| `headless_capture.cpp` | `34f6749aca9bc813b0423c3ba0ba546978b3964d1b4cd9a6f7ff91d63fb4a1b1` | `368af89e0be00a9139fb75905f656cacd1ad0756151ee2d7b30542be90107802` |

The originals were read from the accepted private provider at
`build/port_tools/azahar/src/root_port_capture/` in the primary checkout. Their
byte counts and SHA-256 identities matched before and after preparation.
The normal unified patch passed independent forward applicability against
original-derived copies, applied to those copies, and passed reverse
applicability. The resulting copy bytes equal the prepared final source.
Preparation alone performed no compilation or guest execution. Root subsequently
applied the patch independently to another original-derived source copy and
built two isolated frontends without editing the old source, build or binaries.
The recorded build compiled the GPU object and both headless variants, replaced
one member in each of two copied archives, and relinked the stock/native
frontends with the original CMake flags and remaining link inputs.

The isolated build receipt is
`build/gameplay_capture_provider/build_receipt.json`, SHA-256
`4dba7340b833612492175701d24ac8bf877c4a2567a65f0c86d6d9d8c2689943`.
It completes with return code zero in 5.0894295 seconds and preserves all 2,133
recorded source, header, object, archive, tool, library and other input identities.
Installed SDK header links retain their declared path and file-content identity.
This reused-input build does not claim a hermetic system framework/runtime
closure or a new matching ARMCC check. Its ignored reproduction script is
`build/build_isolated_gameplay_capture_provider.py`.

| Isolated executable | Bytes | SHA-256 |
| --- | --- | --- |
| `azahar_gpu_capture` | 31661504 | `e8406d44f2173ac3b107458fd5f6e6aa4e3dabe2ac45ea559bcdbf021a907493` |
| `azahar_static_execution` | 31712344 | `d0ade201075ffc5c2a9067e87c26b05df4ced406c484049f816a24e7b62c99e7` |

Both isolated frontends replayed the previously accepted 360-presentation movie
with the original snapshot and no input-script injection. PICA and wall-time
options remained unset, preserving their 128 MiB and 120-second defaults. Stock
completed in 24.1326135 seconds, native in 44.2534956 seconds. The unchanged full
movie comparer passes both against the accepted original reference. Both preserve
all sealed inputs and reference files. Native CPU0 executes 393977876 guest
instructions; both CPU receipts report zero interpreter/JIT fallback.

The stock comparison is
`build/gameplay_capture_preparation/stock_former_range_360_comparison.json`,
SHA-256 `bac96963d033094d444f1b7011aff66a8d77b89bcfec14b133dbb88edd187c17`.
The native comparison is
`build/gameplay_capture_preparation/native_former_range_360_comparison.json`,
SHA-256 `9a644dce354aa5c99859bcc35142e2d2f4d498c15f0dab48ddda7c496fb080cb`.
Each covers 1,802 HID polls, 1,350 sound blocks/216,000 stereo frames and all raw
GPU events, ticks, PICA payloads, final pixels and framebuffer bytes. These finite
captures do not establish gameplay, synchronized continuous sound or performance.

Real CLI refusal controls compare both frontends at the old maximum plus one
and the new maximum plus one. Presentation selections 3601 on the old provider
and 7201 on the new provider both abort with return code -6, the same diagnostic
and an incomplete output directory. PICA byte selections 1073741825 on the old
provider and 2147483649 on the new provider both return 2 before creating the
capture directory. Their diagnostics are identical after the intentional
"1 GiB" to "2 GiB" limit text substitution. Neither case reports a complete
capture; all recorded inputs preserve.

The first control receipt retained a failed expectation that presentation refusal
would return 2. It remains unchanged at
`build/gameplay_capture_preparation/invalid_upper_bound_controls/result.json`,
SHA-256 `60d3297a09902ac2dbb0e95d2311ee95f90f1a9339151ee8678ab34ba91b6465`.
The separate actual old/new preservation result passes at
`build/gameplay_capture_preparation/original_upper_bound_controls/preservation_result.json`,
SHA-256 `8df26fa33154b96de67ecd6e7d96f673ffa0381fb2ac8352d05b20215fa890df`.
No capture exception path was changed to make these controls pass.

## Longer capture and retained failure

The first actual 7200-presentation recording used the existing 1800-second host
option, a 1860-second outer limit, 2 GiB PICA, and input/audio recording. Its
external monitor selected 2560 MiB total output, 400000 files, 128 MiB per child
output file and a 5 GiB free-disk floor. Checks occur every two seconds and do not
constitute a filesystem quota. It stopped the direct child at the file extent
after 1111.045487125 seconds, return code -9, last renderer frame 6233. Final
partial output had 405376 files and 1438582864 bytes. Its largest file was
105266119 bytes. All protected original inputs preserved. It has no complete
footer, native comparison, gameplay entry or replay credit.

The immutable failed receipt remains at
`build/gameplay_capture_preparation/reference_gameplay_continuation_7200/execution_receipt.json`,
SHA-256 `d399a8b7f2b054bb6bbb11911c1108ac4bdff6c605c903789b353895bb209649`.
Root individually hashed all 405337 failed PICA files before removing only those
owned discarded payloads. Their 1307365040 logical bytes occupied 2611175424
allocated bytes. The JSON Lines identity manifest remains in that same directory
as `pruned_pica_payload_manifest.jsonl`, SHA-256
`860642d950ba8b908d1e28a5e438f80fd5d5bd7bf7b1f3bf998614496c3a1066`.
`pica_payload_pruning_receipt.json` confirms all other failed files preserved and
zero remaining PICA payloads. No accepted capture, original input or provider
was pruned. The failed capture is retained as failure metadata and cannot be
used for replay comparison.

A separate retry completes successfully at
`build/gameplay_capture_preparation/reference_gameplay_continuation_7200_file_extent`.
Its wrapper changes only the monitored file ceiling to 720000. All provider,
script, presentation, time, byte and disk limits remain identical. It uses root's
63-state controller script, SHA-256
`d8d82a1e5b858449e814edf7cf3540db57959c03340e4f0088c8812a835baeab`,
retaining the exact previous 35-state prefix. The additional Start pulse is at
presentation 3720 with release 3724; A pulses run from 3960 through 6960 every
240 presentations with release four presentations later. Analog and touch stay
neutral. These script indices do not establish game phase or player identity.

Stock completes with return code zero in 1373.300610667 seconds, preserving
every protected input. Its receipt SHA-256 is
`02c49f8faada354af24681258388fa1cf12dc3d9bd9f05ddd2cb90144eb55d1d`.
The independent native replay uses only that recording's original movie and
initial snapshot, without controller-script injection. It completes with return
code zero in 1288.546525708 seconds, preserving every protected input and every
original capture/snapshot file. Native CPU0 executes 2922969242 instructions;
both CPU receipts report zero interpreter/JIT fallback. Its receipt is
`native_gameplay_continuation_7200/execution_receipt.json`, SHA-256
`c4ccfa9dac6a11a39b0ef3bcf4afc417d05871f8276b3e0f44abe9c129acd9f8`.

The original full comparison retained a failed validation result at
`native_gameplay_continuation_7200/full_movie_comparison.json`, SHA-256
`c7b7f891ad96c51e093b3a7832f0b8aea14e07d5c9aa78a5402e676a603bb90e`.
Both captures reached presentation index 7199, while the rendered loader still
required at most 3599. Input and audio matched exactly. That report remains
failed and unchanged. The one-line loader extension described above admits the
actual supported range and retains every comparison criterion.

The full supported-range result is
`native_gameplay_continuation_7200/full_supported_range_comparison.json`, SHA-256
`913c7dc7980360a3fce50293c09093e5290eee622c31531baa1709dd5e332e2f`.
It passes with no validation errors: all 598803 GPU events/ticks, 516903 PICA
payloads/1611203488 bytes, 28554 HID polls, 24734 sound blocks/3957440 stereo
frames, screen metadata and both RGBA/framebuffer byte streams are exact. All
63 supplied script states were delivered. No channel-coverage requirement or
silent-PCM exclusion was changed. The former 360 movie also passes with the final
loader. Its stock/native report SHA-256 identities are
`9a15fd1623672fb15fb8a12c09bbad197bc3b58b56e82eef873e3d6ee45d213a`
and `1b6949424366784c999ab30a80cb7f3189d5061292563f0e8da70fdec3494856`.
The separate `post_comparison_original_preservation.json` passes, SHA-256
`9e86b557392cef604aad968b127d572f5edbca3d99c860a57e9650b0156c4d51`.
It rehashes every protected input, all 516944 original stock capture files and
the original snapshot after comparison and the owner's finished-scratch cleanup.

The actual final stock and native pixels show the World 1 map, Mario at its
starting node and four lives. They do not show entry into World 1-1. The
translated guest library and sibling block schedule remain unchanged. Player
identity, completed-update alignment, goal completion and the whole Section 7
level suite remain unverified. This family claims no milestone completion.
