# Passive original guest writer observation

Root owns `root/guest-write-observation`, frozen origin/main
`6c0c41d954b9a8f0b923067d94cc3e27182f48d9`. The port needs coherent natural
writer evidence before Pro can assign the application root's current slot a
layout or gameplay role. This family adds optional bounded passive Write8/16/32
observation in port runtime, a native frontend option and a separate provider
CMake patch. It changes no Game/lib/config/map/rank/Factory/ledger, original code,
scheduling, existing memory-trace API, accepted provider patch or comparer.
Matching claims, accepted matching bytes and newly translated bytes are zero.
The unchanged native module links 2437712 function bytes.

## Submitted implementation

| File | Final SHA-256 |
| --- | --- |
| runtime/port/GuestWriteObservation.h | c871e4826f242c723608fe0fe7816d7718fc76dbefdd6292ecb56ab0b736f966 |
| runtime/port/GuestWriteObservation.cpp | f8082e973ff413d038bd826aeaa5b5d089381a21826e5fe5d59b3bdffdafbcee |
| runtime/port/StaticArmBackend.h | 223b0192a77bb7f82429b9bd124d419cd0277ee012b564dd029aad371cf8ae7e |
| runtime/port/StaticArmBackend.cpp | b2888dd4c6627df65ae11f7aaef25be5021dce080fb85d37a5be26ccf4343ffd |
| runtime/port/AzaharStaticExecution.cpp | b011762fdadef70196ebac73e4a7f049d348def92eebb4dafffb043ad537b5f7 |
| tools/static_recompiler/azahar_reference/azahar_guest_write_observation.patch | 701b43aad49fe24e1c6030b7b142111e432381dec17a7711837411c649ee044f |

The selected address/offset are external runtime configuration; public source
contains no selected game address, captured registers or original instructions.
Direct reads validate every byte as ordinary mapped RAM in the actual memory
system page table before dereferencing. They do not call guest, Read, MMIO,
rasterizer or service paths. The original actual write executes once. Charge,
callback and post-store contexts remain separate. A readable precise instruction
address remains a candidate until original-code and source/destination checks.
Read-invalid fields, unavailable selection and imprecise spans remain explicit.
An exclusive output, event/byte bounds and an explicit footer prevent silently
crediting a truncated or failed stream. Complete describes output transport.

## Actual isolated native build

Ignored builder: `build/build_isolated_guest_write_provider.py`, SHA-256
`92f331f0961961fa1ed6e69ff4e9d8542d69ff6ab1a69b4b57b0308c393050e9`.
Run from this worktree after sourcing the primary development environment:
`python build/build_isolated_guest_write_provider.py --execute`.
It requires an absent output and the exact sealed original/bounds build closure.
The recorded build checks the additional patch on a copied CMake file, discovers
headers with the compiler, seals them before compilation, and requires actual
compiled dependencies to match. It rebuilds StaticArmBackend, AzaharStaticExecution
and the new GuestWriteObservation object. Six unaffected direct objects and the
sealed bounds core/headless archives remain read-only. No old provider changes.

Receipt: `build/guest_write_provider/build_receipt.json`, SHA-256
`91c1278d814661cce745ded7f48494ab675ce1e1c2604a32f5e5e54f19e5b397`.
All eight stages return zero; build completes in 5.728786708 seconds. All 3376
recorded input identities and 2229 installed alias records preserve; 19 outputs
are sealed. The native executable is 31752056 bytes, SHA-256
`9f97405ca63de81610777afda67035cbda9b644bb71ebf87766c148f8125153d`.
The compiled-module target is not built. Implicit system framework/linker/runtime
closure is not claimed hermetic. No matching ARMCC build is claimed.

## Full movie preservation and real limits

Ignored runner: `build/replay_guest_write_movie.py`, SHA-256
`011ab90f66425de6af1074f09fa4aada08615d25a39c597408e0baf7d939ebb9`.
It checks the complete live build receipt and native module/schedule/dump/comparer
pins, seals every original reference and snapshot file before/after, uses the
original movie without native input-script injection, and requires all complete
capture outcomes plus two native CPU reports. Original snapshots and captures
stay read-only. Its recorded arguments/environment are in each execution receipt.

| Native 360-presentation case | Seconds | Full comparison SHA-256 |
| --- | --- | --- |
| Observation unset | 56.284678083 | 2045582715fd95c50b7e2b71529a83d87130fb3236acd75691f5a8b783b486fc |
| Current-root slot selected | 70.366754875 | ed20226252835ff8d41dea2ff5c7711999e1ded615576eed205bd3d2efe297cf |
| Unreadable aligned root selected | 62.232283042 | 711a1f627f053d818d4be3a8966cb8e312a3cab21feb8214fd6aa3478ca6af68 |

Every unchanged full movie comparison passes all 1802 HID polls, 4583 GPU events
and ticks, 545 PICA files/8642912 bytes, 1350 sound blocks/216000 stereo frames,
864000 PCM bytes, 691200 RGBA and 345600 framebuffer bytes. CPU0 executes
393977876 instructions, CPU1 zero, both report zero interpreter/JIT fallback.
All sealed original inputs/reference files preserve. Unset creates no stream.

Enabled output is 8306 bytes, SHA-256
`1e504de3e384fc75489c5171e59cf8c285ff047a734ece8f4dd794667e87c290`.
Its complete footer reports 56571307 callbacks, two emitted and destination-byte
confirmed events, zero omitted/failed writes and zero imprecise events. The first
root store has unavailable old slot because the root is null. One later four-byte
current-root slot store has a coherent valid root/slot join, stable actual/backend
page tables and byte locations, matching original instruction, and matching
charge-entry source/effective address and post-destination. Full private validation
packet SHA-256 `24ac4b108a8532cd4189a2420bcbf43bfa0c1145033beff40736b182c8f5b007`.
The unreadable-root capture confirms zero emitted events and all 56571307
callbacks counted unavailable for both root and derived slot, with exact replay.

`build/guest_write_preparation/observation_controls/result.json`, SHA-256
`fba645ab7d00781e4ea6c2e14b0ddb5fce5e2c2ef4bf3a455387cb152091078b`,
passes twelve real controls. Partial/signed/unaligned selections, zero/oversized
event limits, undersized/oversized byte limits and existing file/link outputs
refuse before capture. These ten refusals return -6 on the existing uncaught
frontend configuration exception path. Existing fixture bytes and link targets
preserve. Natural event-limit and byte-limit cases return 1 after actual stores,
with incomplete footers and omitted-event counts. The one-event case emits one;
the 8192-byte case remains within 8192 bytes. Failed partials earn no replay credit.
All control-declared inputs preserve. Separate post-control full original
movie/snapshot/reference seals pass, receipt SHA-256
`6c6c9150ed95513a66f963a7133efc66cafe91633e00a0b3c651216f12710982`.

## Limits and next work

Host times include local contention and prove no performance guarantee. Partial
overlap, same-value, wrapped-destination, readable cross-page, exclusive-store and
page-remapping branches were not exercised by these captures. Callback coverage
excludes HLE/direct writes and physical aliases; root/slot-unavailable counts
remain gaps. Consecutive reads are not an atomic freeze. After-store is not full
architectural instruction completion. A committed byte witness establishes no
class, player identity, lifetime, ancestry, update completion or World 1-1 goal.

The self-contained natural writer question is queued for Pro in primary
`.integrator/pro_queue/layout/world-one-application-slot-natural-writer.md`,
SHA-256 `50aa0fccc4801b44d8482c15c5a7644d65545a2dd3d20ba9087f31aee7b2930a`.
Its answer remains a proposal. Independently, the bounded 7200 stock capture
completed with original inputs preserved and shows the World 1 map, not entered
level 1. Its exact native movie replay is running in root/gameplay-capture-bounds.
Milestones 5 and 6 remain in progress. No owner question is required.
