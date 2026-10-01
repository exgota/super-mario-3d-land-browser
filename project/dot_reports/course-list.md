# CourseList initialization and World construction

## Scope and status

Targets are `CourseList::init(const al::Resource*)` at the unchanged interval `0x0016A11C–0x0016A2B4` (408 bytes) and `CourseList::World::World(const al::ByamlIter*)` at `0x00325B44–0x00325E00` (700 bytes). This work started from main `a360142fbddd9ab875ab68314c55445ade05fd00`. The supplied EU executable hashes to `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

The retained C++ already expresses the retail traversal, class layouts and string/type decisions. This checkpoint reconstructs its formerly anonymous string pool as complete source data, and separates the stage-type leaf into its own translation unit. No strict match is claimed before canonical committed-source compilation and checking. Both targets retain their existing `NON_MATCHING` guards.

## Layout and semantic evidence

The List allocation at `0x0016A12C` requests 8 bytes. Its world pointer table is at offset 0 and signed count at offset 4. The World allocation at `0x0016A20C` requests 12 bytes. Its constructor writes the course pointer table at offset 0, signed count at offset 4 and special-world flag byte at offset 8. The Course allocation at `0x00325C20` requests 20 bytes. The inline constructor initializes type 0, stage name null, scenario -1, model-name default Miniature and coin count 0, with five successive four-byte fields. All existing source layouts agree.

The World constructor requires a valid Type string before it processes Course. It compares Type with Special, then allocates and initializes the Course pointer array. Failed indexed-child lookup writes a null entry. Course type comparisons are ordered KoopaCastle=1, KoopaFortress=2, KoopaBattleShip=3, Championship=4, KinopioHousePresent=5, KinopioHouseAlbum=6, MysteryBox=7, Dokan=8 and Empty=10; unmatched values retain Normal=0. It then reads Stage, Scenario, Miniature and CollectCoinNum in that order.

The List constructor likewise looks up Worlds, allocates a pointer array and initializes each entry, preserving null entries on failed lookup. `init` counts courses with type at most Championship after construction. The existing helper at `0x0025BD18` uses an unsigned range comparison, accepting values 0 through 4 and rejecting negative enum bit patterns; the four-instruction leaf does not require a data import. The inherited signed comparison was corrected after the final readback described below.

The previously same-translation-unit `isCourseTypeStage` definition let ARMCC exploit its known register preservation despite `no_inline`, producing caller-saved loop registers unlike the retail caller. Its separate source file preserves the ordinary external call boundary without adding a guessed import or changing its implementation.

## Full string pool and address anchor

`CourseListStrings` is fully initialized ordinary C++ data, 212 bytes including the final 0xFF alignment byte, reproducing the complete observed pool from `0x003A2884` through `0x003A2957`. Its dedicated section is `.sdata_dat_003A2884`. The already existing first data row at `0x003A2884–0x003A2890` is only the address anchor. It does not claim to contain the complete aggregate; the pool spans existing adjacent string rows. No row boundary changes or generic `.constdata` mapping are requested.

| Field | Address | Source extent |
| --- | --- | --- |
| KoopaCastle | 0x003A2884 | 12 |
| Type | 0x003A2890 | 8 |
| Normal | 0x003A2898 | 8 |
| Miniature | 0x003A28A0 | 12 |
| KoopaFortress | 0x003A28AC | 16 |
| KoopaBattleShip | 0x003A28BC | 16 |
| Championship | 0x003A28CC | 16 |
| KinopioHousePresent | 0x003A28DC | 20 |
| KinopioHouseAlbum | 0x003A28F0 | 20 |
| MysteryBox | 0x003A2904 | 12 |
| Dokan | 0x003A2910 | 8 |
| Empty | 0x003A2918 | 8 |
| CollectCoinNum | 0x003A2920 | 16 |
| Scenario | 0x003A2930 | 12 |
| Stage | 0x003A293C | 8 |
| Worlds | 0x003A2944 | 8 |
| CourseList | 0x003A294C | 11 |
| alignment byte | 0x003A2957 | 1, value 0xFF |

All array-tail padding before the final byte is zero. The first checkpoint's independently constructed 211-byte semantic string representation equaled all 211 corresponding retail bytes; the source includes the observed final 0xFF byte as well. Compile-time size and selected offset assertions check the aggregate. A compiled-data comparison will follow the canonical build. All strings in these CourseList files are ASCII, so no CP932 trail-byte escape hazard applies. The existing files retain their original byte-compatible encoding.

The original World constructor loads Miniature at `0x00325BE8`, then obtains Normal by subtracting 8 at `0x00325BF0`. These fields share one source aggregate so their observed relationship remains available to the optimizer. World's own World, Type, Special and Course strings remain local literals in its own function interval.

## Imports

All call identities already have named existing rows:

- `0x00290640`: Resource::getByml, independently established by the scene-resource-heap report and its separate ActorFactory caller.
- `0x002905F0`: ByamlIter binary-data constructor.
- `0x002910E8`: default ByamlIter constructor.
- `0x00290FB0`: tryGetIterByKey.
- `0x0029107C`: tryGetIterByIndex.
- `0x002910F8`: getSize.
- `0x0029101C`: tryGetStringByKey, independently established by its implementation in the scene-resource-heap report.
- `0x0027E068`: tryGetIntByKey.
- `0x00292308`: char-pointer isEqualString.
- `0x002932B0` and `0x00292A78`: scalar and array nothrow allocation, established independently in project/decisions.md from NerveKeeper, NerveStateCtrl and GhostPlayerRecorder.
- `0x0025BD18`: existing Course::isCourseTypeStage leaf.
- `0x00325B44`: existing World constructor.
- `0x003D9C34`, normal ABI address point +8: existing SafeString virtual table.

No address is invented for the source-inlined Course or List constructors.

## Canonical checker record

Pending the first committed-source build/check checkpoint. Baseline canonical World output was 692 bytes, with the target's body reproduced apart from two extra retail `mov r1,r4` instructions before allocations and unresolved anonymous string relocations. Baseline init was 404 bytes with different loop-register allocation and control-flow scheduling. These observations are diagnostic only, not matches.

### Checkpoint 1 result

The parent committed checkpoint 1 as `a69ad8d` and ran the canonical build. Both strict checks reported `U -> M` because the complete section sizes differ: init is 404 bytes, World 644 bytes. The compiled 212-byte `.sdata_dat_003A2884` pool equals all corresponding retail bytes; both copies have SHA-256 `6977bc957a7bbf3f85a4155ad379869b6f43cd0f32eff93ed509568a06b233e5`. This data comparison is independent of code acceptance.

The single aggregate lets ARMCC retain one base address and derive most strings with additions, unlike the retail body. Diagnostic source probes therefore split the pool into fully initialized string arrays at their existing row anchors, keeping Type, Normal and Miniature together in one 28-byte section at `0x003A2890`. That section spans the existing Type/Normal row and Miniature row without changing either boundary. Each other string has its own address-encoded section; the final CourseList name uses a 12-byte struct containing its 11-byte terminated name and the observed 0xFF pad. All source data remains complete. This supersedes the single-aggregate representation described above for the next checkpoint.

The split arrays restore World's 692-byte baseline register allocation and its exact Normal = Miniature - 8 relationship. The independently inspected split compiled data equals the corresponding retail bytes. A diagnostic inclusion of the approved ARMCC `<new>` header had no effect on allocation arguments and is not retained. No artificial nothrow tag, heap argument, pointer cast or instruction-shaped expression was added.

Checkpoint 2 moves the List constructor into an ordinary separate translation unit. The retail init body shows the same sort of late-inlining differences previously recovered elsewhere in this project. This tests the existing canonical source-closure mechanism without assigning the List constructor an original address. Its complete definition remains source code; the checker must reject it if residual helper code remains.

The diagnostics used the same recorded compiler command with only source, output and dependency paths redirected to `/tmp`; ARMCC41INC was set to the approved compiler's include directory as in the normal build. An initial missing ARMCC41INC caused header-not-found errors before compilation. None of these scratch outputs counts as a match or replaces a canonical object.

### Checkpoint 2 result and final source form

The parent committed checkpoint 2 as `ae5aa23` and built it canonically. World's strict check reported `M -> M` for the 692-versus-700-byte section-size difference. The init source closure was rejected with `Residual helper code or another allocated extent remains after inlining.` Its compiled root is 240 bytes and the separately compiled List constructor is 208 bytes. The linker did not produce the original single 408-byte extent. The separate-constructor experiment is therefore not retained: List returns to the same translation unit in the final source form.

All fifteen individual compiled string sections from checkpoint 2 independently matched their corresponding original intervals byte for byte. Together they cover every byte of `0x003A2884–0x003A2958` without gaps or overlaps and retain the aggregate SHA-256 `6977bc957a7bbf3f85a4155ad379869b6f43cd0f32eff93ed509568a06b233e5`. Their sizes in address order are 12, 28, 16, 16, 16, 20, 20, 12, 8, 8, 16, 12, 8, 8 and 12 bytes. The 28-byte Type/Normal/Miniature section is the sole section spanning two original data rows; its first row is an address anchor only. The remaining sections exactly fit their original data rows. All meaningful string contents and observed padding are reconstructed, rather than merely importing an undersized placeholder.

The large functions remain correct semantic reconstructions and NonMatching proposals. World's residual code-generation issue is exactly the two retail allocator-argument moves plus the resulting branch/literal displacements. Both runtime allocators ignore their tag argument, but no unsupported explicit tag or fabricated argument was added merely to force those moves. init retains additional allocation/null-path, local-lifetime and loop-control scheduling differences. The same-translation-unit allocator header experiment did not resolve these differences, and the tested separate-constructor late-inline hypothesis failed the strict closure guard.


### Final canonical result

The parent committed the final large-function/source-data checkpoint as `7b00157` and ran the canonical ARMCC build and strict checks:

- `_ZN10CourseListC1Ev`, `0x0016A2B4–0x0016A31C`, 104 bytes: `U -> O: The complete source-generated function interval matches byte for byte.` This is the single exact function established in this family at this checkpoint.
- `_ZN10CourseList4initEPKN2al8ResourceE`, 404 compiled bytes versus 408 original bytes: `M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.`
- `_ZN10CourseList5WorldC1EPKN2al9ByamlIterE`, 692 compiled bytes versus 700 original bytes: the same `M -> M` size-mismatch result.
- `_ZN10CourseList6Course17isCourseTypeStageENS0_10CourseTypeE`, 16 bytes: `m -> m: The linked candidate differs from the unchanged original interval.` It is not an exact match.

The final canonical CourseList.o again passed a complete read-only comparison of all fifteen string-data sections: 212 bytes, contiguous from `0x003A2884` to `0x003A2958`, aggregate SHA-256 `6977bc957a7bbf3f85a4155ad379869b6f43cd0f32eff93ed509568a06b233e5`. Its source provenance records CourseList.cpp SHA-256 `07b7eb7c46aaaa9a6b4cf0ba31bd3656d781ccbe319a90794cc38a3003fda0b6`.

The leaf mismatch prompted a final retail readback. Its actual instructions use unsigned HI/LS after comparison with 4. The inherited C++ used signed LE/GT, incorrectly treating negative enum bit patterns as stages. The final source changes only that semantic comparison to an unsigned cast and restores the leaf's `NON_MATCHING` guard. This correction is independent of the exact CourseList constructor, whose canonical input files and behavior are unchanged. No additional leaf match is claimed unless a subsequent parent check establishes it. The separate leaf source remains necessary to retain the retail external-call optimization boundary in init.

No further code-generation experiments are proposed for the two large functions in this pass. All constructor/traversal logic, class extents, course types, string contents, data padding and external call identities now have explicit retail evidence; their remaining mismatches require a new compiler or source-lifetime hypothesis. No tools, map intervals, compiler flags, binaries, maps or project ledgers were edited by this lane.

## Latest-main verification (2026-10-01)

Canonical checker/build base: `a5041a5091efbf53fdbf99c6950133f2ec41e998`. Source was committed locally as `1a1dcfe` before `python make.py eu`. No checker changes were made. The full compact build linked and exported successfully. The final unsigned leaf now matches; this supersedes its earlier mismatch above.

Commands (from the repository, with the approved toolchain environment):
```sh
python make.py eu
.venv/bin/python tools/check.py _ZN10CourseListC1Ev --object build/eu/obj/Game/backup/src/System/CourseList.o
.venv/bin/python tools/check.py _ZN10CourseList6Course17isCourseTypeStageENS0_10CourseTypeE --object build/eu/obj/Game/backup/src/System/CourseListCourse.o
.venv/bin/python tools/check.py _ZN10CourseList4initEPKN2al8ResourceE --object build/eu/obj/Game/backup/src/System/CourseList.o
.venv/bin/python tools/check.py _ZN10CourseList5WorldC1EPKN2al9ByamlIterE --object build/eu/obj/Game/backup/src/System/CourseList.o
```
Both first checks: `U -> O: The complete source-generated function interval matches byte for byte.` Exact subtotal: 104 + 16 = 120 bytes. Both last checks: `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` Those 408/700-byte targets are not accepted and stay guarded.

Local-only map changes name existing imports listed above, including Resource::getByml and ByamlIter::tryGetStringByKey. No row boundaries changed. Main must adopt/revalidate these independently established identities before importing the proposal. No map, ledger, STATE, tools, or binaries are included. `tools/diff.py` was unavailable because its dependency setup was not permitted; source diagnostics do not replace canonical `check.py` acceptance. See project/dot_reports/course-list-init-packet-followup.md and project/pro_requests/00325B44.md for unresolved source/codegen questions. Main packet 0016A11C at a0592515 is preserved unchanged; our followup partially answers it but does not resolve init. Main a0592515 changes notes only, so the a504 checker remains current.
