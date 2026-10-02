# Display initializer 0x0028F9DC

Frozen base: `45a4305466a7c4cbd87589169384102e88f8d1b5`.
Branch: `dot/display-initializer-28f9dc`.
Final source checkpoint: `168991f` (identical source inputs to form 2, `e77796f`).

The complete 3,092-byte root is reconstructed as ordinary C++ under `NON_MATCHING`. No exact function or byte is claimed. The closest submitted compiler section is 3,096 bytes; a separate diagnostic link differs in 920 positional bytes including the four-byte extent difference. Its canonical checker rejects source closure before comparison. Data and callback identities remain independent evidence, not map modifications.

## Identity and behavior

The original U row is `0x0028F9DC,0x002903CC,0x002905F0`. Its two direct callers at 0x001022D0 and 0x0010234C pass allocator/release callbacks. Existing map names for `nngxlowInitialize`, `nngxlowRegisterInterruptHandler`, `nngxSplitDrawCmdlist`, framebuffer/vertex/shader/texture/state managers, and `glGetError` independently establish graphics-library display initialization. The public API spelling of this root remains unproven, so the address name is retained.

The callbacks have four arguments. The first is area/category 0x10000, the second an alignment/allocator mode, the third a user/list id, and the fourth allocation bytes or the released pointer. The first allocation is 0x2FDC bytes, not 0x10000 bytes. It is divided into state +0, framebuffer +0x758, vertex +0x768, shader +0xFAC, texture +0x2720 and validator +0x2FCC slices. The original managers make further allocations. The command buffer allocation is 0x101C0 with mode 0x105; it reserves 0x10000 data bytes and sixteen 28-byte records. First initialization allocates 0x8010 for an aligned 128x128 transfer record, then releases the original unaligned pointer after waiting.

Null callbacks or an already-set initialization byte return zero before changing state. The routine clears 0x180 bytes, writes 240x400 and 240x320 dimensions, records both callbacks, sets two enable words and flags 0x700, initializes the six graphics managers, and creates/binds command list 1. It clears the old graphics error before command-list setup, checks the subsequent error, initializes the low-level backend, records the speculative request count, and installs seven callbacks.

First initialization issues 68 ordered hardware-register writes: 64 ordinary writes and four masked writes. Their constants in the source are hardware register addresses and values, not machine instructions or an opcode interpreter. It adds a transfer record, handles 16-byte alignment, starts submission when work remains, waits for the busy byte to clear, releases temporary storage, cancels or clears submission state as appropriate, unbinds/deletes list 1, and sets the initialized byte. Failure exits delete list 1 and return zero. They do not release the manager allocation; no additional cleanup was invented.

## Independent metadata observations

The pool marker is interleaved. Executable instructions occupy 0x0028F9DC..0x002903CC and resume at 0x0029054C..0x002905F0. The intervening 0x180 bytes are literals. Full-root replay loads all 3,092 original bytes, including the resumed cleanup/success/failure tails. No map boundary or pool marker was edited.

Context 0x0041CFA0 has no existing map row. The root explicitly clears 0x180 bytes at that address and reads/writes its +0x10..+0x18 flags, +0x9C current list, +0xA0 active list and +0x164 flags. Independent list bind/delete roots and display callbacks use the same address. The declared 0x180-byte structure is the observed cleared/accessed extent; that is not proof of a standalone allocation boundary.

Allocator data at 0x003E2654 spans a 20-byte observed prefix across existing map rows 0x003E2654..0x003E2658 and 0x003E2658..0x003E2668. No rows were merged. Existing pointer words 0x003E2E30 and 0x003E2E34 hold command-buffer current/end positions. The seventh callback value is 0x0028AEF0, loaded from 0x00290400; it is a `mov r0,r0` entry that falls into the command callback prologue at 0x0028AEF4, currently inside the preceding row's declared literal interval. The preceding routine returns at 0x0028AEE0 before its literals; the registered callback restores its frame at 0x0028AF94 and tail-branches at 0x0028AF98 to the existing nngxlowUnlock entry, which has a direct return at 0x0028A808 and a separate slow path. This is entry/control-flow evidence, not proof that any current data boundary is valid. That independently observed registered entry needs a main-owned identity/boundary decision. The diagnostic linker gives these missing references their observed addresses only in an ignored SYMDEFS file.

## Four forms and canonical result

| Form | Committed input | Structural change | Section bytes | Diagnostic positional differences |
|---|---|---|---:|---:|
| 1 | 1747aef | Complete root, late manager slice expressions | 3080 | 2427 |
| 2 | e77796f | Retain all derived manager slice pointers across calls | 3096 | 920 |
| 3 | 9f09764 | Temporary allocation exists only on first-initialization path | 3104 | 1843 |
| 4 | bbc11f5 | Explicit recovered 28-byte transfer-record fields | 3100 | 1282 |

The final checkpoint restores form 2 unchanged. Four unsuccessful meaningful source forms exhaust this working pass. No flags, compiler configuration, target, checker, or function boundaries were changed. Placement in the existing CtrSDK module selects ARMCC 4.0 build 902; that configured compiler role is measured, while the original root's compiler identity is not independently proven. No alternate compiler was tried.

Committed-source build: `python make.py eu -ca` returned zero, compiled 45 Game/129 al/2 SDK sources, linked and exported in 28.696795 seconds. The function is U and absent from the compact acceptance scaffold, so that successful link does not establish its external identities. A temporary name on its existing row allowed the unchanged canonical command below; the original map bytes were restored in a finally block.

```
python tools/check.py fn_0028F9DC --object build/eu/obj/lib/CtrSDK/sources/DisplayInitializer.o
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
exit 1
```

The diagnostic links use that same unchanged canonical ARMCC object; one is at the retail address for positional comparison, one at 0x00500000 for replay. Neither link changes a rank or provides canonical credit. The final source/header contain one root and no emitted local helper functions.

## Preservation

The coordinator's pristine base clean build and canonical checks returned zero for all 707 accepted roots and all 726 actual canonical definitions in 459.927474 seconds. This proposal separately compares every one of its 175 pre-existing canonical objects and 365 recorded input hashes to that baseline. Compiler hashes, normalized commands, all non-file symbols, section contents/flags/alignment/attributes and relocation-resolved identities agree. Only STT_FILE absolute paths, repository prefixes in nonallocated .comment, and consequent symbol/string-table indices are normalized. No shared source/header changed.

This establishes preservation by the checked pristine baseline plus exhaustive unchanged-input/object equivalence. It is not a claim that all 707 roots were individually checked again in this worktree. Main still performs its acceptance gate on import. Full evidence hashes and per-object equivalence are in `display-initializer-28f9dc-evidence.json`; executable verification recipes are in `display-initializer-28f9dc-replay.md`.

## Final immutable inputs

- `lib/CtrSDK/sources/DisplayInitializer.cpp`: `974b508210515cace41dc88455bf2f2cabfe981935228c2cd19ffebbac5987b4`
- `lib/CtrSDK/include/retail/DisplayInitializer.h`: `2fd0f0080f0ba716b60124b60c6d250b39354aea62b86f8639d62bd5e31a847f`
- `build/eu/obj/lib/CtrSDK/sources/DisplayInitializer.o`: `2d080c309e304459a20a70be04b832246ec7f1af92b463512432e0ae7d3c6604`
- `build/display_init/behavior.axf`: `6806e7d03808f0ef10088d9b134b9b008b9fc2e01535b5760b6040a9dd075977`
- `data/ver/eu/map.csv`: `d517e7aa3eefdbb859e781d7d974f8daf4fbcb33b964edb7d0a5ef6a11db8bd9`

## Bounded differential replay

The final 69 ordinary-fixture suite agrees on 320 returning pairs and 20 matching invalid-memory fault pairs across five FPSCR initial states. Five additional wait-prefix pairs agree and are classified separately: their busy flag is deliberately never cleared, and execution stops after ten modeled yield calls. These prefixes neither prove unbounded nontermination nor establish a completion guarantee.

A separate extended allocator-side-effect fixture agrees on five returning pairs. Its first allocation callback installs a valid pre-existing command list/buffer after the root clears Context. The original bind/search/delete callees execute normally, allowing the root's old-buffer release path to execute. This fixture models an externally supplied callback changing global graphics state; it is not evidence that the game's allocator does so. The combined 350 pairs comprise 325 returns, 20 faults and five bounded wait prefixes; all agree in 41.190058 seconds. The ordinary suite covers 673/677 root instructions; the extended scenario supplies the four old-buffer free instructions, giving 677/677 combined instruction coverage. Instruction coverage alone does not prove all paths or behavior.

The unchanged canonical object is linked at 0x00500000, and Unicorn ARM1176/VFP executes it and the entire original interval separately. All six original manager initializers execute, except where a named failure control supplies a negative result. Original memory-clear, command-list bind/search/delete, graphics-error and their downstream unhooked callees execute. The successful original path makes ten allocations including manager allocations and agrees on the complete ordered allocation/release, register-write, callback-registration, bind/delete and modeled scheduler trace. The transfer temporary tests every 16-byte misalignment.

Modeled endpoints are allocator/free callbacks; low-level initialize/first-initialize/speculative-query/callback-register calls; ordinary/masked hardware writes; split/setup command counters; physical-memory query; lock, unlock and submission; and yield scheduling. The model captures all 68 hardware address/value writes but does not execute the GPU. The split endpoint varies record counts, processed counts, mode and busy state. One final-lock race fixture sets busy again and exercises cancellation. These are constructed interface states, not observed gameplay or real OS scheduling.

Memory hashes compare 0x003E0000..0x00410000, 0x00410000..0x00430000 and the 4 MiB allocator arena 0x00800000..0x00C00000 after each pair. Stack scratch bytes and memory outside those domains are not compared. Returning pairs also compare return value, r4..r11, restored SP and resulting FPSCR. Fault pairs compare fault class/address/size, events, memory and FPSCR; caller-saved registers and stack scratch at the fault are deliberately not compared. The four fault scenarios fail allocations 6..9, exposing original unchecked manager/list/buffer allocation behavior. The instruction bound is 300,000 per run; intentional wait fixtures instead stop on the tenth yield. The five initial FPSCR values are 0, 0x00400000, 0x00800000, 0x00C00000 and 0x01000000 (four rounding modes and an additional flush-to-zero state).

No native game execution, rendering, hardware correctness, general memory safety, arbitrary callback behavior, or whole-program equivalence is claimed. The canonical metadata/size mismatch remains even after these bounded agreements.

## Measurement and handoff

Assignment began 2026-10-02 00:37:54 UTC. Final source, clean-build, canonical rejection, preservation and expanded replay evidence were complete by 00:58:44 UTC: 1,250 elapsed seconds. New accepted complete bytes: zero; accepted throughput for that measured investigation/verification window: 0 bytes/hour. Reporting and coordinator publication follow separately. No main integration or acceptance has occurred, so this result contributes no accepted throughput there either.

The source-only family patch against the frozen base is generated with the reproduction recipe; it adds exactly the new header and translation unit. Reports are separate. Main must adopt/review the context, allocator prefix and callback-entry identities independently before canonical checking can progress. Even then the final section remains four bytes too long and diagnostically differs in 920 bytes, so this proposal is blocked NonMatching, not pending exact acceptance.
