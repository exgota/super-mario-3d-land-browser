# Bubble init: corrected literal and Nerve import

## Result and frozen inputs

The unchanged project checker reports the complete `Bubble::init` interval `0030324C..003033C0`, **372 bytes including the literal pool**, byte-exact from committed project-built C++. This is a dot proposal, not main acceptance. The empty-candidate preservation gate and the separate candidate object check are reported independently below. Main must apply the family unchanged and conduct its normal intake.

- Frozen base: `bc5b236802b0300bc6b00f0ac55cf5ae5749c55c`
- Branch: `dot/bubble-init-literals`
- Immutable source commit: `d33899d8f90b63bd2827e37fd165cdeae7443fa4`
- Final source: `Game/backup/src/Enemy/Bubble.cpp`, SHA256 `5718020825a9331d9e8e8f7d88d5a3a4fb50560f51d68d469ec460029182a484`
- Full source patch: the exact `git diff --binary` command under Reproducibility seals; retained locally as `build/dot-bubble-init-evidence/bubble-init-source.patch`, SHA256 `d34595560c18f432310f466fbb831199ede2fa7c2fe326f7ba98ddf773c7d089`
- Historical packet: `project/pro_requests/0030324C.md`, base blob `76d10eb3c3c701ba4b4d85581a83f6208ec13891`
- Original EU input SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

The patch changes one C++ translation unit. No header, ABI layout, base, vtable field, map, ledger, STATE, configuration, tool, or binary input changes. Existing constructor148, attack124 and the zero-size C2 alias remain intact. The source patch passed `git apply --check` against the pristine bc5 checkout.

## Grounded change and attempt history

The prior four-form packet remains intact. Its form 4 was graded on historical base `88d61052d075cec40c4edae51becfb8317348c69`, at `2026-10-02T02:32:34.806165+00:00..02:32:35.798818+00:00`, under 791 and 894. It emitted372 bytes with three pool-byte differences. That Mac scratch object was not independently retrieved, and no new claim about that object is made here.

This pass implements the packet's previously uncompiled corrections using genuine C++: `-50.0f` yields the retail `C2480000` scalar; switch initialization imports existing singleton row `003F1EF0`, rather than `003F1EFC`. The original pool at `003033A0` independently decodes these values. Startup `0037F5E0` initializes six four-byte Nerve objects from base `003F1EF0`, with vtables `003BA984/994/9A4/9B4/9C4/9D4`. Default and blow-down imports remain `003F1EF8` and `003F1F00`. These are existing whole data rows; the source defines no singleton data or vtable replacement.

`BubbleInitialNerve` and `BubbleSwitchNerve` are concrete source-side import types, not claims about original type names. The existing `BubbleBlowDownNerve` declaration is reused unchanged. Existing neutral callee rows270FC4/27CF20/279B64/27D180 resolve without names, boundary or type metadata changes. Binary calling conventions support actor/s0-float/r1-int, actor/info/mode, the `IUseStageSwitch` receiver at this+0xC, and int-output/info respectively. `EnemyStateBlowDown` already has its grounded0x28 layout and typed constructor; Bubble's `_90` pointer was already corrected in the frozen base.

The source-local vector/quaternion copy adapters preserve the packet's destination-before-getter evaluation order. They define no emitted functions and do not represent runtime classes. There was one new physical source form and one successful configured project build in this pass, with no unsuccessful form. 894 is unavailable here and was not installed or claimed tested. Historical failures and earlier 791/894 observations remain historical evidence. No object or literal bytes were edited.

## Stage 1: current-source clean build and all-prior preservation

The supported unchanged gate requires all tracked inputs clean. The existing init row is U at bc5, and the dot cannot commit map changes. Therefore the gate was run with its supported empty-candidate manifest and the original clean map, then the named candidate was checked separately. No preflight bypass or gate change was used.

Manifest:

```json
{"prior_checkpoint":"bc5b236802b0300bc6b00f0ac55cf5ae5749c55c","candidates":[]}
```

Exact command, after ` . ./development_environment.sh `:

```sh
python tools/acceptance_batch.py build/dot-bubble-init-evidence/manifest.json --output build/dot-bubble-init-preservation
```

Invoked at approximately `2026-10-02T04:24:23Z`. Reported elapsed **630.819049951 seconds**. Exit0, clean-build return0. It verified **752 prior roots and all772 actual canonical definitions**, zero failures. Its empty candidate result is `Canonical candidates: 0 / 0`; it is not an acceptance_batch candidate pass. The project emitted180 objects/provenance records:46Game,132al,1SDK and1generated stub. The normal clean compact scaffold link succeeded; the U candidate's body proof is the separate object check below.

Report: `build/dot-bubble-init-preservation/report.json`, SHA256 `8cca6a019bd0a6f36beeeb0fecc820eda8e833cb2d2687a0f8270ea470da4347`. Clean build log: `build/dot-bubble-init-preservation/clean_build.log`, SHA256 `b1c934aee29105dce2ba499061d8bad000402220f8cc097610195a019a46d50f`. Every per-definition output is retained in the report. Independent pristine bc5 baseline supplied by the coordinator passed752/772 in609.803394764seconds; it does not replace this changed-source gate.

## Stage 2: separate unchanged candidate checker

Exact command, on the same source-generated object after Stage1 completed:

```sh
python tools/check.py _ZN6Bubble4initERKN2al13ActorInitInfoE --object build/eu/obj/Game/backup/src/Enemy/Bubble.o
```

Start `2026-10-02T04:35:31.290285+00:00`; end `2026-10-02T04:35:31.933362+00:00`; elapsed **0.643085515 seconds**, exit0. Exact output, with terminal color escapes removed:

```text
U -> O: The complete source-generated function interval matches byte for byte.
```

The checker alone set O temporarily. The map was immediately restored from the pre-gate original bytes; original/restored SHA256 `350611b6caf9bdba44895628edcf7955b53964976f621613198589760d9217e4`. No rank/name enrollment or map commit is in this proposal. Candidate log/command/timing/restoration record: `build/dot-bubble-init-evidence/candidate-check.json`, SHA256 `75abb6703bbc33a0d2bb8aac3404b7b965892d959ef7073824a939e927f94556`.

The unmodified ARMCC object is `build/eu/obj/Game/backup/src/Enemy/Bubble.o`, SHA256 `3e6306d43bedfac45027d344eedaf310a32bb10f046a117ff2e7a2533cce92fb`. Its recorded source/configuration closure has30 inputs and uses configured ARMCC4.1/791. Provenance: `build/eu/obj/Game/backup/src/Enemy/Bubble.provenance.json`, SHA256 `ba9ee747066cebac2c93731557d091b552122563c33998ffdb6f5d193cd00c29`.

The check directory is `build/exact_checks/eu/function_0030324C_5q7jzcgt`. Its linked image has exactly one allocated section, `CANDIDATE_CODE`, address0030324C, size372, SHA256 `0a2cc83a273025d57cb0355d006cd55c88c481685e86c152aea121156da8f2c1`. The checker's isolated object contains exactly one function definition, init372. Only the project's unchanged check implementation creates these diagnostic projection/link artifacts; none was manually modified or submitted as compiler output.

## Whole-family and helper closure

The canonical object defines init372, constructor148, attack124 and the zero-size C2 alias sharing C1's section. Constructor/attack and every other accepted definition pass Stage1. Current includes also cause five unchanged header methods to emit as weak4-byte functions. None has an established named map identity; no address was assigned:

- `_ZN2al15IUseAudioKeeper2v1Ev`, section `i._ZN2al15IUseAudioKeeper2v1Ev`,4bytes
- `_ZN2al15IUseAudioKeeper2v2Ev`, section `i._ZN2al15IUseAudioKeeper2v2Ev`,4bytes
- `_ZN2al9LiveActor17calcAndSetBaseMtxEv`, section `i._ZN2al9LiveActor17calcAndSetBaseMtxEv`,4bytes
- `_ZN2al9LiveActor3v22Ev`, section `i._ZN2al9LiveActor3v22Ev`,4bytes
- `_ZN2al9LiveActor3v23Ev`, section `i._ZN2al9LiveActor3v23Ev`,4bytes

These sections are not reachable from the selected init body and are absent from the unchanged checker's isolated object and linked image. They receive no identity or credit. The vector/quaternion adapters emit no functions. Import/definition inventories and before/after object observations are retained under `build/dot-bubble-init-evidence/`, including `object-closure.json`, `prior-object-closure.json`, `weak-method-map-identities.json` and `final-closure.json`.

## Reproducibility seals

Source patch reconstruction:

```sh
git diff --binary bc5b236802b0300bc6b00f0ac55cf5ae5749c55c d33899d8f90b63bd2827e37fd165cdeae7443fa4 -- Game/backup/src/Enemy/Bubble.cpp
```

Unchanged tool/configuration/compiler hashes:

```text
f63eacb2b4914872a8258b41d48f5010733b58b69e046c7c07cb55932eeca0f5  tools/acceptance_batch.py
e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317  tools/check.py
aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2  tools/low/checkExactBytes.py
343d1a0954aba9a4805e71420be9a99d91185db4c8b329d1363d11efcee53529  tools/low/buildProvenance.py
b365cd4efca912dabf4b71dbc28d638bc9624cff5bd0669e11d794aa367831fc  tools/diff.py
5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45  data/config.json
d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d  data/compilers/4.1/791/bin/armcc.exe
b9cffb71d7b58f3fd33fa8c4c8af884b75689bb5a9e9014309a08146bba676dc  data/compilers/4.1/791/bin/armlink.exe
e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe  data/compilers/4.0/902/bin/armcc.exe
aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b  data/compilers/wibo
```

Exact ARMCC invocation recorded by the project:

```sh
/workspace/scratch/73cdb2c524af/mario-bubble-init-literals/data/compilers/wibo /workspace/scratch/73cdb2c524af/mario-bubble-init-literals/data/compilers/4.1/791/bin/armcc.exe -I/workspace/scratch/73cdb2c524af/mario-bubble-init-literals/Game/backup/include -I/workspace/scratch/73cdb2c524af/mario-bubble-init-literals/lib/al/include -I/workspace/scratch/73cdb2c524af/mario-bubble-init-literals/lib/CtrSDK/include -I/workspace/scratch/73cdb2c524af/mario-bubble-init-literals/lib/sead/include -DVERSION=EU -DNN_SWITCH_DISABLE_ASSERT_WARNING_FOR_SDK=1 -DNN_SWITCH_DISABLE_DEBUG_PRINT_FOR_SDK=1 -DNON_MATCHING=1 --cpu=MPCore --fpmode=fast --apcs=/interwork --depend_format=unix_quoted --diag_suppress=1608 --arm_only --no_exceptions --diag_style=gnu --arm_only --no_exceptions --diag_style=gnu --signed_chars --dollar --force_new_nothrow --no_rtti --no_debug_macros --no_depend_system_headers -O3 -Otime --gnu --split_sections --force_new_nothrow --multibyte_chars --enum_is_int --signed_chars --no_rtti_data --forceinline --remove_unneeded_entities --no_debug --preinclude=/workspace/scratch/73cdb2c524af/mario-bubble-init-literals/Game/project_globals.h --sys_include -c '-D__BASE_FILE_NAME__="Bubble.cpp"' -o /workspace/scratch/73cdb2c524af/mario-bubble-init-literals/build/eu/obj/Game/backup/src/Enemy/Bubble.o --depend /workspace/scratch/73cdb2c524af/mario-bubble-init-literals/build/eu/obj/Game/backup/src/Enemy/Bubble.d /workspace/scratch/73cdb2c524af/mario-bubble-init-literals/Game/backup/src/Enemy/Bubble.cpp
```

All30 committed source/configuration dependency hashes:

```text
c930766da9ebd67ee82a229d8027fbb601e58cfa3b01bdde15d6f499c1542ea2  Game/backup/include/Enemy/Bubble.h
1d5f96a4459f2f12be538a1e69f42c9bc7d3e5cde90e32f65c19cdad42c56e5e  Game/backup/include/Enemy/EnemyStateBlowDown.h
5718020825a9331d9e8e8f7d88d5a3a4fb50560f51d68d469ec460029182a484  Game/backup/src/Enemy/Bubble.cpp
6c0b7103d61651e77e1a5db8bb1f5bdf509ad3d64c419c4ba22cadaed6a933d1  Game/project_globals.h
5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45  data/config.json
40aa4e5ee7d8980e077963ee87bc6bdeb8908249603c073b5980aed937d5e0bc  lib/CtrSDK/include/nn/types.h
69a1a49e3aa18ed22c79592ff78c73a974a3022acf05048365e347bedb7c4a4d  lib/al/include/Audio/alAudioKeeper.h
6d29cbb16f52fb8f3cc14374d3784e261d19309b3475456170a7bbc66a073451  lib/al/include/Effect/alEffectKeeper.h
689ae3d58eb6c6801fcdf0c37866b0c05a28cc42abb36a08b498187821701c2f  lib/al/include/LiveActor/alActorInitUtil.h
b7ae48418c39def43ff701ca81857fd7279759d11f289fbde42ea9fc53c982af  lib/al/include/LiveActor/alActorPoseKeeper.h
e50328b39d81b7daf03e1c22742f9d06ee73a6cbe11b6b49578064972d541723  lib/al/include/LiveActor/alHitSensorFunction.h
472992d0d1a423b4de484c00ffe79fb556dfdd111b84b859518142d5d5b48f34  lib/al/include/LiveActor/alLiveActor.h
bc855adbda47baffa6061c368ea907c0ee22d9b4009a2e638cbb9af0b8a85c8b  lib/al/include/LiveActor/alLiveActorFlag.h
d572d752bebc4c5f37571bd1f7a96a8f3e2f5dc7d27721abee641fbd78518233  lib/al/include/LiveActor/alLiveActorFunction.h
f2a557f0bac77850ba1d23da1941fa1a458983fa20abacdf1d4f6d863d671892  lib/al/include/LiveActor/alSensorMsg.h
2034a69366356207c8975bfcb8b2f2ef385d1d614fabfceb24e7b4810800b2e3  lib/al/include/MapObj/alMapObjActor.h
4c1e9741c95a4a40fdae00d33a972638ae00c7209a3dd7e6b317647de6995ee7  lib/al/include/Nerve/alActorStateBase.h
131fc15b422681d47eccb569de9e24f3990b7ad6721d9fafa30be6c791a27ec0  lib/al/include/Nerve/alNerve.h
7b0b79c632956a3697160a53f5102c6634238473b2313212919d2937a0100d5f  lib/al/include/Nerve/alNerveExecutor.h
cc7b91a20cf27d2f1f9fb86e0472cb90ebee5609ea7d15fb623b43c2ee1cd267  lib/al/include/Nerve/alNerveFunction.h
28065b7f6feb35d8a0ab88de3bb9596758dd816d3084e037dfd2f129eb16fb46  lib/al/include/Nerve/alNerveKeeper.h
115f8687977cdb0d075d335f02b8c958ec85682418e84e15df5d2ecd392ae953  lib/al/include/Nerve/alNerveStateBase.h
a75dd1efbebb8554cb9cc105758857ef4aa5237ffab131e60deb4d72bcf0e586  lib/al/include/Placement/alPlacementFunction.h
654fc0b4e0df90e6320615cd2de35a785e455270a2449b9b649bca92103c73af  lib/al/include/Placement/alPlacementInfo.h
58e83a38a685cdca5d47900c75f5618049aec86b9f16bbf2876807ab7d625287  lib/al/include/Stage/alStageSwitchKeeper.h
3ceeca89571abeb56431268928af2c13cf412debf7bc751a0b46ffeeabf6a4c4  lib/al/include/Yaml/alByamlIter.h
c4ecd6d6b98b4851e81d9fd144de80b51ca13f4370fb2baf5982a42f6e69d914  lib/sead/include/math/seadMatrix.h
84b3de2e49e5f0505d26ae1e9f5acf782b446507254c6b0e94657350cd1355be  lib/sead/include/math/seadQuat.h
1416056d4201ee62fdc2d2c06b5bd418f3195f8154a74b122869ff8c5f517169  lib/sead/include/math/seadVector.h
e1a13277a1e5c3278d9811cd92e7d737caca4d7ae1bc9b604b99a45e3bbd1da7  lib/sead/include/prim/seadSafeString.h
```

Source investigation began at about2026-10-02T04:21:10Z; evidence sealing completed `2026-10-02T04:38:21.398940+00:00`. Measured build/preservation and candidate-check times above exclude remaining report preparation. No canonical-main accepted bytes or throughput are claimed by this proposal. No broader behavior replay, full-image exactness or gameplay claim follows from this single exact interval. Publication is reserved to the coordinator.
