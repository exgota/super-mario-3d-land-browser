# State sensor handler 0x001359D4: shared vector ABI prerequisite

Status: preflight blocked, with zero source forms and no exact-match credit.
Branch: `dot/root-1359d4`.
Base: `3d69bf00a769676c77f07465163def56a559b48f`, freshly checked by the
coordinator at 2026-10-02 23:22 UTC.
Interval: `[0x001359D4,0x00135C08)`, 564 bytes; pool starts at `0x00135BDC`.
The base row is U and unnamed; no ordinary source body or prior target report
was present. The worktree and target are assigned exclusively to this lane.

## ABI prerequisite

The required call at `0x00135B5C` targets `0x0027CB64`, mapped as
`sead::Vector3CalcCtr<float>::sub(nn::math::VEC3&, const nn::math::VEC3&,
const nn::math::VEC3&)`. The retail helper subtracts three floats per input
and writes the result through the first reference. It is an existing map row,
not missing code or data.

There is no public `Vector3CalcCtr` declaration or `nn::math::VEC3` type in
Game/lib headers or ordinary sources at the base. Public `seadVector.h`
defines field storage, `set`, and scalar `operator*=`, without this operation.
The same mapped C-linkage symbol is imported by accepted Factory roots
`fn_001BAD90` (O), `fn_00171BE0` (O), and `fn_00355820` (O) using distinct
anonymous-namespace `Vec3`/`Vector3` reference types. Matching scalar layouts
do not establish a coherent C++ declaration across these translation units.
A new private alias, cast adapter, or accepted Factory/shared-header repair
would exceed this lane's assigned scope. No such changes were attempted.
Reopen after a coherent public vector contract is available for reuse.

## Module, fields, and data preflight

The receiver is a Game actor state with the existing 16-byte ActorStateBase
prefix: host LiveActor at +0x0C; auxiliary objects at +0x18 and +0x1C.
The adjacent constructor at `0x00135CC0` calls the state base constructor,
installs the table used by this handler, and initializes these fields.
The existing data row `[0x003C6C74,0x003C6CB0)` contains the handler address
at +0x20. This observation does not claim any additional table extent.
The query `0x0026D070` takes both the +0x18 receiver and first sensor, then
compares the sensor's host at +0x28 with receiver +0x10. Its successful path
forwards receiver and both original sensors to `0x0026CD08`.
The player path sends message 43, tests nerves, subtracts sensor positions
at +0x08, and compares its dot product with the host gravity against zero.
The map-object path sends the existing `fn_0027A4EC` message.

All ten external nerve objects already have four-byte rows at `0x003F2E28`,
`2E2C`, `2E30`, `2E34`, `2E48`, `2E3C`, `2E40`, `2E44`, `2E4C`, `2E50`
(the latter addresses share the `0x003F` prefix). Direct callees also have
rows. No missing-row prerequisite or ungrounded data boundary was found.

## Verification limits

Owner executable SHA-256 verified:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
Neither `python make.py eu` nor `python tools/check.py ... --object ...`
was run: the ABI prerequisite stopped work before a source form existed.
No object/provenance or canonical exact-pass claim is made. This report is
the only tracked change. Map bytes, source, headers, tools, flags, ranks,
Factory files and game data remain unchanged. No replay or all-O gate ran.
