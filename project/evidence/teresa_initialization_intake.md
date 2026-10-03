# Teresa initialization and shared import reconciliation

Frozen accepted base: `56b95336e488d69c16709d8db354f1d22ef8b001`, after Bug and the other initializer families were accepted at `cf2886000d0c49e12faddf4cd77a1d47372735c8`. Final committed source: `b1d4a94a8593766f11513e0cb2f82c9b90f6e790`. Supplier: `dot/root-30a714` at `f58cba4f22a627d0749488451639384f4723e384`. Source ownership was recorded before edits. This family has twelve source/header files; it does not overwrite the previously accepted Bug correction.

## Result and scope

The normal project build compiled and linked the committed family. The unchanged canonical object checker passed 27 complete intervals, 3,984 bytes, in 22.444672 seconds: 26 previously exact roots/3,440 bytes preserved, plus Teresa::init at `0030A714..0030A934`, 544 proposed new bytes. Every originally exact definition in the ten affected normal source objects was included. The global duplicate-definition audit reported zero conflicts. The scratch map was restored byte-for-byte to the committed map, which already contains the separately accepted Teresa reconstruction name.

The three other mapped definitions already present in affected source remain outside accepted credit: FallMapParts constructor `00137068` and EffectObj constructor `0031B56C` fail source-closure checking on unresolved non-branch imports; FallMapParts FallSign execute `00357B74` fails complete-section size. They were U in the frozen base, their bodies were not rewritten, and no claim is made for them. These three recorded failures are separate from the 27 passing checks, not new exact-function regressions. The driver does not claim a fresh whole-map preservation pass.

Only the integrator may accept the new root, perform global regression checking, change ranks, write the ledger or move main. Driver acceptance throughput for this not-yet-accepted source is zero bytes/hour.

## One shared declaration source

The declaration-only `alActorInitializationImports.h` replaces local prototypes in the ten affected C++ files. It contains three global C-linkage imports and forward declarations, with no helper bodies, wrappers, alternate names, storage or new runtime allocation. A source search finds zero obsolete local declarations of these imports in the ten touched C++ files.

- `fn_002794F8(int*, const al::ActorInitInfo&)` returns bool. The original reads placement through ActorInitInfo, uses the accepted integer-key helper, rejects -1, writes an integer only on success and returns 0/1. The accepted Bug and Factory integer-output correction is preserved.
- `fn_00280538(al::IUseStageSwitch*, const al::ActorInitInfo&)` returns int. Original virtual slots zero and one match the public stage-switch keeper interface. Its tail path through `0024392C` reaches accepted `StageSwitchAccesser::initWithPlacementInfo` at `001DCD2C`, whose declared return is int and actual result is 0/1. Independent caller `0011B414..0011B424` consumes the result. Void discards a real contract. The original bool-versus-int spelling is not recovered; int is the conservative reconstruction following that accepted tail callee.
- `fn_0027FAB8(al::LiveActor*)` returns bool. Original code retains the unadjusted actor, performs its own stage-switch conversion, invokes makeActorDead or makeActorAppeared, and returns 0/1. Teresa consumes this result and retains its observed additional makeActorDead call.

Existing public actor subclasses retain their natural +0x0C stage-switch base adjustment. The two Factory storage views retain the already observed addresses through explicit typed import-boundary views: stage-switch receiver at +0x0C, whole actor receiver unadjusted. The public ActorInitInfo reference preserves the original pointer argument. These casts do not assert that either private Factory struct is a complete original actor class. Unrelated helper declarations and field extents remain unchanged.

In particular, fn_0027D180 and fn_00270AB0 are outside this family. The older float-output contract for fn_0027D180 remains a separate documented prerequisite, not silently repaired here. The current change removes no public API or persisted format.

## Teresa provenance and limits

The existing actor-registry/constructor evidence binds the Teresa reconstruction to a 0xB4-byte original allocation and the init dispatch slot. The header is explicitly an init-only storage view. Unused fields and the +0x60 secondary-interface storage remain opaque; no full original class, constructor, child-area class or virtual-table reconstruction is claimed.

The imported four-byte nerve at `003F23E0` has an independently inspected zero-offset al::Nerve interface. The shared-import audit records its original startup, table and execute evidence; this change adds no nerve implementation or naming row. The canonical check validates the source import closure.

The normal Teresa object emits its 544-byte root, a 152-byte class table, ordinary SafeString support and shared weak virtual methods. The genuine Teresa class table is absent from the final compact linked image and receives no exact credit. The root is the only new canonical function body in this import. Its Japanese literals retain the supplier Shift-JIS bytes. No new renderer, browser, gameplay or runtime-equivalence result is claimed.

## Canonical function checks

| Address | Symbol | Complete bytes | Status |
| --- | --- | ---: | --- |
| `0012DE18` | `_ZN11KoopaPillar4initERKN2al13ActorInitInfoE` | 516 | Preserved O |
| `00136F64` | `_ZN2al12FallMapParts10receiveMsgEjPNS_9HitSensorES2_` | 140 | Preserved O |
| `00136FF0` | `_ZN2al12FallMapParts4initERKNS_13ActorInitInfoE` | 120 | Preserved O |
| `001674A8` | `_ZN15TransparentWall13makeActorDeadEv` | 4 | Preserved O |
| `001674AC` | `_ZN15TransparentWall4initERKN2al13ActorInitInfoE` | 200 | Preserved O |
| `00167574` | `_ZN15TransparentWallC1ERKN4sead14SafeStringBaseIcEE` | 44 | Preserved O |
| `001794C0` | `fn_001794C0` | 244 | Preserved O |
| `00186CC8` | `_ZN18AquariumSwimDebris4initERKN2al13ActorInitInfoE` | 124 | Preserved O |
| `00186D44` | `_ZN18AquariumSwimDebrisC1ERKN4sead14SafeStringBaseIcEE` | 44 | Preserved O |
| `0018938C` | `fn_0018938C` | 244 | Preserved O |
| `0025AA18` | `_ZN2al3Sky8calcAnimEv` | 28 | Preserved O |
| `002D4544` | `_ZN3Bug4initERKN2al13ActorInitInfoE` | 532 | Preserved O |
| `002D4FF4` | `_ZN2al3Sky4initERKNS_13ActorInitInfoE` | 72 | Preserved O |
| `002D503C` | `_ZN2al3SkyC1EPKc` | 84 | Preserved O |
| `0030A714` | `_ZN6Teresa4initERKN2al13ActorInitInfoE` | 544 | Proposed new O |
| `0031B378` | `_ZN2al9EffectObj17makeActorAppearedEv` | 52 | Preserved O |
| `0031B3AC` | `_ZN2al9EffectObj4initERKNS_13ActorInitInfoE` | 176 | Preserved O |
| `0031B45C` | `_ZN2al17EffectObjFunction18initActorEffectObjEPNS_9EffectObjERKNS_13ActorInitInfoEPKc` | 156 | Preserved O |
| `0031B4F8` | `_ZN2al9EffectObj4killEv` | 40 | Preserved O |
| `0031B520` | `_ZN2al9EffectObj7controlEv` | 76 | Preserved O |
| `00336F6C` | `_ZNK2al5Nerve12executeOnEndEPNS_11NerveKeeperE` | 4 | Preserved O |
| `00357990` | `_ZNK2al15NrvFallMapParts18FallMapPartsNrvEnd7executeEPNS_11NerveKeeperE` | 148 | Preserved O |
| `00357A24` | `_ZNK2al15NrvFallMapParts19FallMapPartsNrvFall7executeEPNS_11NerveKeeperE` | 180 | Preserved O |
| `00357AD8` | `_ZNK2al15NrvFallMapParts19FallMapPartsNrvWait7executeEPNS_11NerveKeeperE` | 56 | Preserved O |
| `00357B10` | `_ZNK2al15NrvFallMapParts21FallMapPartsNrvAppear7executeEPNS_11NerveKeeperE` | 100 | Preserved O |
| `00367EC8` | `_ZNK21NrvAquariumSwimDebris27AquariumSwimDebrisNrvAppear7executeEPN2al11NerveKeeperE` | 48 | Preserved O |
| `00375B38` | `_ZNK2al9EffectObj10getBaseMtxEv` | 8 | Preserved O |

## Final source hashes

| Path | SHA-256 |
| --- | --- |
| `lib/al/include/LiveActor/alActorInitializationImports.h` | `bd1004f3e60809db62fd170fcd0d3fc2901597a91e81ac52f63652d69ce2391f` |
| `Game/backup/src/Factory/fn_0018938C.cpp` | `05ca730525b44ad5375319ea525f8275f255c0d5509cbc747c7dc907fd0269a0` |
| `Game/backup/src/Factory/fn_001794C0.cpp` | `aadc9fb5b04149a98fd2a927f396f45e04799b833a9dff9f8604bd856c9dd9f1` |
| `Game/backup/src/MapObj/KoopaPillar.cpp` | `be0c28da44e5f3eeb551191ff5d26e3edc950a73d6abbb9dd94866b10e04cd90` |
| `Game/backup/src/MapObj/TransparentWall.cpp` | `e46ede8c918d08805dc7e9c00396524f4b071a0c177f1dbc2f136f5417b7a7a3` |
| `Game/backup/src/Npc/AquariumSwimDebris.cpp` | `a4f8039b57bfe9dc6f42c161860739ed18febb161ce9d9dda3ce06f0f3e0f17b` |
| `lib/al/src/MapObj/alFallMapParts.cpp` | `e53556d14b27ba46ddd39720dd5e2a50e7eca270842681968c27cef1642313e8` |
| `lib/al/src/Npc/alEffectObj.cpp` | `2c70ff6f5d9dacc4e81b089a3ac7d28642a6cde9ac6f2a96fc39a73a8def8298` |
| `lib/al/src/Npc/alSky.cpp` | `65a6a0161abe34232b3f3659d2dc6c6203f3759e937e110ce109ad4fd705ed82` |
| `Game/backup/src/Enemy/Bug.cpp` | `a7d42d989b08b8ae15ff22d402c266b91a0e7e573b12fe6e587832bc5dbbf226` |
| `Game/backup/include/Enemy/Teresa.h` | `1e52aad2c0e742a21e25cb4a83c763dc37e5f9d4dda04904d98c276130422b69` |
| `Game/backup/src/Enemy/Teresa.cpp` | `fe18bccc3b7fb3f1194cf9dc14d03e16a16efa37504bca2e5a8305235a08d6a4` |
