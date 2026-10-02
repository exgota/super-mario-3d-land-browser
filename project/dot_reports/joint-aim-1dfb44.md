# Joint aim root 001DFB44: complete NonMatching reconstruction

Base: `754f99a30a337756df5aa01c28b4ed6a977fecd5`. Branch: `dot/root-1dfb44`. Source commit: `379883a33c831ca48c328af1098a81c60f1f0131`.

The complete `001DFB44..001E04DC` root is reconstructed as ordinary C++ in [alJointAim1DFB44.cpp](../../lib/al/src/Model/alJointAim1DFB44.cpp), with a dedicated observed-layout [header](../../lib/al/include/Model/alJointAim1DFB44.h). One meaningful source form was committed and built. The project checker rejects source closure; **zero exact roots and zero exact bytes are proposed**. The root remains U, and no map, rank, boundary, Type, ledger, STATE, tools or configuration changed.

The canonical rejection is a real data-identity prerequisite: the existing clean `Vector3<float>::ex` and `ey` declarations have independently established addresses but no rows in this checkpoint's map. Resolving that prerequisite would only allow normal grading; the current 2388-byte compiler output also differs from the original 2456-byte interval. No replacement constants, fake imports, shortened boundary, object patching, global flags, extra compiler installation or inline assembly were used.

## Family and ownership evidence

The root implements a joint-index lookup in a 0x88-stride ring, computes a target direction from the supplied joint transform, applies front-facing hysteresis, clamps horizontal/vertical aim angles, interpolates a retained quaternion, transforms the correction into the joint's basis, and multiplies the supplied matrix in place. The descriptive record names are hypotheses about purpose, not claims about original class names.

Before source editing, the lane read AGENTS, BRIEF in full, DOT_BRIEF, STATE, the newest daily report and Guide, and checked source, reports, blocked/packet history and active owners. No root-address implementation, packet, prior pass or family ownership was found. FileDevice/NoteObj, Draw24B168 and Controller480/456 remained untouched. Only this new header/TU family is included.

Independent allocation wrapper `0026CA54` allocates 0x24 bytes and invokes `001E04DC`. The latter zeros the target vector, saves the actor matrix at +0C, initializes the ring at +10..+1C and the two flags at +20/+21, and allocates 0x88-stride entries constructed through `001DF860`. Insertion `001DF8FC`, called by independent wrapper `0026C9CC`, fills the joint ID, degree-to-radian limits, three joint basis vectors, initial quaternion, interpolation factor, optional reference matrix and its three basis vectors before appending. Its copy reaches +84, independently confirming the 0x88 stride.

The root itself has no direct BL/B caller in the scanned executable. Descriptor `003A9850..003A9858` contains its function pointer and zero adjustment. Wrapper `0026CA54` loads that pair from its literal at `0026CB64`, stores it at callback object +8/+C, stores the controller at +4 and installs the dispatch table at `003D93AC`. This is an indirect callback identity, not an invented direct caller.

The original map Pool value `001DFFC8` marks an interior literal pool, not the end of executable code. The executable ranges are `001DFB44..001DFFC8` and `001DFFEC..001E04D8`: 604 ARM instructions. The two pools occupy 40 bytes in total; all 2456 bytes remain in scope.

## ABI and source closure

The header asserts the entry's 0x88 size, quaternion offsets +38/+48, hysteresis +58, reference matrix +60, controller 0x24 size and ring count +1C. The float vector/quaternion/matrix fields reuse the existing clean 12/16/48-byte sead records. Named math imports bind the exact native map identities, and anonymous imports retain existing address identities; no new shared math class definition or alternative helper body is introduced. The pointer argument declarations describe observed ABI records; their original named APIs use same-address references. All callers pass the established float layouts. No translation unit in this checkpoint has a conflicting declaration of the new anonymous imports.

The complete candidate object contains one strong function definition, `fn_001DFB44`, and no compiled helper functions. It imports 17 genuine original function entries and 3 data identities. The normal project build links successfully with the root U; its compact image pulls the generated stub for this unenrolled root. This does not establish root linking or root behavior. Separate diagnostic links of the actual ordinary compiler object resolve all 20 imports at their observed original addresses, including the two explicitly unenrolled data identities.

The main-owned metadata prerequisite is limited to these two existing declared data objects:

- `004305C8..004305D4`, 12 bytes, `sead::Vector3<float>::ex`, existing mangled declaration `_ZN4sead7Vector3IfE2exE`
- `004305D4..004305E0`, 12 bytes, `sead::Vector3<float>::ey`, existing mangled declaration `_ZN4sead7Vector3IfE2eyE`

Their addresses were already documented by the clean `lib/sead/README.md` and header. This lane independently executes original `__sti___14_seadVector_cpp` at `00383870` and observes all six four-byte stores to those intervals, yielding ex=(1,0,0), ey=(0,1,0). The next object ez already begins at `004305E0`, so the proposal does not alter an existing interval. Original quaternion initializer `00381350` similarly supplies the already enrolled unit quaternion. No initializer instructions or original code were patched. A main-owned metadata review is needed before any enrollment; this lane supplies no map patch.

## Unchanged checker and attempt history

First source form `379883a` was committed before the normal project build. The first unchanged project check took 1.362815328 seconds, exited 1 and printed:

```text
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

After the final clean build, the unchanged source was checked again. The repeat took 1.222660777 seconds, exited 1 and printed the same rejection. Only the existing root row's blank Symbol spelling was temporarily set to `fn_001DFB44`; the exact original map bytes were restored in a finally block. No Type, boundary, rank or dependency row was changed. The missing non-branch relocations target ex/ey. No byte-exact claim follows from this rejected check.

The independent diagnostic relink at the original root address measures 2388 candidate bytes against 2456 original bytes, 2106 differing byte positions in their overlap and 68 unpaired original bytes. That is not a canonical grade. Adding metadata alone will not make this source exact. There was no second source form or cosmetic register/padding/volatile tuning. The four-unsuccessful-form cap was not reached; further grading is blocked on genuine data closure.

## Bounded whole-root replay

The final replay passes 575 paired whole-root fixtures: 569 normal returns and 6 matching memory faults. It executes 1,418,266 ARM11 instructions in 17.997137518 seconds and visits all 604 executable instruction addresses in the original root. Address coverage is not all-path or all-input coverage. Both sides execute actual original instructions for all math callees, including transitive trigonometric, normalization and matrix/quaternion routines. There are zero modeled callee boundaries. Original vector/quaternion static initializers execute before each fixture state is instantiated.

Fixtures include nonpositive counts, misses, wrapped rings and late matches; all enabled/retain/hysteresis combinations with front/side/back/coincident targets; optional reference matrices; interpolation factors inside and outside [0,1], negative and nonunit quaternions; equal/reversed angular limits; 150 deterministic bounded random target/limit/translation cases; and 80 nonorthogonal matrix/basis cases. There are 180 signed-zero, subnormal, normal-edge, ±infinity and ±quiet-NaN cases spread across target, current quaternion and basis fields under default, FZ, DN and all three nondefault rounding modes. Three explicit alias cases share actor/joint matrices, reference/joint matrices, or the joint matrix with entry basis fields.

Each normal return checks the return value, full fixture memory, complete modeled global data region, caller-owned stack above entry SP, external call-entry sequence, FPSCR, restored SP, R4..R11 and D8..D15. Original/candidate scratch addresses and dead lower callee stack are excluded. The six faults cover null controller/matrix/buffer, negative/oversize first index and an oversized count; both sides reach the same access kind, address and size with the same externally visible memory. Faulting paths do not claim return-register or stack-restoration guarantees. No fixture reaches the instruction budget; zero bounded/nonreturning outcomes were observed, and no cycle/hang property is inferred.

The first replay-harness run correctly hit a write-protection fault while original static initializers wrote global data. The harness then separated code from its writable data region and reran the untouched source. Original instruction regions remain immutable. This is an execution-environment correction, not an algorithm change. The 312-case first semantic run and 575-case expanded run both passed before the final clean-build rerun.

Limits: the valid ring domain has bounded, nonoverflowing signed indices/counts. General signed-overflow equivalence, arbitrary aliasing, concurrent mutation, signal handlers, signaling NaNs, enabled floating traps, complete actor construction, engine integration, rendering and gameplay remain unverified. Actual original math execution establishes bounded behavior of this root, not source reconstruction of the callees or exact-byte credit.

## Clean build and preservation

Final normal `python make.py eu -ca` returned 0, compiled 45 Game / 133 al / 1 SDK C++ sources, linked and exported in 35.591816635 seconds. ARMCC 4.1/791 compiled the candidate; installed 4.0/902 remains the configured SDK compiler. Build 894 is absent and was not installed or tested.

The coordinator separately verified pristine base754 by a clean build and the unchanged canonical checker on all 733 prior roots/all 753 actual definitions, with zero failures in 582.169390522 seconds. Baseline report SHA256: `c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028`.

This lane exhaustively compares all 178 prior canonical Game/lib compiler objects, including objects without accepted-root entries, against that verified baseline. All allocated section metadata/bytes, complete function-symbol definitions and relocation symbol identities agree. All 370 prior source/header/config inputs, compiler hashes and normalized build commands agree; every prior canonical definition is accounted for. Raw whole-object hashes differ; this comparison intentionally excludes nonallocated records, including checkout-specific build metadata. The new object has only the one declared root and cannot replace a prior symbol. The comparison passed in 22.900381090 seconds.

This is **equivalence-backed preservation against the separately passed baseline**, not a repeated 753-check canonical run in this lane. The rejected new root adds zero exact credit; accepted coverage remains 733 roots / 48,576 complete bytes.

## Timing, artifacts and next step

Investigation began 2026-10-02T03:17:13Z. The sole source form was committed and first normal build started at approximately 03:24Z. First full replay passed at 03:28Z; the expanded replay passed at 03:30Z. Final clean build, repeated check, original-callee replay and exhaustive preservation completed during 03:30..03:33Z. Packaging follows those checks. This pass has zero accepted bytes, hence zero accepted-byte throughput; future intake is not included as completed work.

[The executable validation recipe](joint-aim-1dfb44-replay.md) contains every script used to build, probe, link the actual root, execute fixtures, verify preservation and seal evidence. [Frozen verification evidence](joint-aim-1dfb44-evidence.md) records exact source/checker/config/toolchain hashes and measured outputs. Ignored local build artifacts are under `build/root-1dfb44/`. No private dump, compiler, compiled object or other binary is committed.

Main can review the isolated ex/ey identity prerequisite, then grade future full-source proposals with its unchanged canonical acceptance gate. This proposal is a fully replayed NonMatching implementation with a documented closure blocker, not an acceptance-ready exact family. It does not claim ownership of any existing shared source/header family.
