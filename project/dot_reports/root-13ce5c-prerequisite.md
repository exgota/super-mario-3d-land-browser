# NeedleRoller initializer: shared-contract prerequisite

Target: unnamed U function `[0x0013CE5C, 0x0013D090)`, 564 bytes,
with literal pool beginning at `0x0013D048`.
Base: `3d69bf00a769676c77f07465163def56a559b48f`.
Branch: `dot/root-13ce5c`. Result: excluded before source attempts;
zero compiled forms, zero exact bytes, no NonMatching source adopted.

## Blocking accepted import

The target calls `0x0026D5E0(actor, actor + 0xAC, "NeedleRoller")`.
Its existing declaration is `fn_0026D5E0(Actor*, Joint**, const char*)`
in `Game/backup/src/Factory/fn_001794C0.cpp:36`. Both parameter types
belong to that translation unit's anonymous namespace. The caller at
`0x001794C0` is accepted rank O on this base. A declaration using a new
NeedleRoller type would not share those C++ types merely because the
external C symbol name and ARM pointer registers agree.

The callee obtains a joint from the actor's model keeper at `+0x28`,
then forwards the actor, output address and result to `0x0023FDEC`.
That establishes the calling shape, not a safe public type identity.
Prerequisite: the owner of the accepted Factory source must establish a
common, evidenced import contract and reconcile its caller. This lane
does not edit accepted Factory code, introduce a wrapper, or duplicate
its private class definitions to bypass that prerequisite.

## Binary and data preflight

Constructor `0x0013D0FC` calls the LiveActor constructor and installs
dispatch address `0x003C73E4`, whose existing row ends at `0x003C747C`.
Its `+4` slot points to this target, and `+8` points to `0x0013CDA4`.
The target's archive and joint strings are "NeedleRoller"; its object
variant test uses "NeedleRollerFreeMove", followed by action "Wait".
These literals can be ordinary C++ strings within the target's pool.

All target data references have rows: SafeString dispatch
`0x003D9C3C` is `+8` in named vtable row `[0x003D9C34,0x003D9C50)`;
the two nerve objects have separate four-byte rows at `0x003F2A18`
and `0x003F2A20`. No missing-row request or inferred table extension
is necessary. Every direct call target also has a function row.

The initializer copies the quaternion to `+0x68`, creates a key-pose
keeper at `+0x64`, copies translation to `+0x78`, computes direction
vectors at `+0x84` and `+0x90`, orders the projected next-key displacement
and zero into `+0xA0/+0xA4`, and sets motion speed at `+0xA8`.
It optionally replaces a sensor-position Y component from argument 7,
connects execution, then invokes the actor's virtual slot `+0x10`.

## Verification limits

Owner code SHA256 verified:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No `make.py` or `tools/check.py` run: the explicit accepted-private-contract
exclusion was reached before C++ generation. No object/provenance exists.
No map edits, scratch symbol names, replay, or preservation gate were used.
Only this report is committed; ranks, tools, data, and accepted sources
remain unchanged. Revisit after the common import contract lands on main.
