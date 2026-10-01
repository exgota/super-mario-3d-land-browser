# Togezo initialization and walker parameter layouts

## Status

Target: `Togezo::init(const al::ActorInitInfo&)`, `0x0030B60C–0x0030B7D4`, 456 complete bytes, including its literal pool at `0x0030B770`. Investigation used the verified EU executable with SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64` and started from main `9fba58fa1db1aaabad80d2b77d87139bd83b80e6`. A refresh to `a360142fbddd9ab875ab68314c55445ade05fd00` confirmed that this target remained rank U and its source unchanged.

The canonical project checker verifies eight complete Togezo functions, totaling **1,204 bytes**, from committed source `5758521` on refreshed main `a360142fbddd9ab875ab68314c55445ade05fd00`. Init itself passes all 456 bytes. The source restores complete walker-parameter layouts, allows the three ordinary static parameter objects to share their evidenced BSS allocation, and gives the three action-name literals source-specific data sections. It also corrects an existing attack-state behavior bug: the movement helper must run on every frame, not only the first frame.

All eight functions were rebuilt and independently rechecked against the refreshed main checker, each reporting U -> O. The earlier pinned `9fba58f` checker also passed them from source `f73c0d4`. Both runs used independently reviewed import identities and the new BSS row applied locally. The integrator must review those identities and rerun the canonical checks before accepting the proposal; map names, ranks, and BSS registration are not part of this source branch.

These are canonical object-check results, not a full linked-game pass. The refreshed project build compiled the objects but its compact scaffold link failed on unrelated Fugumannen vtable helper imports and the zero-vector dependency after earlier roots were enrolled. That linker failure does not alter the eight independently passing canonical-object checks; no full-link success is claimed.

## Independent data identity

The already named translation-unit static initializer `__sti___10_Togezo_cpp` at `0x0037FEC0–0x00380040` establishes the three globals independently of the init function's desired relocations:

| Object | Address | Size | Evidence |
| --- | --- | --- | --- |
| `sTogezoWalkerStateParam` | `0x0042FB10` | `0x20` | At `0x0037FF40`, `vstmia r0,{s0–s7}` writes eight floats |
| `sTogezoWalkerStateWanderParam` | `0x0042FB30` | `0x6C` | `0x0037FF68` calls its constructor with `(30,90,0.7,4.0,10.0,"Walk","Wait")` |
| `sTogezoWalkerStateChaseParam` | `0x0042FB9C` | `0x70` | `0x0037FFA4` calls its constructor with `(false,true,1.3,30.0,150.0,3.0,20.0,"Run","Wait")` |

The first object's eight values, in member order, are `4.0, 0.98, 0.85, 250.0, 700.0, 180.0, 70.0, 150.0`. The independent constructor bodies and nested fixed-string constructor below establish the sizes. Together they cover `0x0042FB10–0x0042FC0C` without gaps or overlaps. These addresses lie beyond code.bin's initialized range; their identities and extents come from runtime initialization and constructor storage, not a guessed binary data pattern.

Init independently agrees: it loads WanderParam at `0x0030B68C` and computes WalkerStateParam by subtracting `0x20`; it loads ChaseParam at `0x0030B6CC` and computes WalkerStateParam by subtracting `0x8C`. With the old separate `staticd` sections, the compiler emitted a second literal load for each WalkerStateParam reference and an extra pool word, yielding 460 bytes rather than 456. Keeping them as ordinary static variables allows the normal `.bss.Togezo.cpp` allocation to express their evidenced adjacency without pointer arithmetic in source, assembly, or compiler-flag changes.

The canonical compiler object confirms `.bss.Togezo.cpp` as an `SHT_NOBITS` section of size `0xFC`, containing the three symbols at offsets `0x00,0x20,0x8C` with sizes `0x20,0x6C,0x70`.

The existing upstream map has no rows for these BSS objects. An integration-only new data row can describe the complete shared allocation at `0x0042FB10–0x0042FC0C`, with section name `.bss.Togezo.cpp`, using the compiled section and offsets confirmed above. No existing boundary needs changing. This proposal does not commit a map edit.

## Parameter types and layout

`WalkerStateWanderParam`'s constructor is `0x001A6344–0x001A63A0` (literal pool `0x001A639C`). It stores integer arguments at offsets `0x00,0x04` using `stm`; stores the three float arguments at `0x08,0x0C,0x10` using `vstmia`; and constructs two inline fixed strings at `0x14` and `0x40`.

`WalkerStateChaseParam`'s constructor is `0x001A1580–0x001A15F8` (literal pool `0x001A15F4`). Its fields are:

| Offset | Type | Constructor input |
| --- | --- | --- |
| `0x00` | float | first float |
| `0x04` | int | second float, converted with `vcvt.s32.f32` |
| `0x08` | int | third float, converted with `vcvt.s32.f32` |
| `0x0C` | float | fourth float |
| `0x10` | int | fifth float, converted with `vcvt.s32.f32` |
| `0x14` | bool | first bool |
| `0x15` | bool | second bool |
| `0x18` | `sead::FixedSafeString<32>` | first action name |
| `0x44` | `sead::FixedSafeString<32>` | second action name |

The constructor still takes five floats; the change concerns stored member types, not its ABI. Both parameter constructors call `0x00252BF8` for each string. That callee sets capacity `0x20` at string offset `0x08`, sets the buffer pointer at offset `0x04` to object+`0x0C`, terminates the final buffer byte, and copies the supplied SafeString into the inline buffer. Thus each string occupies `0x0C+0x20 = 0x2C` bytes. The existing clean `sead::FixedSafeString<32>` declaration has precisely this representation. No external sead implementation was consulted.

An additional independent initializer, `__sti___20_WalkerStateChase_cpp` at `0x00388F28`, constructs the default ChaseParam at `0x0043026C` directly. It writes floats `2.0` and `4.0` at offsets `0x00` and `0x0C`, integer values `65,150,15` at `0x04,0x08,0x10`, false bytes at `0x14,0x15`, and invokes the same string constructor at `0x18` and `0x44`. This corroborates the corrected integer interpretation and complete string layout independently of the Togezo caller.

Size assertions now document `WalkerStateParam=0x20`, `WalkerStateWanderParam=0x6C`, `WalkerStateChaseParam=0x70`, and `Togezo=0x6C`. Togezo's constructor at `0x0030B860` clears its state pointers at `0x60,0x64,0x68`; init allocates state objects of `0x20,0x24,0x28` bytes and stores their pointers at those same offsets.

## Additional callee identities

These are existing unnamed function rows. Their original intervals remain unchanged:

| Start | Pool | End | Identity |
| --- | --- | --- | --- |
| `0x0027C04C` | none | `0x0027C05C` | `_ZN2al11getFrontPtrEPNS_9LiveActorE` |
| `0x0026B9D8` | `0x0026BA88` | `0x0026BAB0` | `_ZN17WalkerStateWanderC1EPN2al9LiveActorEPN4sead7Vector3IfEEPK16WalkerStateParamPK22WalkerStateWanderParam` |
| `0x001A6344` | `0x001A639C` | `0x001A63A0` | `_ZN22WalkerStateWanderParamC1EiifffPKcS1_` |
| `0x001A1580` | `0x001A15F4` | `0x001A15F8` | `_ZN21WalkerStateChaseParamC1EbbfffffPKcS1_` |

Only the first two are new direct imports of init. The parameter constructors matter to the separate static initializer, which is not claimed here. Its destructor registrations additionally identify empty nondeleting parameter destructors at `0x001A63A0` (Wander) and `0x001A15F8` (Chase).

The front-pointer helper at `0x0027C04C` loads LiveActor's pose keeper at `+0x14`, then tail-calls the keeper's virtual slot `+0x24`. This slot is independently identified in the existing named ActorPoseKeeperTFSV table at `0x003D6DF4`: its address point is `0x003D6DFC`, and slot `+0x24` contains the named `ActorPoseKeeperTFSV::getFrontPtr` at `0x001DB734`.

The Wander constructor calls the already named NerveStateBase constructor at `0x002684E0` with the Shift-JIS name クリボー型うろつき状態, stores host/front pointers at `+0x0C/+0x10`, clears its private helper pointer at `+0x14`, stores the Walker and Wander parameters at `+0x18/+0x1C`, initializes its nerve, supplies the default parameter at `0x00430314` only if the passed WanderParam is null, then allocates its helper and copies the actor translation into it. These stores agree with the retained WalkerStateWander declaration and with the allocation size `0x20` in Togezo.

EnemyStateBlowDown's constructor and `al::initNerveState` were independently identified in the Fugumannen report. Existing `fn_0027a1a0` and `fn_0027cf20` address-encoded imports already have mapped retail function rows. No new name is needed for them.

## Checker results and reproduction

The parent built committed source through `python make.py eu`, then ran the project checker on the canonical `build/eu/obj/Game/backup/src/Enemy/Togezo.o`. All eight checks pass on refreshed main `a360142fbddd9ab875ab68314c55445ade05fd00` after local source commit `5758521`:

| Function | Complete interval | Bytes | Final result |
| --- | --- | ---: | --- |
| init | `0x0030B60C–0x0030B7D4` | 456 | U -> O |
| constructor | `0x0030B860–0x0030B8A0` | 64 | U -> O |
| BlowDown nerve execute | `0x00349AF0–0x00349B2C` | 60 | U -> O |
| Chase nerve execute | `0x00349BE8–0x00349C28` | 64 | U -> O |
| Wander nerve execute | `0x00349C28–0x00349C80` | 88 | U -> O |
| Attack nerve execute | `0x00349D34–0x00349D9C` | 104 | U -> O |
| Search nerve execute | `0x00349B2C–0x00349BE8` | 188 | U -> O |
| Turn nerve execute | `0x00349C80–0x00349D34` | 180 | U -> O |
| **Total** | | **1,204** | |

Commands, in the same order as the table:

```sh
. ./development_environment.sh
python make.py eu
python tools/check.py _ZN6Togezo4initERKN2al13ActorInitInfoE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
python tools/check.py _ZN6TogezoC1ERKN4sead14SafeStringBaseIcEE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
python tools/check.py _ZNK9NrvTogezo17TogezoNrvBlowDown7executeEPN2al11NerveKeeperE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
python tools/check.py _ZNK9NrvTogezo14TogezoNrvChase7executeEPN2al11NerveKeeperE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
python tools/check.py _ZNK9NrvTogezo15TogezoNrvWander7executeEPN2al11NerveKeeperE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
python tools/check.py _ZNK9NrvTogezo15TogezoNrvAttack7executeEPN2al11NerveKeeperE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
python tools/check.py _ZNK9NrvTogezo15TogezoNrvSearch7executeEPN2al11NerveKeeperE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
python tools/check.py _ZNK9NrvTogezo13TogezoNrvTurn7executeEPN2al11NerveKeeperE --object build/eu/obj/Game/backup/src/Enemy/Togezo.o
```

Exact final checker output, in that order:

```text
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
```

Init's initial successful check after parameter/group recovery in `b862958` reported `M -> O: The complete source-generated function interval matches byte for byte.` On the original pinned checker, the subsequent final `f73c0d4` run gave O -> O for init/constructor/BlowDown/Chase/Wander, M -> O for Attack, and U -> O for Search/Turn. The first five also passed after `f7b20eb`, before the final action-string declarations. The refreshed-main build/check is the independent all-U -> O run reported in the table above.

Only `exeAttack` changes control flow. The other seven functions reuse their retained bodies, with data layout/identity corrections and symbolic action-name references where needed. The translation-unit static initializer and unmapped out-of-line exe methods are not included in this eight-function result. No assembly, machine-code arrays, edited objects, compiler-flag changes, or target-byte modifications were used. No game data, map, rank, tool, boundary, ledger, or compiler configuration change is part of the source proposal.

## Adjacent attack-state correction

The retained `Togezo::exeAttack` source incorrectly called `fn_00258774(this, &sTogezoWalkerStateParam)` only on the first nerve step. In retail `TogezoNrvAttack::execute`, the conditional branch at `0x00349D48` skips only `startAction("AttackSuccess")` and lands at `0x00349D58`, the parameter load for `fn_00258774`; its call at `0x00349D60` therefore runs on every step. The proposed C++ moves that helper call outside the first-step conditional. This is a behavioral correction established directly by the retail control-flow graph. The correction was committed in `f7b20eb`; the complete 104-byte Attack nerve executor subsequently passes from source `f73c0d4` and independently from refreshed-main source `5758521`, with the source-specific action-string identities below. Its target-size difference and missing NOP disappear naturally under ARMCC after this control-flow correction.


## Additional identities for neighboring nerve checks

`_ZN2al8getFrontEPKNS_9LiveActorE` is the existing function interval `0x0027D520–0x0027D530`. Like getFrontPtr, it loads the actor's pose keeper at `+0x14`, then uses virtual slot `+0x0C`. The independent named ActorPoseKeeperTFSV vtable entry at `0x003D6E08` contains `ActorPoseKeeperTFSV::getFront` at `0x00334F80`.

`_ZN2al11isActionEndEPKNS_9LiveActorE` is the existing function interval `0x0027CE60–0x0027CF20`. Its body tests the actor's model animation-controller pointers in order (model-keeper pointer at actor `+0x28`, then controller slots `0x20,0x28,0x2C,0x30,0x24,0x34`). It dispatches to the corresponding channel completion helper, or returns true if no channel is present. The terminal helpers at `0x003307E8`, `0x0024EB8C`, and `0x003306FC` compare end-frame at frame-control `+0x18` against current-frame at `+0x08` and return true once the end frame is reached. An independent named caller, `al::NrvBreakModel::BreakModelNrvBreak::execute` at `0x0035A068`, calls this function at `0x0035A074` and then conditionally invokes virtual kill, agreeing with the retained `BreakModel::exeBreak` source.

The retail action strings are `"Turn"` at `0x003BC950–0x003BC958`, `"Search"` at `0x003BC958–0x003BC960`, and `"AttackSuccess"` at `0x003BC960–0x003BC970`. These are three existing data rows. The first canonical object pooled those literals into a generic `.constdata` section with offsets `0,8,16`, initialized size 30 bytes excluding final retail alignment padding. The final proposal instead gives each literal a source-specific symbolic constant: `sTogezoTurnAction`, `sTogezoSearchAction`, and `sTogezoAttackAction`, in `.constdata_` sections with the same suffixes. The three existing retail rows receive those names during local checking. This avoids assigning a globally ambiguous `.constdata` base to all compiler objects. The arrays contain only their ordinary null-terminated action strings; no machine-code bytes or absolute addresses enter C++ source. Source references replace the three original `startAction` literal arguments without changing their strings. All eight final checks pass with this representation; no generic `.constdata` map alias is used.


## Local integration identities

The final eight checks require these additional local identities; existing named dependencies remain unchanged:

```csv
0x0026B9D8,0x0026BA88,0x0026BAB0,          ,U,f,_ZN17WalkerStateWanderC1EPN2al9LiveActorEPN4sead7Vector3IfEEPK16WalkerStateParamPK22WalkerStateWanderParam,
0x0027C04C,          ,0x0027C05C,          ,U,f,_ZN2al11getFrontPtrEPNS_9LiveActorE,
0x0027D520,          ,0x0027D530,          ,U,f,_ZN2al8getFrontEPKNS_9LiveActorE,
0x0027CE60,          ,0x0027CF20,          ,U,f,_ZN2al11isActionEndEPKNS_9LiveActorE,
0x003BC950,          ,0x003BC958,          ,U,dc,sTogezoTurnAction,
0x003BC958,          ,0x003BC960,          ,U,dc,sTogezoSearchAction,
0x003BC960,          ,0x003BC970,          ,U,dc,sTogezoAttackAction,
0x0042FB10,          ,0x0042FC0C,          ,U,db,dat_0042FB10,.bss.Togezo.cpp
```

The last row is new and describes an independently evidenced BSS allocation, not a changed boundary. The prior seven keep the original function/data intervals and types. The three action rows map naturally to `.constdata_` plus their symbolic names. The common constructor/action registrations inherited from the Fugumannen investigation are also required:

```csv
0x0027B54C,0x0027B5DC,0x0027B60C,          ,U,f,_ZN18EnemyStateBlowDownC1EPN2al9LiveActorEP23EnemyStateBlowDownParamPKci,
0x001CACB4,          ,0x001CAD00,          ,U,f,_ZN2al14initNerveStateEPNS_9IUseNerveEPNS_14NerveStateBaseEPKNS_5NerveEPKc,
0x0027F244,          ,0x0027F2C0,          ,U,f,_ZN2al11startActionEPNS_9LiveActorEPKc,
```

These rows are documented as integration evidence only. The owner maintains the actual map and acceptance ranks. The parameter-constructor identities described earlier were independently recovered but are not required to isolate/check these eight functions.

## Independent branch context

The final proposed files were also rechecked without the unrelated Fugumannen header edits. Local checkpoint `32419ca` restores those three headers to exact main `a360142` content, then rebuilds the Togezo canonical object through the unchanged project build. All eight commands above report `O -> O: The complete source-generated function interval matches byte for byte.` The proposal therefore does not require applying the Fugumannen header branch; only the independently reviewed import identities are shared. The unchanged cube files in that verification worktree are not Togezo build inputs. The same unrelated compact-scaffold link blockers remain; no complete game link or runtime result is claimed.
