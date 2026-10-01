# Placement map reconstruction

Base/checker: `a5041a5091efbf53fdbf99c6950133f2ec41e998`; committed source checkpoint `1a1dcfe`, built with the unchanged canonical `python make.py eu` and approved ARMCC 4.1 build 791. EU code SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Result

`al::tryGetPlacementInfo` at `0x00276C60–0x00276CF0` is exact: 144 bytes. `al::initPlacementMap` at `0x00274EF0–0x00275048` remains NonMatching after eight candidates: complete 344-byte extent and pool agree in size, but the linked candidate differs. Its guard remains.

```sh
python make.py eu
.venv/bin/python tools/check.py _ZN2al19tryGetPlacementInfoEPNS_9ByamlIterEPKNS_8ResourceEPKc --object build/eu/obj/lib/al/src/Scene/alSceneFunction.o
.venv/bin/python tools/check.py _ZN2al16initPlacementMapEPNS_5SceneEPKNS_8ResourceERKNS_13ActorInitInfoEPKc --object build/eu/obj/lib/al/src/Scene/alSceneFunction.o
```
Exact output: `U -> O: The complete source-generated function interval matches byte for byte.` Root output: `U -> m: The linked candidate differs from the unchanged original interval.` Full latest-main compact build linked and exported. No oracle edits.

## Recovered behavior and identities

The former lookup fragment could fall through a bool function without returning. Retail returns false for a null archive, obtains StageData, queries AllInfos, then returns the actual named-child lookup result. A scoped default ByamlIter in the successful AllInfos path is retained because its constructor call is present in retail.

The root gets placement records, looks up ObjectName, resolves a creator in the scene factory, initializes the complete ActorInitInfo, creates the actor, then applies placement initialization. Its final source uses an ordinary inline traversal helper; no residual helper body is accepted. An unsuccessful separate lookup translation unit was removed.

The recovered call at `0x00268EB0–0x00268FB8` (pool `0x00268FAC`) is `_ZNK2al12ActorFactory10getCreatorEPKc`. Independent evidence: the routine reads factory+4, obtains ObjectName/ClassName through Byaml, then scans 225 eight-byte entries of already mapped `sActorFactoryEntries` (`0x003B99F0–0x003BA0F8`) and returns a creator pointer. Separate callers at `0x00144800` and `0x0019CD00`, and factory constructor `0x0026902C` loading CreatorClassNameTable, corroborate the role. Naming this existing row is a required local-only map change. Main must adopt/revalidate it; no map is published.

The global `al::initActorInitInfo` at `0x00276C08` is already mapped. Its body copies fields at +0/+4/+8/+0x10 and derives view id at +0x14. The source calls this established function rather than the former nonexistent member method. ActorInitInfo is 24 bytes; ByamlIter is eight; Scene has factory pointer +0x1C and size 0x34; ActorFactory is eight bytes. Existing SafeString vtable ABI base `0x003D9C34` uses addend +8. Existing data rows `dat_003B7848` (StageData) and `dat_003B783C` (AllInfos) are imported without changed boundaries or generic constant aliases. Root pool words at `0x0027503C/40/44` are respectively `0x003B7848`, `0x003D9C3C`, `0x003B783C`.

## Eight-candidate record

1. `a69ad8d`: explicit false return; root 332 bytes, helper 152, both nonmatching
2. `a9fce38`: bool accumulator; same sizes/result
3. `fdee522`: return query result; root 332 nonmatching, helper 144 exact
4. `ae5aa23`: separate-source closure; root 228 + helper 152, rejected residual allocated helper extent; removed
5. `7b00157`: inline traversal factor; root 340 nonmatching, helper 144 exact
6. `0ed4609`: lookup definition moved earlier; same result
7. `b4574a1`: named SafeString temporary; root 340, helper 144 nonmatching
8. `0d6dc6a`: restored implicit temporary, caller archive guard; root 344 nonmatching, helper 144 exact

Final root diagnostic comparison: 33 differing bytes in 20 words, primarily stack/register scheduling; exact pool. These are read-only Capstone/byte diagnostics, not a replacement oracle. `tools/diff.py` dependency setup was not permitted, so canonical diff output is unavailable. The canonical object checker above is the acceptance evidence. No ninth source-shape attempt in this pass. Packet `project/pro_requests/00274EF0.md` preserves the unresolved target.

Published files are this report, packet and `lib/al/src/Scene/alSceneFunction.cpp` only. No tools, map, ledger, STATE, compiler flags or binaries. Exact acceptance remains subject to main source-import revalidation.
