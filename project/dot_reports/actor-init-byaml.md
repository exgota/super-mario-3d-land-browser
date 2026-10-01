# Actor initialization from BYAML

Branch `dot/actor-init-byaml` reserves the unchanged unnamed EU root
`0x002417E8..0x002428CC` (4,324 bytes, U), based on main
`57f902421f6874f132d5ea971d1aa5b224c5a528` fetched by the parent on
2026-10-01 at 20:58 UTC. No exact or functional-equivalence claim yet.

Initial verified facts: this is actor initialization, not graphics validator
state. Both established `al::initActor` (call at `0x0027B6E4`) and
`al::initActorWithArchiveName` (call at `0x00280250`) invoke it. Its five-argument
ABI is actor r0, ActorInitInfo reference r1, object-name SafeString reference r2,
archive-path SafeString reference r3, and optional suffix on the incoming stack.
Other direct callers are at `0x0025A9F8`, `0x00267FBC`, and `0x002D57E8`.
The existing `alActorInitUtil.cpp` has an empty static `initActorImpl` placeholder
with precisely this signature; its body is the target of this lane.

It obtains the resource through the established `findOrCreateResource`, reads
`InitActor` BYAML, handles pose/model/material/executor/sensor/collision/effect,
rail and switches, and later clipping, group clipping, shadows and flags.
The mapped pool marker `0x002419F8` is executable (`ldr r1,[sp,#0x98]`).
The real return is `0x00242854`; embedded literal/string islands appear at
`0x00241B1C..0x00241BD0`, `0x00241F8C..0x00242014`,
`0x0024240C..0x00242490`, and after the return. Reconstruction will cover all
reachable blocks, not stop at the erroneous early pool hint.

The configured `lib/al` compiler is ARMCC 4.1 build 791, verified from this
checkout's `data/config.json`, not inferred from the address. The authorized
local executable's SHA256 was rechecked as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
No map, tools, flags, ledger, STATE, game data, SDK reference or function-byte
replacement is changed. This reservation is notes only. The parent publishes.
