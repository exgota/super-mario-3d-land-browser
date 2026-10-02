# Player effect update, 0x00308C6C: ABI prerequisite

Status: blocked before source trials; zero exact or non-matching credit.
Branch: `dot/root-308c6c`.
Frozen base: `e2336037d4f709acc082b3322daedadf9500564a`.
Target interval: `[0x00308C6C, 0x00308E8C)`, 544 bytes; pool at `0x00308E64`.
The base row is U and unnamed. No Game/lib body or existing target report was found.

## Required shared declaration

The target calls `fn_0026E1DC()` and dispatches table slot +0x00 on its result;
the result of that virtual call is a float consumed by vector scaling.
Two accepted Factory sources already declare the same C-linkage import using
incompatible translation-unit-private types:

- `Game/backup/src/Factory/fn_001BAD90.cpp:22`: anonymous `Limit*` return;
  its table has a float-returning method at +0x38. The root is O, 248 bytes.
- `Game/backup/src/Factory/fn_00171BE0.cpp:41`: anonymous `Settings*` return;
  its table has float-returning methods at +0x394 and +0x398. The root is O,
  252 bytes.

These are distinct C++ types, even though each observed ABI begins with a table
pointer. No shared declaration of the getter or provider exists in Game/lib.
Another anonymous provider type, a `void*` return, or a cast would hide the
incompatible existing contract. Repairing accepted Factory declarations and
shared private types is outside this lane's authorized scope.

The target also calls the existing mapped `sead::Vector3CalcCtr<float>` add
and multScalar symbols at `0x0027CB48` and `0x0027CC64`. Both Factory sources
import those symbols with their distinct anonymous `Vec3` types; any future
shared declaration repair must preserve the actual `nn::math::VEC3` reference
ABI named by the rows and reconcile all existing imports.

## Module and layout evidence

`Game/backup/include/Player/Player.h` places `PlayerProperty*` at +0x00,
`PlayerFigureDirector*` at +0x40, and unresolved objects at +0x30, +0x60 and
+0x70, exactly the fields used here. `PlayerProperty.h` places translation
at +0x00 and up vector at +0x18. The root belongs in Game's Player family,
using its configured ARMCC 4.1/791, rather than the SDK or Actor modules.
The sole direct caller found is `0x00309010`, at callsite `0x00309308`;
it passes the same Player object used for PlayerTrigger and PlayerActionGraph.

The target reads a one-byte flag at +0x05 of the +0x70 object. The +0x30
object dispatches boolean queries at table +0x04 and +0x08. The +0x60 object
uses string-taking operations at +0x00, +0x04 and +0x08. Figure state at +0x00
of the +0x40 object selects states 1 and 6. Local string literals are `Land`,
`FallSplash` and `RaccoonDogWhite`; local float 0.5 scales the up displacement
for state 1. These observations do not establish complete provider classes.

The getter at `0x0026E1DC` returns the pointer reached through +0x60 then
+0x10 from the object at `0x003EFEE4`. That global already has a map row
`[0x003EFEE4, 0x003EFF50)`. The target's external direct callees all have
existing rows; its own data references stay inside its mapped literal pool.
No missing-row or invented-table-extent prerequisite was found.

## Verification and limits

The owner's EU code SHA-256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
Preflight used the unchanged executable, accepted source, map and headers.
No source form was compiled; neither `python make.py eu` nor
`python tools/check.py ... --object ...` was run because the shared ABI
prerequisite was identified first. No source/header, Factory, map, rank,
ledger, tool, configuration, game-data or disassembly changes are submitted.
Only this evidence note is proposed. The complete map remains byte-identical
to the frozen base. No object or object-provenance claim is made.

Reopen after the shared getter/provider and vector imports have one compatible
established contract that this lane may reuse without changing accepted
Factory sources. Integrator review and canonical checking remain required.
