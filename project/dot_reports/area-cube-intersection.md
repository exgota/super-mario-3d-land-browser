# AreaShapeCube ordered segment intersection

Frozen integration base: `754f99a30a337756df5aa01c28b4ed6a977fecd5`.
Branch: `dot/area-cube-intersection`. Final C++ commit: `f86d4c4`.
Target: unnamed whole function `0x0033091C..0x003311D8`, 2,236 bytes.
The proposal adds one isolated guarded `NON_MATCHING` TU. It changes no shared header, accepted Cube/base method, virtual table, map, rank, ledger, STATE, project tool, compiler setting, or binary. No publication was performed by this lane.

## Result and scope

Zero exact roots and zero accepted bytes. All three meaningful forms compiled to 2,232 bytes, and the unchanged checker rejected the size against the complete 2,236-byte interval. This is a real semantic reconstruction, not an exactness claim. The final source preserves form 1 with a normal `NON_MATCHING` guard; adding the guard reproduced the exact form-1 object SHA-256. No fourth form was needed, and no register/volatile/padding tuning, inline assembly, opcode replacement, hand-edited object, fabricated callee implementation, global-flag change or alternative compiler installation was used.

The final whole-root ARM1176 replay passes 1,967/1,967 comparisons in 34.372856 seconds. All 553 original executable instruction addresses were reached; this includes the substantial tail after the six-word internal pool at `0x00330CF4..0x00330D0C`. It executes all original callees and installs only the unmodified linked candidate root for the candidate run. There are zero modeled callees. This is bounded differential evidence, not a proof for every possible input or a native-port/gameplay result.

The normal-map clean build passed in 39.415257 seconds. Since the map leaves this root unnamed and U, the normal linked scaffold does not establish execution of this source root. The separate diagnostic link explicitly links the actual project-built C++ object and all six independently grounded imports at their original addresses, without modifying the object or checker. Its section is 2,232 bytes. Its 1,771 unequal bytes in the overlapping prefix are diagnostic only; the four-byte size deficit remains a strict-check failure.

## Binary-grounded interface and behavior

The retail constructor `0x001C50EC` calls the AreaShape base constructor, stores the byte flag at offset `0x14`, and installs address point `0x003D62C0`. The slot at `0x003D62C8` points to this root. The already accepted `isInVolume` method reads the same flag. Independent table evidence is in `project/area_shape_cube_data_evidence.md`; its old “not applied” wording is stale at this frozen base. Main already has the complete 20-byte Cube row `0x003D62B8..0x003D62CC` plus the separate successor header. This proposal leaves both intact.

The root preserves incoming r0 as shape, r1 as output and r3 as the second position; incoming r2 is passed as the first position to the established `AreaShape::calcLocalPos` helper. Both local vectors start as the named zero vector at `0x004305F8`, and both helper results are ignored. The first Cube flag read chooses Y bounds `[0,1000]` only for byte value 1, otherwise `[-500,500]`. A second read after the two transforms establishes initial containment. X and Z use `[-500,500]`.

The root forms end minus start, then tries faces in Y, Z, X order. When the start lies inside, it tries the exit face selected by each direction sign; otherwise it tries the entry face. Each candidate divides the face distance by the corresponding end-minus-start component, accepts t in `[0,1]`, scales the full direction, adds start, checks the two remaining face coordinates, transforms the hit back, and returns true. It returns false after the ordered candidates fail. It does not search all faces and choose a minimum parameter.

The neutral source signature is `bool fn_0033091C(const al::AreaShapeCube*, sead::Vector3f*, const sead::Vector3f&, const sead::Vector3f&)`. No semantic method name, C++ virtual override name, or broader class reconstruction is claimed. Access through `const u8*` at byte 20 uses the independently observed representation without altering the shared class declaration.

The existing named retail arithmetic imports are subtraction `0x0027CB64`, addition `0x0027CB48`, and scalar multiply `0x0027CC64`. Their independent bodies load three contiguous floats, apply the named arithmetic, store three floats, and return. The source’s small private by-value arithmetic wrappers contain only these real imported calls. They have no residual compiled helper definitions. The C-linkage declarations retain the existing mangled symbol spelling and observe the pointer ABI; they do not add a speculative SDK class definition.

The remaining unnamed import `0x00337488..0x00337520` independently checks the three shape scales through `0x0026D698`, multiplies the local position componentwise by the scale, and optionally transforms by the shape’s matrix. It returns 0 on rejected scale, 1 otherwise. Its boolean result is ignored here, exactly as in the root. Its neutral name `fn_00337488` has no unrelated provider or alias bridge.

## Attempts and exact output

All source forms were committed before `python make.py eu`, followed by unchanged `python tools/check.py fn_0033091C --object build/eu/obj/lib/al/src/AreaObj/alAreaShapeCubeIntersection.o`.

| Form | Commit | Meaningful representation | Full section | Result |
| --- | --- | --- | ---: | --- |
| 1 | `9e71ab0` | Two scalar Y bounds and rejection-form initial containment | 2232 | Strict size rejection |
| 2 | `5669f48` | Inclusive initial-containment conjunction | 2232 | Identical emitted object; strict size rejection |
| 3 | `e321e62` | Two-component Y-bound intervals | 2232 | Strict size rejection; diagnostic overlap 1792 differences |
| Final | `f86d4c4` | Form 1 restored, guarded as NonMatching | 2232 | Same exact object as form 1; strict size rejection |

Exact final checker output, with terminal colors removed:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The temporary checker map named only the existing whole rows `0x0033091C` as `fn_0033091C` and `0x00337488` as `fn_00337488`. No Type, start, pool, end, or data identity was changed. The local rank change made by check.py was discarded by restoring the original map bytes in a finally block. The final map SHA-256 is `b70e4f37358ebc14aa0c0d522c83172901f23998510a9a2a4e1a36d6cd811881`. Integration of those neutral row spellings remains the coordinator’s separate metadata decision; the source remains guarded and uncredited.

## Preservation and replay limits

Exhaustive comparison against pristine `mario-main754` verifies all 178 old canonical C++ objects, including the eight objects with no accepted-definition entries. Every allocated section’s bytes, type, size, flags and alignment; every relocation and its target; every symbol after checkout-path normalization; every old provenance input, compiler identity and normalized command all agree. All 370 old input files are identical and committed. Raw object file hashes differ because of checkout-path metadata; no raw-object equality is claimed.

The baseline report SHA-256 is `c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028`. It records 733 roots/all 753 actual definitions passing, clean-build return 0, and 582.169391 seconds. This lane’s preservation comparison took 23.714124 seconds. This is an exhaustive equivalence proof to that separately checked baseline, explicitly not a new 753-check run.

The final replay includes 240 signed face/flag/output-alias fixtures, 24 boundary and degenerate fixtures, 1,200 deterministic random inputs with rotations/translations/signed scales, 270 float/scale edge fixtures under three FPSCR modes, 12 rejected-scale cases, 72 raw NaN-bit cases including signaling NaNs, 144 matrix float-edge cases, and five alias/fault controls. It compares boolean results, all 4 KiB of caller-visible fixture memory, the local/world-transform argument traces, cumulative FP exception bits, preserved r4-r11/d8-d15, and restored SP. Four controls fault equally, with matching access kind/address/size and caller memory; these are matching-fault controls, not successful returns. The normal-return count is 1,963: 517 true and 1,446 false.

All ten reached helper entries execute unchanged retail instructions: `0x001D7978`, `0x00216D9C`, `0x0026D698`, `0x0027CB48`, `0x0027CB64`, `0x0027CB80`, `0x0027CB9C`, `0x0027CC48`, `0x0027CC64`, `0x00337488`. The only compiled candidate entry is `fn_0033091C`. Original/candidate instruction totals are 708,040/705,899, maximum 515/514; maximum stack use is 160 bytes each. These are instruction counts, not measured hardware cycle counts. Both use a 10,000-instruction watchdog; neither times out. This root and these callees do not traverse cyclic object structures. Arbitrary malformed pointers, all FP bit combinations, concurrent mutation, hardware timing and whole-game behavior are outside the result. The fixtures exercise float overflow, infinities, signed zero, subnormals, NaNs, matrix aliases and null/unmapped read/write faults; no integer-overflow claim is needed for this fixed, acyclic arithmetic root.

## Timing, hashes and reproduction

Work began 2026-10-02 03:42:54 UTC. Form 1 was committed at approximately 03:46:13 UTC; form 2 at 03:50:32; form 3 at 03:51:51; final guarded source at 03:53:39. All original-callee replay and preservation evidence was complete by 03:57:22 UTC. These wall times use the session clock; physical command durations above use monotonic clocks. The three forms plus final guard/check are four physical source-build/check snapshots but only three distinct algorithm/source-form attempts. No exact accepted-throughput numerator is added.

The final C++ SHA-256 is `a29ace477716fcb951ab3f9f3322bac5e71fed9379173d40998b1c8a4d59b500`; final object SHA-256 is `3c8eadec19515505cc721c9219a653bd5a8724943ed6641d44463fe245dd9a4a`. The unchanged EU dump SHA-256 was checked before use as `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. No binary was committed or uploaded.

```
check.py           e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317
checkExactBytes.py  aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2
buildProvenance.py 343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529
stepBuild.py       b7be5977a0a1afc339eb4c9087b447cdc25b1c42daa3fe23af4b765008b0aac0
config.json        5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45
ARMCC 4.1/791      d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d
armlink 4.1/791    b9cffb71d7b58f3fd33fa8c4c8af884b75689bb5a9e9014309a08146bba676dc
wibo              aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b
```

ARMCC 791 is the only compiler used for this root; installed 902 was not an alternate-form test, and absent 894 was not installed. The final object provenance records the unchanged project flags, including `-O3 -Otime --fpmode=fast --cpu=MPCore --forceinline` and `NON_MATCHING=1`.

The companion note `area-cube-intersection-replay.md` contains the exact read-only verification recipes. After the committed-source normal clean build, run `check_form.py final`, `link_diagnostic.py final`, `replay.py final`, and `preserve.py /path/to/pristine/mario-main754` from the checkout. The strict checker still rejects; the unrestricted diagnostic source link is used only for behavior. Local sealed evidence is under `build/cube-evidence/`, including every form result/check/build log, final replay, preservation details and hashes. `project/pro_requests/0033091C.md` retains the complete target disassembly with the internal pool and tail; no target boundary is shortened.
