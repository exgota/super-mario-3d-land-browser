# 00316788: missing vector row and ABI prerequisites

Status: blocked at preflight; zero source forms, builds, or canonical checks.
No match is claimed. Target remains U; no source/header candidate is supplied.
Base: `6b0e2a1385b814b70344023d9816424edcc7d832` (main refreshed 2026-10-02 23:36 UTC).
Branch: `dot/root-316788`. Full interval: `00316788–003169C0`, 568 bytes;
code ends at `00316990`. Original code SHA-256 was rechecked as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Missing data row

Literal `00316998` references `004305D4`, copied as three words before the
cylinder-sensor test at `003167E4`. No map row contains this BSS address.
This root establishes an access/dependency of at least 12 bytes, not ownership.
Independent identity is recorded in published
`project/dot_reports/root-316014-needs-rows.md` (branch `dot/root-316014`),
also rechecked directly here. The clean `sead::Vector3<float>::ey` declaration exists in
`lib/sead/include/math/seadVector.h`; its address is also documented in
`lib/sead/README.md`. Independent static initializer evidence establishes
`004305D4–004305E0` as the 12-byte `(0, 1, 0)` object: `00383B20` loads its
address from `00383D88`, and `00383B24/00383B28` write three float components.
The following independent initialization uses `004305E0` for `ez`, whose
12-byte row already exists. Known minimum and supported object extent are
12 bytes; no unknown table extent is being guessed. Proposed identity is
`_ZN4sead7Vector3IfE2eyE`. An integrator-owned data row is required before
this import can enter an unchanged canonical check; no row was added here.

The other external data references are existing four-byte rows `003F16A8`
and `003F16AC` (nerve instances), plus natural strings in the target's pool.
The installed dispatch table already has row `003D4148–003D4200`; its entry
at `003D417C` points to this target, so no speculative table-prefix row is needed.
Its init entry `003179B0` selects PoleGoal, PoleGoalSuper, and PoleGoalLast.
This grounds the Game actor family and receive-message role without naming an
unverified complete class layout. Inputs are actor, message, sender, receiver;
known local accesses include actor `+68/+84/+B8/+D8/+F0`, sensor position `+8`,
and sensor host `+28`. The established HitSensor header agrees with those offsets.

## Accepted-definition conflicts

`fn_002786A0` is O but defined as `void (private Dispatch*)` in
`Game/backup/src/Factory/group_002786A0.cpp:13`. Retail dispatches virtual slot
`+18`; this target calls it at `0031694C` and immediately uses the returned
address as a three-float input. A void declaration cannot represent this use.

`fn_002D1950` is O but defined through `TAIL_WRAPPER` as `void (void*)` in
`Game/backup/src/Factory/group_0027D24C.cpp:55`. This target calls it at
`0031691C` and copies three words through its returned pointer. The accepted
`fn_002529BC.cpp:29` already imports it incompatibly as `const private Vec3& ()`.
That private import is evidence of an existing conflict, not a public contract
that authorizes another incompatible declaration. No established compatible
public declaration was found for either accepted definition.

Other calls with public contracts include `al::isSensorName`, `al::getTrans`,
`al::isNerve`, `al::setNerve`, `al::invalidateClipping`, and the fireball/boomerang
message predicates. These can reuse their existing symbols and headers.
Further private imports, including `fn_0027D5C4`, have incompatible historical
pointer/reference or return declarations and still need typed review before a
future source attempt. Those do not justify changing accepted wrappers here.

Next prerequisite: establish the missing vector row and reconcile the two
accepted return contracts in their owning lane. No adapter, Factory edit,
shared-header edit, scratch map mutation, replay, or preservation gate was used.
Canonical command/output: not run because preflight failed. There is no object
or build provenance. This report is the complete local-only deliverable.
