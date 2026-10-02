# FunctorV0M cleanup evidence

Cleanup lane `cleanup/functor-v0m` owns this change, based on `e8cc28789146f8adefb87de4b218a032767296bd`.
Only the owner's EU dump, the existing map, and repository headers/source were
used. This note records identifiers, addresses and type/layout evidence; it
contains no binary data or disassembly.

## Inventory

The 72-member family is the existing AppearStep instance (vtable symbol
0x003D5A34, callable address 0x003D5A3C) plus 71 callable tables at
0x003D5A4C through 0x003D5EAC. These 71 have 142 virtual functions,
141 still expressed as raw Factory functions; Garigari's operator already uses
the template. AppearStep's two functions already use the template and remain
unchanged. The row 0x003D5EBC..0x003D5F4C is an actor vtable: constructor
0x002627B0 installs it after calling LiveActor's constructor, and its entries
include LiveActor's init, placement, appear and other actor virtual methods.
It is not another FunctorV0M instance. This inventory covers the 144 virtual
functions of these 72 instances.

The scan considers every function's map-declared literal-pool range and filters
references to the callable tables. Clone self-references are excluded when
identifying builders. Direct callbacks are read from the member-pointer
descriptor the builder copies; virtual callbacks are resolved by their slot in
the actual object's table and the corresponding existing class declaration.

## Grounded instances

All reconstructed types use `FunctorV0M<Host*, void (Host::*)()>`. Each operator
uses the existing template's parent/member-pointer invocation. Each clone uses
the copy construction specialization already used in AppearStep.cpp. The
member-pointer representation and dispatch remain supplied by the compiler.

| Callable table | Host and callback | Builder evidence | Source |
| --- | --- | --- | --- |
| 0x003D5A9C | StageScene, virtual init() | 0x001A50CC and 0x002574C4 bind the object returned by 0x00124614, not the builder's this. That constructor calls Scene's constructor, installs table 0x003C52C8, stores the stage-start parameter at 0x34 and initializes fields through 0x98, as declared in Scene/StageScene.h. Its stage-scene display identifier agrees with that header. Descriptors at 0x003AE4C0 / 0x003AE62C select virtual slot 0x14, init(), declared in Scene/alScene.h. | Game/backup/src/Functor/FunctorV0M.cpp |
| 0x003D5CBC | CourseSelectScene, virtual init() | 0x002123D4 binds the result of constructor 0x0017D8C0. That constructor calls Scene's constructor, uses the course-select-scene display identifier, installs table 0x003CD21C and stores the start parameter at 0x34, agreeing with Scene/CourseSelectScene.h. Descriptor 0x003AE184 selects init() at slot 0x14, declared in Scene/alScene.h. | Game/backup/src/Functor/FunctorV0M.cpp |
| 0x003D5D6C | BlockDragonGenerator::startAppear() | 0x00192C80 copies descriptor 0x003BA584, whose direct callback is the map-named method 0x001931B4, and binds its this. Enemy/BlockDragonGenerator.h declares that method. | Game/backup/src/Enemy/BlockDragonGenerator.cpp |
| 0x003D5E3C | DemoScene::init() | 0x0018AFB8, 0x001B5784, 0x0036944C, 0x0036A87C and 0x00371F20 all bind the result of constructor 0x00255A70. It calls Scene's constructor, uses the demo-scene display identifier, installs table 0x003D4944 and stores the stage-start parameter at 0x34, agreeing with Scene/DemoScene.h. The copied descriptors select slot 0x14, the init() override declared in that header. | Game/backup/src/Functor/FunctorV0M.cpp |
| 0x003D5EAC | al::LiveActor::appear() / kill() | Builders 0x0027AE3C, 0x0027ECA4 and 0x0027FAB8 take LiveActor*, grounded by existing callers in TransparentWall.cpp, AquariumSwimDebris.cpp, alSky.cpp and alFallMapParts.cpp. They bind that pointer and copy descriptors at 0x003A9820 selecting virtual slots 0x0C and 0x14. LiveActor's named table 0x003D7974 has its address point at 0x003D797C; those slots resolve to the map-named appear() and kill(), both declared in LiveActor/alLiveActor.h. Other references are 0x0012D790, 0x0013C500 and 0x0031B5A8. | lib/al/src/LiveActor/alLiveActor.cpp |

## Map evidence

Only the ten function Symbol fields change. Their Start, Pool, End, Section,
Rank, Type and SectionName stay exactly as on the base. Every existing function
rank is preserved. The following data-only partitions establish the real
vtable symbol addresses independently of the checks.

ARMCC's existing AppearStep object relocates its clone's table literal against
the mangled vtable symbol with addend 8; its mapped symbol begins at 0x003D5A34
and its address point is 0x003D5A3C. The five raw builders and clones store
their table address points, not the ABI symbol addresses. Each selected table's
ABI header begins eight bytes before its two callable entries, and the next
table's header begins immediately after them. The old anonymous 16-byte rows
grouped two callable entries with the following table's header. Repartition
only the two anonymous data rows covering each selected table into an 8-byte
previous tail, the actual 16-byte header-plus-callables table, and an 8-byte
following header. All data remains U, the total covered interval is unchanged,
and no function boundary or oracle tool changes. The preceding raw clone's
referenced data-row start is retained.

| Address point | Vtable symbol start | Vtable symbol end | Symbol |
| --- | --- | --- | --- |
| 0x003D5A9C | 0x003D5A94 | 0x003D5AA4 | `_ZTVN2al10FunctorV0MIP10StageSceneMS1_FvvEEE` |
| 0x003D5CBC | 0x003D5CB4 | 0x003D5CC4 | `_ZTVN2al10FunctorV0MIP17CourseSelectSceneMS1_FvvEEE` |
| 0x003D5D6C | 0x003D5D64 | 0x003D5D74 | `_ZTVN2al10FunctorV0MIP20BlockDragonGeneratorMS1_FvvEEE` |
| 0x003D5E3C | 0x003D5E34 | 0x003D5E44 | `_ZTVN2al10FunctorV0MIP9DemoSceneMS1_FvvEEE` |
| 0x003D5EAC | 0x003D5EA4 | 0x003D5EB4 | `_ZTVN2al10FunctorV0MIPNS_9LiveActorEMS1_FvvEEE` |

| Function address | Old symbol | New symbol |
| --- | --- | --- |
| 0x0039B56C | `fn_0039B56C` | `_ZNK2al10FunctorV0MIP10StageSceneMS1_FvvEE5cloneEv` |
| 0x0039B5AC | `fn_0039B5AC` | `_ZNK2al10FunctorV0MIP10StageSceneMS1_FvvEEclEv` |
| 0x0039C33C | `fn_0039C33C` | `_ZNK2al10FunctorV0MIP17CourseSelectSceneMS1_FvvEE5cloneEv` |
| 0x0039C37C | `fn_0039C37C` | `_ZNK2al10FunctorV0MIP17CourseSelectSceneMS1_FvvEEclEv` |
| 0x0039C7B4 | `fn_0039C7B4` | `_ZNK2al10FunctorV0MIP20BlockDragonGeneratorMS1_FvvEE5cloneEv` |
| 0x0039C7F4 | `fn_0039C7F4` | `_ZNK2al10FunctorV0MIP20BlockDragonGeneratorMS1_FvvEEclEv` |
| 0x0039CCFC | `fn_0039CCFC` | `_ZNK2al10FunctorV0MIP9DemoSceneMS1_FvvEE5cloneEv` |
| 0x0039CD3C | `fn_0039CD3C` | `_ZNK2al10FunctorV0MIP9DemoSceneMS1_FvvEEclEv` |
| 0x0039CFD4 | `fn_0039CFD4` | `_ZNK2al10FunctorV0MIPNS_9LiveActorEMS1_FvvEE5cloneEv` |
| 0x0039D014 | `fn_0039D014` | `_ZNK2al10FunctorV0MIPNS_9LiveActorEMS1_FvvEEclEv` |

## Blocked instances

These 66 instances retain their raw Factory definitions. A method labeled
`fn_...` is still unnamed for this task. Binary behavior or an adjacent named
class does not supply a real missing class or callback name. Garigari's existing
template operator is retained, but its raw clone is blocked by its unnamed
callback. AppearStep is already complete, rather than blocked.

| Callable table | Builder(s) | Reason |
| --- | --- | --- |
| 0x003D5A4C | 0x00117A34 | Host T has no grounded map/header name; callback 0x00117BB4 has no real map/header method name. |
| 0x003D5A5C | 0x0011E908 | Host T has no grounded map/header name; callback 0x0011E9E0 has no real map/header method name. |
| 0x003D5A6C | 0x0011EAB8 | Host T has no grounded map/header name; callback 0x0011EC10 has no real map/header method name. |
| 0x003D5A7C | 0x00120514 | Host T has no grounded map/header name; callbacks 0x0012088C, 0x001208E4 have no real map/header method name. |
| 0x003D5A8C | 0x00122144 | Host T has no grounded map/header name; callback 0x00122454 has no real map/header method name. |
| 0x003D5AAC | 0x001248FC | Host T has no grounded map/header name; callback 0x00124C54 has no real map/header method name. |
| 0x003D5ABC | 0x00183924, 0x00183AB8 | Host T is unnamed; both builders bind the title-scene constructor 0x001267B4. No corresponding class declaration or named map method grounds T. Callback is virtual init(). |
| 0x003D5ACC | 0x00128914 | Host T has no grounded map/header name; callbacks 0x00128CE4, 0x00128D7C have no real map/header method name. |
| 0x003D5ADC | 0x0012B018 | Host T has no grounded map/header name; callback 0x0012B3B4 has no real map/header method name. |
| 0x003D5AEC | 0x0012BBE4 | Host T is unnamed; callback is virtual appear() at slot 0x0C, but no map/header identifies this actor class. |
| 0x003D5AFC | 0x0012D370 | Host T has no grounded map/header name; callbacks 0x0012D22C, 0x0012D2D8 have no real map/header method name. |
| 0x003D5B0C | 0x0012D790 | Host T and direct callback 0x0012D6D8 are unnamed; the additional virtual appear() callback does not identify T. |
| 0x003D5B1C | 0x0012FB7C | Host T has no grounded map/header name; callbacks 0x0012FE28, 0x0012FE34 have no real map/header method name. |
| 0x003D5B2C | 0x00136368 | Host T has no grounded map/header name; callback 0x00136208 has no real map/header method name. |
| 0x003D5B3C | 0x0013C500 | Host T and direct callback 0x0013C408 are unnamed; the additional virtual appear() callback does not identify T. |
| 0x003D5B4C | 0x0013C704 | Host T has no grounded map/header name; callback 0x0013C878 has no real map/header method name. |
| 0x003D5B5C | 0x0013E3F0 | Host T has no grounded map/header name; callback 0x0013E358 has no real map/header method name. |
| 0x003D5B6C | 0x0013F478 | Host T is unnamed; virtual callback slot 0x18 belongs to this actor class, which has no grounded map/header name. |
| 0x003D5B7C | 0x00140584 | Host T has no grounded map/header name; callback 0x001406B0 has no real map/header method name. |
| 0x003D5B8C | 0x00143ADC | Host T has no grounded map/header name; callback 0x00143C5C has no real map/header method name. |
| 0x003D5B9C | 0x0014414C | Host T is unnamed; virtual appear()/kill() callbacks do not identify this actor class. |
| 0x003D5BAC | 0x00144E98 | Host T has no grounded map/header name; callback 0x00144FC4 has no real map/header method name. |
| 0x003D5BBC | 0x00146ECC | Host T has no grounded map/header name; callback 0x00146D88 has no real map/header method name. |
| 0x003D5BCC | 0x0014DC68 | Host T has no grounded map/header name; callback 0x0014DF2C has no real map/header method name. |
| 0x003D5BDC | 0x00153D84 | Host T has no grounded map/header name; callback 0x00153E6C has no real map/header method name. |
| 0x003D5BEC | 0x00154F68 | Host T has no grounded map/header name; callback 0x00154DB8 has no real map/header method name. |
| 0x003D5BFC | 0x0015B2F0 | Host T has no grounded map/header name; callback 0x0015B3CC has no real map/header method name. |
| 0x003D5C0C | 0x0015C388 | Host T is unnamed; virtual appear()/kill() callbacks do not identify this actor class. |
| 0x003D5C1C | 0x0015D324 | Host T has no grounded map/header name; callbacks 0x0015D6E8, 0x0015D6FC have no real map/header method name. |
| 0x003D5C2C | 0x0015E34C | Host T has no grounded map/header name; callback 0x0015E4C0 has no real map/header method name. |
| 0x003D5C3C | 0x00372394 | Host T is unnamed; builder binds constructor 0x001605F0 (photo-album scene), with no corresponding class declaration or map name. Callback is virtual init(). |
| 0x003D5C4C | 0x00163320 | ProductSequence is grounded by named constructor 0x00163B88 installing the builder's host table 0x003CB108, but callback 0x00163290 has no real map/header method name. |
| 0x003D5C5C | 0x00169B08 | Host T has no grounded map/header name; callbacks 0x00169D58, 0x00169DC4 have no real map/header method name. |
| 0x003D5C6C | 0x0016C0BC | Host T has no grounded map/header name; callback 0x0016C2D8 has no real map/header method name. |
| 0x003D5C7C | 0x0016FA54 | Host T has no grounded map/header name; callback 0x0016FC30 has no real map/header method name. |
| 0x003D5C8C | 0x00176480 | Host T has no grounded map/header name; callbacks 0x001766A0, 0x001766D4 have no real map/header method name. |
| 0x003D5C9C | 0x00178868 | Host T has no grounded map/header name; callback 0x001786A4 has no real map/header method name. |
| 0x003D5CAC | 0x0017AFE4 | Host T has no grounded map/header name; callback 0x0017B268 has no real map/header method name. |
| 0x003D5CCC | 0x001B5DBC, 0x001B6088 | Host T is unnamed; builders bind constructor 0x0017F86C (Toad-house scene), with no corresponding class declaration or map name. Callback is virtual init(). |
| 0x003D5CDC | 0x0017FA24 | Host T has no grounded map/header name; callbacks 0x0017FB2C, 0x0017FB70 have no real map/header method name. |
| 0x003D5CEC | 0x00180FA0 | Host T has no grounded map/header name; callback 0x0018101C has no real map/header method name. |
| 0x003D5CFC | 0x00184424 | Host T has no grounded map/header name; callbacks 0x001843F4, 0x001846A0 have no real map/header method name. |
| 0x003D5D0C | 0x001855C0 | Host T has no grounded map/header name; callback 0x00185B40 has no real map/header method name. |
| 0x003D5D1C | 0x0018D0B4 | Host T has no grounded map/header name; callbacks 0x0018D084, 0x0018D328 have no real map/header method name. |
| 0x003D5D2C | 0x0018D404 | GhostPlayerRecorder is grounded by named constructor 0x0018D828 installing table 0x003CE760, whose initSceneObj slot is this builder. Callback 0x0018D640 has no real map/header method name. |
| 0x003D5D3C | 0x0018D96C | Host T has no grounded map/header name; callback 0x0018DA74 has no real map/header method name. |
| 0x003D5D4C | 0x001903A0 | Host T has no grounded map/header name; callback 0x00190334 has no real map/header method name. |
| 0x003D5D5C | 0x001907E4 | Host T has no grounded map/header name; callbacks 0x0019077C, 0x001909A8, 0x001909D0 have no real map/header method name. |
| 0x003D5D7C | 0x00193570 | Host T has no grounded map/header name; callbacks 0x00193A30, 0x00193A3C have no real map/header method name. |
| 0x003D5D8C | 0x00199824 | Host T has no grounded map/header name; callbacks 0x0019992C, 0x00199A30 have no real map/header method name. |
| 0x003D5D9C | 0x0019B674 | Host T has no grounded map/header name; callbacks 0x0019B8F8, 0x0019B984 have no real map/header method name. |
| 0x003D5DAC | 0x001A1474 | Host T has no grounded map/header name; callbacks 0x001A13AC, 0x001A1420 have no real map/header method name. |
| 0x003D5DBC | 0x002D6078 | Host T has no grounded map/header name; callback 0x002D606C has no real map/header method name. |
| 0x003D5DCC | 0x002D6388 | Host T has no grounded map/header name; callback 0x002D62F8 has no real map/header method name. |
| 0x003D5DDC | 0x002F87D8 | Host T has no grounded map/header name; callbacks 0x002F87B4, 0x002F87D4 have no real map/header method name. |
| 0x003D5DEC | 0x00304138 | Host T has no grounded map/header name; callbacks 0x003040F8, 0x00304334 have no real map/header method name. |
| 0x003D5DFC | 0x0030C0EC | Host T has no grounded map/header name; callback 0x0030C1F4 has no real map/header method name. |
| 0x003D5E0C | 0x003129A0 | Host T has no grounded map/header name; callback 0x00312AF8 has no real map/header method name. |
| 0x003D5E1C | 0x003153FC | Garigari is grounded by the existing specialization and host table, but callback 0x00315598 is only fn_00315598 and Garigari.h declares no real name for it. |
| 0x003D5E2C | 0x003166A0 | Host T has no grounded map/header name; callback 0x00316704 has no real map/header method name. |
| 0x003D5E4C | 0x00320420 | Host T has no grounded map/header name; callback 0x0032081C has no real map/header method name. |
| 0x003D5E5C | 0x00322978 | Host T has no grounded map/header name; callbacks 0x00321FFC, 0x00322144 have no real map/header method name. |
| 0x003D5E6C | 0x0032387C | Host T has no grounded map/header name; callback 0x00323814 has no real map/header method name. |
| 0x003D5E7C | 0x00323C78 | Host T has no grounded map/header name; callback 0x00323DB8 has no real map/header method name. |
| 0x003D5E8C | 0x001027D0 | al::SaveDataDirector is grounded by its named constructor, but callback 0x001D1730 is unnamed and alSaveDataDirector.h declares no callback name. |
| 0x003D5E9C | 0x0027E0A4 | al::AreaObj is grounded by its named init method, but callbacks 0x001EA49C / 0x001EA4C8 are raw fn_ names and alAreaObj.h declares no real names for them. |

## Integrator intake limit

The installed integrator's `rank_column_changes` compares the union of old and
new row starts and treats a missing row as rank None. It therefore flags 15
starts in these five data partitions: five removed U rows and ten added U rows.
No row present in both maps changes rank, and all added/removed rows are data
at U. The resulting intake rejection would be "lanes never set ranks; the
branch changes rank cells in map.csv". This is a row-partition handling issue,
not a function-rank change. The operator must resolve that intake limit before
the integrator can accept these independently evidenced data partitions.
This branch does not change the integrator or its rank guard.

## Verification

On 2026-10-02, after sourcing development_environment.sh, the normal
`python make.py eu` build passed with the repository's unchanged compiler
configuration (ARMCC 4.1/791 for Game and al). `python tools/check.py <symbol>`
printed "Still matching" for each of the ten new function symbols above.
`python tools/check.py -q -w` completed in 1019 seconds. Its .changes file
contained three changes, all on previously non-exact rows: CalendarTime's
constructor M to U, sPhotoScenarioNames U to m, and sPlacementCategoryEntries
U to m. There were zero O-to-other changes; all 2,193 baseline O functions,
83,268 complete bytes, were preserved.

The committed map was restored after checking. A column audit confirms every
function's original Start, Pool, End, Section, Rank, Type and SectionName;
only the ten listed function Symbol fields differ. The five table partitions
remain U. All ten moved raw definitions and their unused vtable externs are
absent from Factory. The five generated vtable sections are each 16 bytes and
every clone relocates against its own mangled vtable symbol with addend 8.
This moves five instances / ten already-exact functions / 520 complete bytes;
it adds no new exact credit. Sixty-six instances remain blocked as listed above,
and AppearStep was already fully on the template.

The checked source hashes (SHA-256) are:

| Source | Hash |
| --- | --- |
| Game/backup/src/Enemy/BlockDragonGenerator.cpp | 2b1c813573d3056fa872651659603aa01884c9c21e58f0baef20b4354215be6f |
| Game/backup/src/Functor/FunctorV0M.cpp | 39f04d4755763a456d58c2c50407b7afb2c39eea0534cedd11c012afcc2795e6 |
| lib/al/src/LiveActor/alLiveActor.cpp | 2470905721271ae6503e2a3dabe334e877ebbe190bd84da63738426792241345 |
