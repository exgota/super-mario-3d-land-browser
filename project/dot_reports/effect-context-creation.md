# Effect context creation proposal

Address identity fn_002E54AC, unchanged target002E54AC..002E5984,1,240 bytes. Latest reviewed maina74ef6247dcf52073d9b05fd0e091aa7e3579563 still ranks it U. This is a guarded, bounded-tested source proposal with zero exact credit. Original SDK class/method spellings are deliberately unrecovered.

## Result and three actual forms

| Form | Source hypothesis | Complete bytes | Canonical outcome |
|---|---|---:|---|
|1, e919fe0|Direct binary-supported pool/field reconstruction|1176|U -> M, different complete section size|
|2,19f5c52|Scalar copies for the two independently observed Vector3 ones assignments|1200|M -> M, different complete section size|
|3,8cf3391|Keep explicit-resource-seed and generated-seed stores in their observed separate paths|1216|M -> M, different complete section size|

The first three forms were committed and built through unchanged python make.py eu on main48c97b5157698b6722a6d96330ad718f5351a854. Their normal builds link/export. No global/shared header, compiler flag, tool or original boundary was changed. The retained third form is also applied to latest maina74ef624 as source checkpoint542c8c3 for fresh canonical revalidation; its result is appended below after that build.

Canonical command:
```
.venv/bin/python tools/check.py fn_002E54AC --object build/eu/obj/lib/al/src/Effect/alEffectCreationFunction.o
```
The exact third-form output is `M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` Checker blob7c0ccd93b7387a2f9afa9b304b5254f34149b318. The function remains24 bytes short, with a smaller stack frame, different saved registers, and some combined stores. No unchanged or cosmetic fourth form was compiled.

## Recovered behavior

The routine selects a1D0-byte set slot and up to eight B4-byte emitter slots, copies two matrices, initializes defaults, prepends emitters to one of256 group lists, binds resource records, seeds per-emitter random state, and finalizes the reference/counts. Independent constructors, alternate creation, and accepted deletion functions establish the layouts; see effect-context-layouts.md.

Important edge behavior is preserved: the free-emitter precheck uses the entire resource count even if the mask selects fewer; early insufficient capacity leaves the reference untouched; set exhaustion clears its pointer; a single signed unsuccessful-probe counter is reused across both pools and successive emitters; emitter exhaustion after a set was selected still reaches successful finalization, without rollback. The alternate creation routine resets its counter and must not be substituted here.

## Local import prerequisites

Only local diagnostic map names on existing unchanged rows were added:002E54AC=fn_002E54AC,002200FC=fn_002200FC,003E26DC..003E26E4=dat_003E26DC. Existing named matrix-copy0027C18C and RNG0024B12C remain unchanged. A new nonoverlapping BSS import row is independently supported:
```
0x004305EC,          ,0x004305F8,          ,U,db,_ZN4sead7Vector3IfE4onesE,
```
Named vector static initializer loads1.0f at00383AC8 from00383D70, loads destination004305EC at00383B38 from00383D90, then writes the three components at00383B3C/40/44. The next existing row is Vector3::zero at004305F8. No original function/data interval was split. These local map changes are not published; main must adopt or independently revalidate the import identities before checking the source.

The native MTX34 declaration is a private minimal48-byte view for the existing mapped copy signature. On joint intake with other native-math proposals, consolidate duplicate declarations into one consistent header rather than combining different member spellings. This branch does not change shared SDK/sead headers.

## Bounded ARM execution

512 paired Unicorn2.1.4 ARM1176 runs of the complete original and retained compiler output passed against an independent memory-state model. VFP is enabled. Results:240 false returns,272 true returns,242 report calls,170 actual retail RNG calls; maximum1,179 instructions per body including callees. Every modeled data byte, returned boolean, external-call trace, SP/callee-saved integer registers and saved D8 agreed. Two cases report exhaustion yet return true, covering partial-allocation success. The actual retail12-float matrix-copy and xorshift RNG bodies execute. Only the error-report callback is a declared non-mutating model, and its message bytes/call order are checked.

Domain: two banks/two records,8-slot power-of-two pools,0..8 resource members, various occupied/free slots, selected masks, group endpoints0/1/127/128/254/255, explicit/zero seeds, signed ranges, and arbitrary matrix word patterns. Pointers are valid and nonaliasing except supported pool/list relationships. No malformed resources, signed overflow, unexpected reporter mutation/reentrancy, hardware faults or concurrency are claimed covered. This is emulator/model evidence, not physical hardware or whole-game correctness.

Since canonical check.py rejects the different size before linking, behavioral replay uses a separately labeled diagnostic ARMCC link of the unchanged full canonical object at the original address with established symbol imports. It neither changes the object nor relaxes or reruns the exact checker with altered rules; no rank comes from that link. Complete candidate1216B SHA25675a1f8cf6d8d9232cdd594801fe230fac61004c0ddf53a676c40c706c9154035; original1240B SHA2561ac10e5798edd2a34e585ab789110d52645e0bf594b7d592cc3efb9dc1dc580c. Original canonical object SHA25658d03039c6f6d0948363428110d736da8b81e4127ac92e97f1d227ed968b3c20. Reproducible link and replay scripts are in effect-context-replay.md; no game bytes are embedded.

132 separate arithmetic boundary cases also confirm the high-word formula for unsigned32-bit state times signed32-bit range. The old state is used for scaling while the stored LCG state advances modulo2^32. This arithmetic evidence alone is not an additional function match.

## Build/publication limitation

A pristine untouched a74ef624 worktree fails python make.py eu -ca at L6218E, undefined Vector3<float>::zero from alAreaShapeCube.o; an unchanged second normal build links. Logs are preserved and main was notified. No local tool repair was made. The owner clarified at2026-10-01 16:27 UTC that the clean-build rule applies to main pushes and dot proposals should continue. This proposal records that limitation rather than calling a warm build clean. Main should revalidate its intake on the fixed clean build.

## Current-main revalidation

Source checkpoint542c8c3 on unchanged maina74ef6247dcf52073d9b05fd0e091aa7e3579563 compiled, linked and exported through normal python make.py eu. Canonical check.py output: `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` The1216-byte diagnostic linked SHA is unchanged. The replay scripts extracted from this report reran against that fresh canonical object: all512 original/candidate/model pairs pass with the same counts and hashes. No local main/tool patch was used; only the explicitly listed local import map enrollments are needed. No exact acceptance is claimed.

Fresh canonical object SHA256: `0e67a84e6d03112ce030a41b2e434396d06defa168146aa191e202032593ccbc`.

## Recreation-family extension

The adjacent large routine fn_002E4E18 now supplies an independently observed set copy-construction/assignment path. The private header is refined to express those operations; the creation body only receives corresponding descriptive field renames. Its complete linked output remains unchanged and all 512 creation replay pairs pass. See effect-context-recreation.md for the separate 1,564/1,572-byte proposal and 512 bounded recreation pairs. Source-level aggregate spellings remain hypotheses constrained by the observed offsets/copies, not recovered original class definitions.
