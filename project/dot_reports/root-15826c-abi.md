# State sensor handler 0x0015826C: shared vector ABI prerequisite

Status: preflight blocked, with zero source forms and no exact-match credit.
Branch: `dot/root-15826c`.
Base: `3d69bf00a769676c77f07465163def56a559b48f`; public main rechecked
with `git ls-remote` at 2026-10-02 23:27 UTC and still at this commit.
Interval: `[0x0015826C,0x001584A0)`, 564 bytes; pool at `0x00158474`.
The base target is U and unnamed, with no source/report ownership overlap.
This reserved follow-on was inspected only after the 1359D4 packet was sealed.

The required call at `0x001583F4` targets `0x0027CB64`, mapped as
`sead::Vector3CalcCtr<float>::sub(nn::math::VEC3&, const nn::math::VEC3&,
const nn::math::VEC3&)`. The observed helper subtracts the three float
components and writes through its first reference. No public declaration
of this helper or `nn::math::VEC3` exists in Game/lib at the base. The public
`seadVector.h` provides scalar storage and operations without this helper.
Accepted Factory roots `fn_001BAD90`, `fn_00171BE0`, and `fn_00355820`
all rank O and import that symbol with distinct translation-unit-private
`Vec3`/`Vector3` reference types. A new private alias or cast adapter would
hide the inconsistency. Establishing a coherent public contract would
require work on accepted declarations outside this lane's scope.

The receiver has the Game ActorStateBase prefix and host LiveActor at
+0x0C. Its auxiliary state objects are at +0x1C and +0x20, four bytes later
than the corresponding 1359D4 fields. The constructor at `0x00158558`
calls the state base constructor, installs its dispatch table, and clears
these fields. The existing row `[0x003C9D0C,0x003C9D48)` contains this
handler at +0x20; no additional table boundary is inferred.
`0x0026D070` consumes the +0x1C object and the first sensor, comparing the
sensor host (+0x28) with the object's +0x10 field. A successful comparison
forwards both sensors to `0x0026CD08`. Otherwise the player path sends
message 43, checks nerves, and uses the difference of sensor positions
at +0x08 and the host gravity to decide whether to send enemy attack.
The map-object path uses the existing `fn_0027A4EC` message helper.

All ten external nerve references have four-byte rows: `0x003F2E54`,
`0x003F2E58`, `0x003F2E5C`, `0x003F2E60`, `0x003F2E74`, `0x003F2E68`,
`0x003F2E6C`, `0x003F2E70`, `0x003F2E78`, and `0x003F2E7C`.
All direct callees also have rows. No missing-row blocker was found.

The owner executable SHA-256 is
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No source form, build, or `tools/check.py --object` run was made: ABI
preflight found the prerequisite first. There is no object/provenance
or exact-pass claim. This report is the only tracked change. Map bytes,
source, headers, Factory files, ranks, flags, tools and game data remain
unchanged. No replay or all-O gate ran. Reopen once the shared vector
contract is established and reusable without repairs to accepted sources.
