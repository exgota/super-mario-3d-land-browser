# Disjoint initializer intake batch

The driver owns cleanup/initializer-intake-batch from main `4e5fe20cb2ed6ff928aaf1244b74181004b748a0`. Source commit `d0557ac53e037afb8840568f638e6fe2191c42e3` combines nineteen byte-identical files from the separately reviewed Bug, flower and Hammer families. There are no overlapping owned source files, and every pre-existing file equals the individual family's frozen base before import. Batching lets one mandatory integrator preservation cycle judge the five new roots; it changes no acceptance requirement, production setting or queue priority rule. No throughput improvement is claimed before the actual verdict.

## Scope and evidence

| Family | Proposed new roots | Proposed complete bytes | Prior checked source | Interface evidence |
| --- | ---: | ---: | --- | --- |
| Bug | 1 | 532 | `6c59fa81185be4e141ea78b772b59f7a47ac7972` | [Bug](bug_initialization_abi.md) |
| FireFlower, BoomerangFlower, SuperLeafSpecial | 3 | 1596 | `fd26fbac41de6b1e9af80dad352fdc1c6195d9f7` | [Flowers](flower_initialization_intake.md) |
| HammerBrosHammer | 1 | 540 | `a58aea9320aed2ccd3c564c642cfc6ec7364091f` | [Hammer](hammer_initialization_intake.md) |

The eleven previously reviewed symbol identities are carried into a separate names-only evidence ancestor, a7214322. They are the same identities proposed by 9b3c89dda and 29bbc9aae. The owner approved the provisional FlowerInit names at 19:16 ET. All other map fields, including ranks, boundaries and pools, remain unchanged. The original names-only requests remain independent.

## Final committed-source verification

Normal `python make.py eu` passes on the final source commit. Canonical `tools/check.py --object` passes all 87 complete intervals across eleven normal project objects in 30.741 seconds. This preserves 82 existing roots/1504 bytes and proposes five new roots/2668 bytes. The checker's source/object provenance gate applies to every interval. Scratch ranks were restored to the committed named map afterward.

The source import is byte-identical to the three reviewed family snapshots. The batch introduces no new implementation form. The incomplete actor interface, provisional compiler-generated table, original-name uncertainty, inferred formal types and unresolved parameter consumer retain their original limits in the linked family evidence. No full class, runtime or gameplay result is claimed.

The integrator must still build this exact source and preserve every previously accepted root. This document does not award any new accepted byte. The source batch replaces the pending source requests cleanup-bug-initialization-abi-2d34a171e, cleanup-flower-initialization-abi-74a2ab71b and cleanup-hammer-initialization-abi-60ddcdffc only after the replacement is successfully queued. Their exact request files and hashes are retained locally for recovery; names-only requests remain queued.

## Final source hashes

| Source | SHA-256 |
| --- | --- |
| `Game/backup/include/Enemy/Bug.h` | `26e9ff9206a1d481ab56c636e317ab2ff44faf5cb402d657ea5174aeaec26d33` |
| `Game/backup/include/Enemy/EnemyStateHipDropDown.h` | `4ca853056122c4ed0d5103f778c5dfe756471ea6be8280fe9dc676c52daf251a` |
| `Game/backup/include/Enemy/HammerBrosHammer.h` | `71ef46f253e21a0c22031dbf78f6f8dc99ac2d43f94b160ea5fe3ffd84f1c9c6` |
| `Game/backup/include/MapObj/BoomerangFlower.h` | `a3f9e66ab9fe360681c10fe3c4c695e462b798d7f69e6467dcc3568c750889bd` |
| `Game/backup/include/MapObj/FireFlower.h` | `1462db6aa78e9e0d10048ca53373a6b601863682796b668e7844b9ecbe13c240` |
| `Game/backup/include/MapObj/FlowerInitNerve.h` | `54fddb2e43051c6187637a5579c7df4f9e7b86108792d2bc1ac625c319311b4c` |
| `Game/backup/include/MapObj/FlowerInitState.h` | `7831191d0ecf0191f9997bd828ef3af453b8db29ef44b97ee9d717344d134af5` |
| `Game/backup/include/MapObj/SuperLeafSpecial.h` | `c228b961dfbce88cc0716c535d81113b18bf091e4acbd21cad60c93872f4c8e3` |
| `Game/backup/src/Enemy/Bug.cpp` | `5d47ee35fefd82c9262e4c7cc525d105d369beb8a404077866d67e698c2fba34` |
| `Game/backup/src/Enemy/HammerBrosHammer.cpp` | `6af99ccd9f9189d07dab6e6e83a98e1779494501fdb20c333a5ee38ccd06d951` |
| `Game/backup/src/Factory/fn_0018938C.cpp` | `156e46a711de88272f5e43cb2665d0b0d4c298042ca8feac70472004ece64a58` |
| `Game/backup/src/Factory/group_001177D4.cpp` | `b6caaad930ae146840c5e66fe2def604d9d412d7adbab6a2669c4247538ff19a` |
| `Game/backup/src/Factory/group_003460E0.cpp` | `2c8a8d463f16aeaecd726f0bd4d7b6640995987ee76cf19ef10f32d31cb1a876` |
| `Game/backup/src/Factory/group_00356920.cpp` | `9223bf5a53d5f5baaa968853babf119413b36de4eaf14ed2acbf8f1a16472c83` |
| `Game/backup/src/Factory/group_00363114.cpp` | `ee10f269687a5de1da657ff7ce04140eac797486f6016c3005a5225e019078dc` |
| `Game/backup/src/MapObj/BoomerangFlower.cpp` | `db9d062c42ea7801f3df38902a6e035352709cf9133a4f877679f9c86f60164d` |
| `Game/backup/src/MapObj/FireFlower.cpp` | `1580e9f62e38666e2353fc347ed05f2a94a09580d88cfe7bd916f97c2905d180` |
| `Game/backup/src/MapObj/FlowerInitNerve.cpp` | `3c17a249e5a910f4b487472d10a7ad1b0497281d73d490217491bfc85d858232` |
| `Game/backup/src/MapObj/SuperLeafSpecial.cpp` | `d77ac6e7367a1536d9022668c762fc709f7c59ea6732ec3c6b35af8dd8e3bb55` |
