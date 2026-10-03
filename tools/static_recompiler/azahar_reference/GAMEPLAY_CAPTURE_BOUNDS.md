# Longer bounded natural captures

The existing 3,600-presentation ceiling stops the retained exact reference/native
replay in the opening cinematic. The source-only
`azahar_gameplay_capture_bounds.patch` raises that supported ceiling to 7,200 and
the aggregate PICA payload ceiling to 2 GiB. It changes five lines in two capture
translation units. A larger supported bound does not establish World 1-1 entry,
controlled-player identity, completed-update alignment or Section 7 replay.

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

## Planned isolated verification

The translated guest library and sibling block schedule remain unchanged.
Check relevant invalid selections and both PICA validation sites. Record a
longer natural stock capture and
replay its own original movie and initial user snapshot independently on native,
with complete outcomes and unchanged input/GPU/PICA/pixel/audio/tick checks.

The root lane's planned recording selects 7200 presentations, the existing
1800-second host option, 2 GiB of PICA bytes, and input/audio capture. Planned
external bounds are 1860 seconds, 2560 MiB total output, 400000 files, 128 MiB per
child output file and a 5 GiB free-disk floor. These are planned selections and
monitoring limits, not provider defaults or observed successful results.
Retain failure receipts and partial outputs without success credit. Inspect the
actual longer presentation before claiming World 1-1 entry, then establish
controlled-player and update semantics independently for Section 7. This patch
claims no milestone completion.
