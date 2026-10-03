# Bros movement jump update: player-position entry prerequisite

Branch: dot/root-13ffb8. Base: 64d1bb9f3c97318481de9baa08de473608b9e042.
Target: 0013FFB8–001401FC, 580 bytes, U; pool starts 001401DC. Exact claims: none.

Constructor 001401FC calls al::NerveStateBase at 002684E0 with the name "ブロス[移動]" (Bros movement), installs the mapped dispatch 003C7C58, and stores the host at +0x0C. Its movement parameters at +0x10 are three float pointers; +0x20 selects a position/quaternion record; +0x24 holds a subordinate movement state; +0x28 is a second actor; +0x2C is an optional position pointer; +0x30–32 are byte flags. This identifies an Enemy movement-state method, without claiming a recovered original C++ class name.
Initializer 00385AA4 installs dispatch 003C07CC for nerve 003F2FC4; that dispatch enters 0013FFB0, loads the state host and falls through into the target. The target starts Jump actions, runs its movement state, turns toward the player, and transitions both actors to JumpEnd before selecting nerve 003F2FC8.
The three nerve rows 003F2FC0, 003F2FC4 and 003F2FC8 and the installed dispatch rows exist separately. No missing-row or adjacent-data-grouping claim is made.

At 00140110, the target calls entry 0027A5C8 without preparing an argument; the result is passed as a position reference to 00273E5C. Entry 0027A5C8 is a four-byte no-op prefix before the distinct mapped rp::getPlayerPos() entry 0027A5CC.
The ordinary Player/PlayerFunction.h route returns const sead::Vector3f& with no arguments, but maps to 0027A5CC. Case-insensitive Game/lib searches found no ordinary caller or declaration for the exact 0027A5C8 entry. Accepted Factory/group_00223D40.cpp instead defines fn_0027A5C8 as void*(void*) and declares the underlying mangled position function with that extra argument.
This repeats the prerequisite independently documented by dot/root-3112e4 in published commit 4ac3a68df657ca342656edac01ef4fd399c32d64: [root-3112e4 ABI preflight report](https://github.com/exgota/super-mario-3d-land-browser/blob/4ac3a68df657ca342656edac01ef4fd399c32d64/project/dot_reports/root-3112e4-abi-preflight.md). That report is not present on this refreshed base.
A coherent, compatible prefix contract requires integrator-owned reconciliation. Substituting the +4 entry, inventing a dummy argument, creating a new alias, or editing accepted Factory code is outside this attempt.

Stopped at ABI preflight: zero source forms, no build/object, no canonical check, no match claim. No map/rank/tool/header/source changes. The entire map remains byte-identical to the base. No replay or daily audit was run.
Owner code.bin SHA-256: e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64.
