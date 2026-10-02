# Collect-coin counter transition at 00187AB8

Canonical result: one exact function, 516 bytes including its literal pool.
Branch: `dot/root-187ab8`; integrator acceptance pending.
Base: `eeaca9ad7a606674b428b29630da497031ff913d`.
Final source commit: `f5ba872fe7b1dc53d5a9de73c2e2640e322204c7`.
Source: `Game/backup/src/Layout/CounterCollectCoin.cpp`.
Source SHA256: `f447b0165bd2187b9abd6db43c8c732ed2048e887e5c03719396740ec7115e26`.
Interval: `[00187AB8,00187CBC)`, pool begins `00187C74`.
The base row was U. Local and origin dot refs had no address/source/report overlap.

## Verification

The original code SHA256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The final normal build used the existing Game configuration, ARMCC 4.1/791
through the installed wibo; SDK dependencies retain ARMCC 4.0/902.
No compiler flags, tools, configuration, function boundaries or target bytes changed.

Commands, run from the branch worktree:

```sh
. /workspace/scratch/73cdb2c524af/mario-dot/development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py _ZN18CounterCollectCoin7collectEi --object build/eu/obj/Game/backup/src/Layout/CounterCollectCoin.o
```

The build exited 0 and printed `Linking RE-Pepper.axf` and `Exporting code.bin`.
The canonical check exited 0 and printed:
`M -> O: The complete source-generated function interval matches byte for byte.`
Final canonical object SHA256:
`5f94e9524cefb17dc9c3c954ceb19e29382280655c954a6d05b63ab4c695867f`.
The object and its provenance remain in the worktree for independent checking.
No replay, emulation, full-map or full-image gate was run.

## ABI and ownership evidence

The neighboring game constructor `00187CDC` calls the established LayoutActor
constructor, names the archive `CounterCollectCoinD`, allocates three flag bytes,
and stores their pointer at receiver offset `0x30`. It installs the native table
at `003CDF54` and secondary interface address points at `003CDF84` and `003CDF98`.
Those secondary entries reach the accepted LayoutActor audio/effect thunks.
The established LayoutActor base provides its alive byte at `0x2C`, audio interface
at `+4`, effect interface at `+8`, and nerve interface at the unadjusted receiver.
The root calls virtual `appear` through slot `+4` before collecting when not alive.
The preceding `00187A98` prepares the counter and index and falls into this root.
These observations place the reconstruction in Game, not an SDK module.
`CounterCollectCoin::collect` is a descriptive proposed C++ name, not a recovered symbol.
`fn_0027109C` returns a sound handle: the root consumes its return value as the receiver
for `00278B98`, which updates the handle at offsets `0x4C`, `0x50`, and `0x54`.
Its declaration here therefore returns `void*`; call sites that ignored the return
had previously declared it void and need consistent declarations on integration.
The timer proposal on `dot/root-18c7dc` now uses the same `void*` return and retains its exact targeted check.

All imported data already have rows. The compiler emits a 20-byte initializer
section corresponding to existing `[003B7AB0,003B7AC4)`, containing two local
name-array initializers. Their five string starts are `003DF29D`, `003DF2AB`,
`003DF2A6`, `003DF2B4`, and `003DF2B9`, each with an existing complete string row.
The nerve import uses existing `[003F17DC,003F17E0)`; its public state name is unknown.
SafeString uses the established `_ZTVN4sead14SafeStringBaseIcEE` identity.

## Forms and scratch map

Four meaningful source forms were compiled. Form 1 used a nullable free-function
receiver: 536 bytes, rejected for its then-unnamed initializer relocation.
Form 2 used a nonnull reference and copied imported table aggregates: 492 bytes,
rejected for size. Form 3 restored local arrays with that reference: 492 bytes,
also rejected for size. Form 4 used the native member receiver: 516 bytes, exact.
A path-only move to Layout was followed by another normal build and exact check.
The member form preserves the original separate effect cases; the free-reference
forms merged their common call tail. No padding, register or volatile tricks were used.
Two compile-only corrections fixed an abstract imported declaration and an editing
syntax error; the latter's stale-object check correctly rejected changed source.

For independent checking, name the root row `_ZN18CounterCollectCoin7collectEi`
and temporarily mark M. Name existing data row `003B7AB0`
`.constdata.CounterCollectCoin.cpp`, keeping its rank U and its 20-byte extent unchanged.
The other imported function/data address names resolve through existing rows.
Only the checker set O. The entire map was restored after verification.
The delivery changes only this source and this report; no exact credit is committed.
