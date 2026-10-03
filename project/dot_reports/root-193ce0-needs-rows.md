# 0x00193CE0: camera dash-angle update needs a vector data row

Refreshed public main and based `dot/root-193ce0` on
`64d1bb9f3c97318481de9baa08de473608b9e042` (2026-10-03 UTC).
The target remains U: full interval `[0x00193CE0, 0x00193F28)`, 584 bytes;
the literal pool starts at `0x00193EF8`.

## Missing row

`0x004305D4` is the existing clean-header declaration
`sead::Vector3<float>::ey` (`_ZN4sead7Vector3IfE2eyE`). Main has no data row
covering this address. The target passes this address as the axis argument to
`0x0026F7B4` at `0x00193E58`, using the literal at `0x00193F24`.
The root itself performs no direct loads from this object; its own evidence
establishes reference use, while the 12-byte span comes from the independent
initializer. See the published `root-316014-needs-rows.md` report for the same
identity and a separate target that directly reads all three components.

Independent vector static initialization at `0x00383B20` loads this object's
address from `0x00383D88`; `0x00383B24` and `0x00383B28` initialize its three
float components to `(0, 1, 0)`. The proven minimum is therefore 12 bytes,
`[0x004305D4, 0x004305E0)`, consistent with the declared Vector3 ABI. Any
larger containing object's extent is unestablished and is not proposed.
Adjacent `ez` already has its own row `[0x004305E0, 0x004305EC)` and must
remain separate. No row or boundary was added or changed.

## Module and ABI evidence

Constructor `0x00193F5C` calls `al::NerveExecutor` with the embedded name
`ダッシュ角度調整者` (dash-angle tuner). Its allocator caller at `0x00149128`
requests 0x14 bytes. The constructor installs vptr `0x003CECE0`, clears two
floats at +0x08/+0x0C, and enables a byte at +0x10. Target `0x00193CE0`
conditionally updates its nerve and adjusts camera position/target fields at
+0x34/+0x40 in its second argument. Caller `0x002777AC` supplies the camera's
dash-angle parameter object as its third argument. Existing `lib/al/Camera`
headers identify the paired angle and zoom-offset parameters, supporting
placement in that family; no new class/header contract was committed.

Stopped before source attempts because the required data row is missing.
Attempts: 0. No compile, link/export, or canonical `tools/check.py --object`
run was attempted; no exactness claim or object/provenance artifact exists.
Only this report is committed. Map bytes remain identical to base.
