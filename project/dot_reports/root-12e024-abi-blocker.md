# KoopaPillar break state: existing import ABI blocker

Base: `8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65`.
Branch: `dot/root-12e024`.
Fresh public main was confirmed at that base on 2026-10-02 at 20:27 UTC.
Target: `[0x0012E024, 0x0012E234)`, 528 bytes including its pool; base rank U.
Result: parked before candidate compilation; no matching claim.

## Blocker

The item-type 2 branch calls `al::getQuat(this)` and passes its result unchanged
as the third argument to existing accepted `fn_002CCEAC` at `0x002CCEAC`.
The clean shared declaration in `alActorPoseKeeper.h` returns `const sead::Quatf&`.
The only declaration or definition of `fn_002CCEAC` in Game/lib sources is
`Game/backup/src/Factory/fn_002CCEAC.cpp:39`:

```cpp
extern "C" void fn_002CCEAC(const void* key, const void* arg, const Value* value)
```

Here `Value` belongs to that translation unit's anonymous namespace and contains
four floats. There is no shared canonical type or declaration for another source
to use. Matching its storage shape does not establish a compatible C++ type.

The coordinator explicitly excludes Factory repair, duplicate private `Value`
types, nominally compatible replacement imports, and casts hiding this conflict.
Consequently no implementation or import declaration was added. Integrator work
must first establish a shared quaternion-compatible API for the existing accepted
callee, preserve its exact result, and publish that prerequisite before retrying
this target. A new source can then use the accepted KoopaPillar header directly.

## Other evidence and limits

The existing KoopaPillar constructor/init establish fields at 0x60, 0x68, 0x6C,
0x70 and 0x74 used by this state. The state invokes the break model, handles the
three item-type cases, starts the Break action, and branches on mArg1/mPillarType.
Both direct data references have actual rows: `[0x003BE514, 0x003BE524)` holds the
Break name, and `[0x003F293C, 0x003F2940)` is the next nerve instance. No missing
row or guessed data boundary blocks this target.

Source forms attempted: 0. Builds and canonical checks performed: 0.
No source or header changed; accepted KoopaPillar init remains untouched.
The required init dependency recheck applies after a candidate normal build;
no candidate was produced, so no unnecessary dependency/full audit was run.
The complete map remains byte-identical to the base. No rank, ledger, tools,
configuration, Factory, game data, or disassembly changes are proposed.
Owner code SHA-256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
