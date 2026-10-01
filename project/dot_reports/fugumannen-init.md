# Fugumannen initialization and EnemyStateBlowDown identity

## Status

Target: `Fugumannen::init(const al::ActorInitInfo&)`, 0x0011A498–0x0011A5B4 (284 bytes, including the literal pool beginning at 0x0011A580). The original full EU image has SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Investigation started from main commit `9fba58fa1db1aaabad80d2b77d87139bd83b80e6`.

The canonical project checker verifies five existing Fugumannen functions, totaling 676 complete bytes, against ARMCC 4.1/791 output from committed source. Init itself passes for all 284 bytes. No function body needed a change. The branch corrects independently recovered header layouts and records five local-only import identities. Exact commands and outcomes are below. Move/Move2 nerve execution and the static initializer remain mismatching and are not included in this result.

## Import identities required by the existing source

These four existing function rows have blank symbols in the upstream snapshot. Their intervals must not change. The names below are the exact mangled imports emitted by the current project object.

| Original start | Pool | Original end | Proposed identity |
| --- | --- | --- | --- |
| 0x0027B54C | 0x0027B5DC | 0x0027B60C | `_ZN18EnemyStateBlowDownC1EPN2al9LiveActorEP23EnemyStateBlowDownParamPKci` |
| 0x0027D238 | 0x0027D244 | 0x0027D24C | `_ZN2al10tryGetArg0EPfRKNS_13ActorInitInfoE` |
| 0x001CACB4 | none | 0x001CAD00 | `_ZN2al14initNerveStateEPNS_9IUseNerveEPNS_14NerveStateBaseEPKNS_5NerveEPKc` |
| 0x0027F244 | none | 0x0027F2C0 | `_ZN2al11startActionEPNS_9LiveActorEPKc` |

The original rows are:

```csv
0x0027B54C,0x0027B5DC,0x0027B60C,          ,U,f,,
0x0027D238,0x0027D244,0x0027D24C,          ,U,f,,
0x001CACB4,          ,0x001CAD00,          ,U,f,,
0x0027F244,          ,0x0027F2C0,          ,U,f,,
```

No map, boundary, rank, tool, compiler flag, ledger, or STATE change is part of this proposal. The integrator owns reviewing and applying these identities.

### EnemyStateBlowDown constructor: independent evidence

Fugumannen::init calls 0x0027B54C at 0x0011A514 after allocating 0x28 bytes. Arguments are `(allocation, this, nullptr, "SwimBlowDown", 1)`, with the fifth argument at stack offset zero.

An independent named caller, Togezo::init at 0x0030B60C, allocates the same 0x28 bytes and calls 0x0027B54C at 0x0030B714 with `(allocation, this, nullptr, nullptr, 0)`. The result is stored at Togezo+0x68 and registered as its BlowDown state at 0x0030B754. This agrees with the retained public game-source declaration and is independent of Fugumannen's own desired relocation.

The callee's own behavior confirms the class family. At 0x0027B568 it invokes the already identified `al::NerveStateBase` constructor with the local Shift-JIS name 敵吹き飛び状態 ("enemy blow-away state"). It then stores the host at +0x0C, installs vptr 0x003CE090, stores the optional parameter pointer at +0x10, copies the unit-Z vector (0, 0, 1) from `sead::Vector3<float>::ez` at 0x004305E0 into +0x14..+0x1C, chooses the supplied animation name or its local `"BlowDown"` default for +0x20, and clears +0x24. A missing parameter pointer selects global 0x0043005C when the final integer is nonzero, or 0x00430040 otherwise. Finally it calls `al::NerveExecutor::initNerve` with nerve object 0x003F2FD0 and zero step count.

There are 41 direct ARM branch/call references to this constructor in the executable. No constructor implementation or guessed global import is needed merely to establish this existing constructor import.

### Other imports

- `al::tryGetArg0(float*, const ActorInitInfo&)`: Fugumannen passes the float field at +0x60. The function at 0x0027D238 loads ActorInitInfo's first pointer and passes its own local `"Arg0"` string to 0x00250DC4. That helper reads an integer placement argument, rejects the absent/-1 case, and converts the signed integer to a float before writing the output. This differentiates the float overload from the integer overload.
- `al::initNerveState(IUseNerve*, NerveStateBase*, const Nerve*, const char*)`: Fugumannen passes `(this, new state, BlowDown nerve, "state:BlowDown")`. The function invokes the state's vtable slot +0x0C (`init`), invokes the host's `getNerveKeeper` slot, loads the keeper's controller at +0x10, and forwards the three state registration values. Its original linker-inlined continuation begins at 0x001CAD00. The existing boundary is left unchanged.
- `al::startAction(LiveActor*, const char*)`: called at 0x0011A538 with `"SwimWait"`. The callee reads LiveActor's action keeper at +0x1C, checks current action state, and forwards the same action name to the model/animation/action helpers at 0x0024FD4C, 0x0024FD08, 0x00268804, 0x00265128, 0x0024FCB8, and 0x0024FC68. This agrees with the existing shared declaration and broader use of the action helper.

### Additional import for the adjacent attackSensor function

`_ZN2al9sendMsg41EPNS_9HitSensorES1_` is 0x0027D548. Original row:

```csv
0x0027D548,          ,0x0027D564,          ,U,f,,
```

The retail body independently establishes the numeric message identity: it loads the first HitSensor's actor owner from +0x28 and tail-calls its virtual `receiveMsg` slot +0x34 with message 0x29 (41), the second sensor, and the first sensor. This is the only additional identity needed to check attackSensor.

## Layout findings

Fugumannen is 0x68 bytes: the existing LiveActor/MapObjActor base is 0x60, followed by `float mRailMoveSpeed` at +0x60 and `EnemyStateBlowDown* mStateBlowDown` at +0x64. The constructor at 0x0011A5B4 writes 10.0 and null to those fields. init writes the argument into +0x60 and the state into +0x64; receiveMsg independently reads +0x64.

EnemyStateBlowDown is 0x28 bytes. The inherited NerveExecutor keeper pointer is +0x04, NerveStateBase dead flag +0x08, and ActorStateBase host +0x0C. Its own fields are the parameter pointer +0x10, a three-float direction +0x14, animation name +0x20, and a **message ID**, not a pointer, at +0x24. Both 0x0027B750 and 0x0027B848 store the incoming `u32 msg` into +0x24. The death-effect wrapper loads it at 0x002CD43C and the continuation passes it to predicates including `al::isMsgPlayerInvincibleAttack` and `al::isMsgPlayerTailAttack`.

The retail vtable address point is 0x003CE090. Its slots are `getNerveKeeper` 0x00331520, two null destructor slots, `NerveStateBase::init` 0x001E2DA0, an overridden `appear` at 0x00189118, `NerveStateBase::kill` 0x001E2DA4, `NerveStateBase::update` 0x001E2DBC, and `NerveStateBase::control` 0x001E2DF0. Therefore EnemyStateBlowDown's retained declaration is missing its `appear` override. The override uses host +0x0C, parameter +0x10 and vector +0x14, sets its nerve and clears the dead flag.

The constructor copies its vector from 0x004305E0. Default parameter data at 0x00430040 and 0x0043005C and that vector lie outside code.bin's initialized range and have no matching rows in the current map. An independent static initializer at 0x0038A614–0x0038A6C0 references both parameter addresses and the same nerve object 0x003F2FD0. It establishes both 0x1C-byte parameter objects as five floats followed by two integers:

| Address | +0x00 float | +0x04 float | +0x08 float | +0x0C float | +0x10 float | +0x14 int | +0x18 int |
| --- | --- | --- | --- | --- | --- | --- | --- |
| 0x00430040 | 3.5 | 0.98 | 0.87 | 10.0 | 43.0 | 17 | 10 |
| 0x0043005C | 1.0 | 0.98 | 0.87 | 3.0 | 20.0 | 27 | 10 |

The stores at 0x0038A654, 0x0038A65C, 0x0038A674 and 0x0038A688 use small integer constants, not float representations. The same initializer sets the nerve object to vtable address point 0x003C080C, whose execute target is 0x00368398. That execute function independently loads parameter +0x14 at 0x003683F0 and passes it to `al::isStep(IUseNerve*, int)`; it loads +0x18 at 0x00368408 and passes it to `al::isGreaterStep(const IUseNerve*, int)`. This confirms the integer interpretation from runtime use, not only initializer constants. Accordingly `EnemyStateBlowDownParam.h` changes the last two member/constructor types from float to int and asserts a 0x1C size. Semantic field names remain unassigned. The missing original BSS rows remain a checking blocker for a full constructor implementation even though parameter initialization is now recovered. No address/boundary is invented here. The ABI vtable header lies eight bytes before its address point, so naming the current data row as a complete C++ vtable without resolving that boundary would also be unsound.

## Checker results

The parent rebuilt the canonical project object after committing the Fugumannen and EnemyStateBlowDown header corrections in `9dfff01`. It reran the checker after the parameter-header correction in `dbada0b`; that header is not a transitive dependency of Fugumannen.cpp. All five functions pass with the evidenced import names applied to existing map rows locally, leaving all original boundaries unchanged.

```sh
. ./development_environment.sh
python tools/check.py _ZN10Fugumannen4initERKN2al13ActorInitInfoE --object build/eu/obj/Game/backup/src/Enemy/Fugumannen.o
python tools/check.py _ZN10Fugumannen10receiveMsgEjPN2al9HitSensorES2_ --object build/eu/obj/Game/backup/src/Enemy/Fugumannen.o
python tools/check.py _ZN10FugumannenC1ERKN4sead14SafeStringBaseIcEE --object build/eu/obj/Game/backup/src/Enemy/Fugumannen.o
python tools/check.py _ZNK13NrvFugumannen21FugumannenNrvBlowDown7executeEPN2al11NerveKeeperE --object build/eu/obj/Game/backup/src/Enemy/Fugumannen.o
python tools/check.py _ZN10Fugumannen12attackSensorEPN2al9HitSensorES2_ --object build/eu/obj/Game/backup/src/Enemy/Fugumannen.o
```

Exact outputs in command order:

```text
O -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
U -> O: The complete source-generated function interval matches byte for byte.
```

| Function | Original interval | Complete bytes |
| --- | --- | ---: |
| init | 0x0011A498–0x0011A5B4 | 284 |
| receiveMsg | 0x0011A38C–0x0011A43C | 176 |
| constructor | 0x0011A5B4–0x0011A5F4 | 64 |
| BlowDown nerve execute | 0x0035113C–0x00351178 | 60 |
| attackSensor | 0x0011A43C–0x0011A498 | 92 |
| Total | | 676 |

Init's first check before the header correction reported `U -> O: The complete source-generated function interval matches byte for byte.` Its repeated O→O result confirms that the corrected declaration preserves its exact source-generated bytes. All five source bodies were already in the repository; the blocked init needed sound callee identities rather than a control-flow rewrite.

The three retained header corrections are: assert Fugumannen size 0x68; assert EnemyStateBlowDown size 0x28, declare the retail appear override, and change +0x24 from a pointer to a u32 message ID; assert EnemyStateBlowDownParam size 0x1C and change its last two field/constructor argument types to int. A separate ARMCC 4.1/791 header/constructor probe compiled the final parameter declaration successfully (exit 0), using the same compiler and project flags plus an explicit compiler-system-header include directory. That ancillary probe verifies C++ syntax/layout only and is not a game-function matching claim. Its source, command and log remain ignored under `build/fugumannen_header_probe/`.

The map names and ranks are intentionally not part of the branch. The owner must review/register these identities and rerun the same project checks when integrating the headers/report. No inline assembly, hand-built instruction bytes, target modifications, or compiler-flag changes are used.

## Current canonical checker revalidation

On 2026-10-01, a clean worktree of main `a360142fbddd9ab875ab68314c55445ade05fd00` was reconstructed and its entire Git tree verified. The proposed source/header edits were applied and committed as local verification checkpoint `eace3c0`. The unchanged current project build (`python make.py eu`, ARMCC 4.1/791 for game code) compiled, linked, and exported successfully. The current canonical `tools/check.py --object` commands were then rerun on those freshly built objects. No checker/tool changes were made. The same independently evidenced local-only map identities were supplied; no existing function interval changed.

All five listed functions independently report `U -> O: The complete source-generated function interval matches byte for byte.` The total remains 676 bytes. These are existing bodies reverified with recovered names and corrected layout headers, not five newly authored implementations.
