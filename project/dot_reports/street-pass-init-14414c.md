# StreetPassObj initializer, 0014414C

Frozen base: `54d39734c4edffeeac012157ab71fb11c40d815b`. Branch: `dot/street-pass-init`. Final C++ source commit: `69c5c663a0197e7f091fea83c6d63437da642819`.

The complete 2,544-byte root at `0014414C..00144B3C` is a **NonMatching source proposal** with **zero accepted exact bytes**. The unchanged project checker accepts its import/helper closure, then rejects its 2,640-byte complete compiler section on extent. There is no canonical differing-byte count because comparison stops at the extent check. The root remains U in the restored map; main owns acceptance.

## Role and independent ABI evidence

The root initializes StreetPassObj. Its own archive path `ObjectData/StreetPassObj`, `ObjectList` BYAML, `Type`/`Reward` fields, four `StreetPassObj1..4` placement selectors and special `KuriboTower`, `KuriboTailSearch` and `TentenGenerator` construction branches identify its role. It belongs in the configured Game MapObj module, which uses ARMCC 4.1 build791. This is module/build-policy evidence, not independent proof of this root's original compiler. No alternate compiler was tried in this pass.

Independent constructor `00144DFC` calls the accepted MapObjActor constructor, loads `003C851C` from its literal at `00144E38`, and installs it as the primary table. The only aligned word in the verified executable equal to the root address is at `003C8520`, the initialization slot at table+4. Constructor writes also establish selected index+68=-1, enabled byte+6C=0 and indicator pointer+70=0. Sibling table entries at `003C8528` and `003C8530` identify `00144B60` and `00144B3C`; both operate the indicator at+70 before calling base lifecycle paths. The initializer observes the table pool at+60 and reward array at+64. These are private observed prefixes, not a complete public class definition.

Independent KuriboTower constructor `0012F204` clears+60/+64 and initializes+68=3, with a total caller allocation of0xA4. Getter `00326A04` follows+64, reads count-minus-one at its+28, and indexes child pointers. This exposed a first-draft offset error in replay; the fourth source form corrects parts/count to+64/+68 and asserts both offsets. No other source family or header was edited.

Callback data `003BF430..003BF440` is already an existing dc row. Its two ARM member-function-pointer pairs are `(0x0C,1)` and `(0x14,1)`. Existing dc row `003D5B9C..003D5BAC` points to call operator `0039BC2C` and clone `0039BBEC`. The call operator loads owner+4 and the two member-pointer words+8/+C, tests the low virtual flag, applies the encoded this adjustment, then dispatches through the object's table. The clone copies exactly this 16-byte representation and reinstalls the same table. This independently supports the private C++ Method/Callback representation; the source imports the existing data rows by address names and adds no data definitions, guessed identities or row boundaries. SafeString dispatch uses the already-named table003D9C34+8.

The source contains only this one new private translation unit. It does not touch PlayerActor, FileHandle, NoteObj, shared headers, map boundaries/types, ledger, STATE, configuration, flags, oracle tools or binaries. All source helpers disappear; the final object defines only `fn_0014414C`. Direct imports use existing named or generic function/data rows. `NON_MATCHING` guards the body. Temporary map enrollment sets only this existing U row's blank Symbol to `fn_0014414C` for the checker and restores the exact original file in a finally block.

## Four source forms and measured tools

| Form | Committed source | Complete object section | Unchanged checker result |
|---|---|---:|---|
|1|50c7eb3|2648|Source closure rejected: unresolved source helper referenced by non-branch relocation|
|2|9287b3c|2656|Same closure rejection|
|3|e780811|2640|Complete-section extent mismatch|
|4|69c5c66|2640|Complete-section extent mismatch|

Form1 used aggregate reconstruction; form2 corrects free-list object lifetime through placement construction. The remaining non-branch dependency was a compiler-emitted callback-initializer constant table. Form3 replaces that aggregate with the independently evidenced member-function-pointer constructor, eliminating that dependency, and gives array append its explicit store-then-count-update semantics. Form4 fixes the real Tower ABI error found by full-root replay. No fifth form, flag/pragmas, forced padding, artificial register/volatile tuning or fabricated helper address was attempted. Forms1/2 sizes are ELF diagnostics only and are not canonical byte comparisons. All four C++ forms were committed before the normal project build and unchanged check.py invocation. Later repeated probes use exactly form4 and are validation, not new source forms. Measured checker times for forms1..4 were 1.399668, 1.703977, 0.648940 and 0.808943 seconds. The extracted committed-note recipe was executed again unchanged after the clean build; its probe repeats the same extent rejection and its full replay/preservation contracts pass.

Initial reading/classification began2026-10-02T02:46:21Z. Source forms were committed approximately02:51,02:53,02:55 and02:59UTC. Final source clean build and replay/preservation ran during03:01..03:06UTC, with report packaging afterward. The host displays local time at UTC-04:00; UTC and Unix time are consistent with the session clock (verified with date -u and date). Elapsed timings below use monotonic or elapsed timers; listed event times are UTC. This pass has zero accepted bytes, hence zero accepted bytes/hour. Intake time remains outstanding and no sustained throughput is inferred.

## Bounded whole-root replay

The executed recipe in [street-pass-init-14414c-replay.md](street-pass-init-14414c-replay.md) passes310 paired full-root fixtures:277 normal returns,32 original-equivalent memory faults and one explicitly nonreturning callback model stopped at a150,000-instruction budget per side. The final run executes 3,705,857 instructions in 14.194186 seconds. It visits586/587 executable instruction addresses in the original root; only001449A8 remains unvisited. Coverage does not imply all paths or inputs. The nonreturning fixture establishes caller behavior under that explicit callback model, not a claim about an original runtime hang.

Twenty-five of the44 direct function imports execute their original code unchanged, including BYAML parsing and key/index conversion, placement-link enumeration and init-info construction, actual linked and ordinary actor-initialization wrappers, the pointer-array setter/allocation wrapper, string equality, actor child getters, gravity/translation accessors and vector-to-quaternion mathematics. Their descendants also execute original code until an explicitly modeled boundary. Nineteen direct imports are models; the complete table below marks them. Six additional modeled virtual endpoint kinds provide actor init/appear/dead, pose gravity/quaternion setter and a factory creator. Deep constructors initialize explicitly supplied pose/part state for later root accesses; they are fixture state, not recovered actor implementations. The resource model supplies generated serialized BYAML, not an original archive. Allocation models use bounded separate allocations and deterministic failure indices. The allocator's unused nothrow argument is deliberately excluded from semantic events.

Each pair compares the whole1MiB object/heap/fixture region,448KiB of original global range003E0000..00450000, the exact direct-import call sequence, modeled events, allocation sizes, and FPSCR exception/control bits masked by0x07C0009F. Normal returns additionally preserve SP,r4..r11 andd8..d15 canaries. Memory writes outside the compared heap region or the excluded256KiB stack range fail the harness. Fault pairs compare access kind/address/size, observed memory, events and floating state up to the fault; callee-save postconditions are asserted only on normal returns. The void root's caller-saved residue and private stack are outside the output contract. Both machines use unchanged original code at every original address; the generated candidate is separately linked at00500000 with only independently established imports.

The310 fixtures cover empty and null BYAML data, unsupported placement names, absent/wrong-typed Type/Reward fields, empty types, absent creators, all four placement columns, two table rows and signed selected-index boundaries(-128,-1,0,1,2,127), linked generic and Tenten construction, all three special paths, reward/enable/registration states, reward-capacity saturation, zero and bounded child counts, null Tower parts, individual allocation failures1..9 for each special/generic path,100 deterministic randomized bounded combinations, and42 floating cases. Floating inputs include signed zero, small subnormals, finite values, infinity and a quiet NaN under default,FZ,DN and the three nondefault rounding modes. No floating traps, arbitrary aliasing, concurrent mutation, unbounded or adversarial sizes, complete resource/actor startup, actual scene state, engine integration or gameplay equivalence is claimed.

The first replay exposed and preserved the form3 Tower failure. A harness bookkeeping bug initially counted normal returns as timeouts because Unicorn stops at its until address before invoking the hook; it was corrected to inspect PC and all normal-return ABI checks were then executed. A subsequent trace comparison for the synthetic nonreturning endpoint was corrected to record its one boundary entry, rather than count self-loop hook visits as repeated calls. These corrections changed no source or original instructions. Replaying the actual linked/ordinary init wrappers instead of their earlier broad models increased measured fault cases from31 to32; final evidence uses the narrower19-model boundary set.

## Clean build and prior-root preservation

The final clean normal project `make.py eu -ca` returns0, compiles46 Game/131 al/1 SDK sources, links and exports in39.990827 seconds. The map is unchanged; this U root is compiled into the archive but not enrolled into the compact scaffold image. Its independent diagnostic link resolves all44 function imports and3 data imports and contains only the compiler-generated root. No object was copied from another checkout.

The coordinator's pristine54d canonical baseline clean-builds and checks731 prior roots/all751 actual canonical definitions with zero failures in584.717486seconds. Baseline report SHA256 is114120a460faf7dcbca722a5afffc48904e81c052a1598788ad627eaccbf9b82. This lane exhaustively compares all177 prior C++ objects' allocated sections, relocation identities and complete function-definition tables against that verified baseline, plus369 unchanged source/config/header inputs, compiler hashes and normalized compile commands. All pass in 24.676976 seconds. Raw whole-object hashes differ because checkout-specific nonallocated records differ; allocated objects agree. Every751 baseline definition is accounted for. The only extra source/object defines this root. Generated scaffold stubs are outside this C++ equivalence check. This is equivalence-backed preservation, **not a new751-check run** and not a linked-image equality assertion. Main's independent intake remains authoritative.

## Frozen hashes

- `Game/backup/src/MapObj/StreetPassObjInit14414C.cpp`: `4ca80c1d8a123d2ae6beb31eeedb3ed80d593ee4c1667a89c9afc28c4bf10c1b`
- `build/eu/obj/Game/backup/src/MapObj/StreetPassObjInit14414C.o`: `9d12437bb33536a749463cf807bcdcba05171317fdd46e8e7af256a9156ef8e7`
- `tools/check.py`: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- `tools/low/checkExactBytes.py`: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`
- `tools/low/buildProvenance.py`: `343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529`
- `data/ver/eu/map.csv`: `434ad589b05ee299e967025dc3bc9ace954e547db7f83911fba2dc72daf8eb90`
- `data/ver/eu/code.bin`: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- `replay result memory digest`: `2bcc011b853922b2c7028d7890565cb45d307141a92080c57d824ab44e285fbd`

## Complete direct import closure

All entries below resolve to pre-existing unchanged map rows. Modeled/original denotes the final replay boundary; it is not an implementation or exactness claim for those functions.

|Address|Existing import|Replay|
|---|---|---|
|0027D588|`_ZN2al10getGravityEPKNS_9LiveActorE`|original code|
|0026902C|`_ZN2al12ActorFactoryC1Ev`|modeled boundary|
|00267050|`_ZN2al12isObjectNameERKNS_9ByamlIterEPKc`|original code|
|00276C3C|`_ZN2al13ActorInitInfoC1Ev`|original code|
|00292308|`_ZN2al13isEqualStringEPKcS1_`|original code|
|0027AB64|`_ZN2al16calcLinkChildNumERKNS_13ActorInitInfoE`|original code|
|00276C08|`_ZN2al17initActorInitInfoEPNS_13ActorInitInfoEPKNS_9ByamlIterERKS0_`|original code|
|00267088|`_ZN2al19getLinksInfoByIndexEPNS_9ByamlIterERKNS_13ActorInitInfoEi`|original code|
|00243260|`_ZN2al20findOrCreateResourceERKN4sead14SafeStringBaseIcEE`|modeled boundary|
|0027AAEC|`_ZN2al23getLinksActorObjectNameERKNS_13ActorInitInfoEi`|original code|
|002801E0|`_ZN2al24initActorWithArchiveNameEPNS_9LiveActorERKNS_13ActorInitInfoERKN4sead14SafeStringBaseIcEEPKc`|modeled boundary|
|00277D94|`_ZN2al32initCreateActorWithPlacementInfoEPNS_9LiveActorERKNS_13ActorInitInfoE`|original code|
|0028028C|`_ZN2al8getTransEPKNS_9LiveActorE`|original code|
|002905F0|`_ZN2al9ByamlIterC1EPKh`|original code|
|002910E8|`_ZN2al9ByamlIterC1Ev`|original code|
|0027A81C|`_ZN4sead12PtrArrayImpl9setBufferEiPv`|original code|
|0027CB64|`_ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_`|original code|
|00268EB0|`_ZNK2al12ActorFactory10getCreatorEPKc`|modeled boundary|
|00290640|`_ZNK2al8Resource7getBymlERKN4sead14SafeStringBaseIcEE`|modeled boundary|
|0027E068|`_ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc`|original code|
|0029107C|`_ZNK2al9ByamlIter17tryGetIterByIndexEPS0_i`|original code|
|0029101C|`_ZNK2al9ByamlIter17tryGetStringByKeyEPPKcS2_`|original code|
|002910F8|`_ZNK2al9ByamlIter7getSizeEv`|original code|
|003D9C34|`_ZTVN4sead14SafeStringBaseIcEE`|data input|
|002933D0|`_ZnajPN4sead4HeapEi`|modeled boundary|
|002932B0|`_ZnwjRKSt9nothrow_t`|modeled boundary|
|003BF430|`dat_003BF430`|data input|
|003D5B9C|`dat_003D5B9C`|data input|
|0011C224|`fn_0011C224`|modeled boundary|
|0012F204|`fn_0012F204`|modeled boundary|
|00166F3C|`fn_00166F3C`|original code|
|0016742C|`fn_0016742C`|original code|
|0016743C|`fn_0016743C`|modeled boundary|
|0016F8A4|`fn_0016F8A4`|modeled boundary|
|001B88B8|`fn_001B88B8`|modeled boundary|
|0026AC60|`fn_0026AC60`|original code|
|0026C290|`fn_0026C290`|modeled boundary|
|0026CCD0|`fn_0026CCD0`|modeled boundary|
|002702EC|`fn_002702EC`|original code|
|00271028|`fn_00271028`|original code|
|00277E5C|`fn_00277E5C`|modeled boundary|
|0027A9D4|`fn_0027A9D4`|original code|
|0027B51C|`fn_0027B51C`|modeled boundary|
|00280474|`fn_00280474`|modeled boundary|
|00326A04|`fn_00326A04`|original code|
|00327E90|`fn_00327E90`|modeled boundary|
|0032AEFC|`fn_0032AEFC`|modeled boundary|

The packet for a future distinct pass is [0014414C](../pro_requests/0014414C.md). The complete original code and pools are in [street-pass-init-14414c-disassembly.md](street-pass-init-14414c-disassembly.md). The executable recipe includes source/provenance checks, diagnostic link, full replay and the baseline-equivalence check. The patch contains only this source and notes and must apply unchanged to54d; main must still run its canonical intake before any accepted credit.
