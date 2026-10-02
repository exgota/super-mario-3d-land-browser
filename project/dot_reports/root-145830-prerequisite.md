# WaterFlowCube initializer: accepted-contract prerequisite

Base: `3d69bf00a769676c77f07465163def56a559b48f` (main refreshed by coordinator).
Branch: `dot/root-145830`.
Target: unnamed U row `0x00145830–0x00145A64`, 564 bytes;
literal pool starts at `0x00145A24`.

## Result

Stopped during ABI preflight, with zero source forms or compile attempts.
No exact claim, canonical check, replay, or all-O gate was made.
No source, shared header, accepted Factory source, map, or tool was changed.

## Blocking accepted contract

`Game/backup/src/Factory/fn_0018938C.cpp` declares the C-linkage import
`fn_00270AB0(Actor*, const ActorInitInfo*)` using two anonymous-namespace
types. Its function is already O at `0x0018938C–0x00189480` in this base.
The accepted source arrived in `f6bf44f1563b84d0d3489bd518922f4a7740a5c5`.

This target calls the same helper after the accepted
`al::initActorPoseTRSV(al::LiveActor*)`. The callee at `0x00270AB0`
accesses LiveActor's pose keeper at +0x14 and applies translation, rotation,
and scale obtained from ActorInitInfo. A typed WaterFlowCube implementation
would need the established `al::LiveActor` and `al::ActorInitInfo` contract,
which is incompatible with the accepted private-type declaration.

The same accepted Factory file also declares `fn_0027D180` with a float
destination and private ActorInitInfo pointer. Its callee reads an integer
placement argument with a -1 sentinel; this target's destination is the
integer field +0x70, initialized to 60 by its constructor.
`Game/backup/src/Enemy/Bubble.cpp` already uses the binary-supported
`bool fn_0027D180(int*, const al::ActorInitInfo&)` declaration.

Prerequisite: the owner of the accepted Factory source must reconcile these
shared imports with the established al types and reverify its accepted
function before this target is retried. This packet does not rename an
import, introduce a private adapter, or edit accepted source to bypass that
contract.

## Reusable identity and data evidence

The constructor `0x00145A68` installs dispatch row `0x003C87A4–0x003C885C`;
its init slot points to this target. The target's local literals identify
WaterFlowCube and its EffectObjStream effect setup.
The constructor establishes an additional interface pointer at +0x60,
float fields +0x64/+0x68/+0x6C, integer +0x70, float +0x74, matrices at
+0x78/+0xA8, and an AreaShape pointer at +0xD8.
Scene-object registration receives the interface at +0x60, not LiveActor.

All directly referenced out-of-function data have existing rows:
SafeString dispatch `0x003D9C34–0x003D9C50`, address point +8, and
Nerve instance `0x003F2D58–0x003F2D5C`. The installed actor dispatch row
also exists; no inferred ABI prefix is treated as a missing-row blocker.
The remaining constants and strings belong to the target's existing pool.
No data boundaries or owners were invented.

Original executable SHA-256 verified:
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No object/provenance packet exists because the prerequisite prevented a
permitted source attempt. The scratch map remains byte-identical to base.
