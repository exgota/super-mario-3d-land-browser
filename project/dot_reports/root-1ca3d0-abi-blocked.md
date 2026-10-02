# Actor light transition update: ABI preflight blocker

Base: `8ae3d9d55e639057950efb7ba3a0c7ccc29c5f65`.
Branch: `dot/root-1ca3d0`.
Target: unnamed `0x001CA3D0`, interval `[0x001CA3D0, 0x001CA5E8)`;
536 bytes including the four-byte pool at `0x001CA5E4`; rank remains U.
Outcome: parked before implementation. No matching claim.

## Module and ABI evidence

- The only direct branch to the target is the tail continuation at
  `0x001CAAA8` in `fn_001CA998`. It forwards its controller and execution
  field arguments unchanged.
- Existing `lib/al/src/LiveActor/alLiveActor.cpp` declares `fn_001CA998`
  with an `al::ActorLightCtrl*` first parameter. `LiveActor::movement` calls
  it with `mActorLightCtrl` and `ActorExecuteInfo::getPointerAtOffset18()`.
  This grounds the target in the al actor-light family, not Game Factory.
- `0x001CA5E8` allocates 0x2C bytes, calls the constructor at `0x001CAAB0`,
  and stores the result in the LiveActor light-controller field at +0x48.
  That constructor initializes fields through +0x24 and allocates a
  separate 0x60-byte light state at controller +0x18.
- The target reads controller type +0x00, selected state +0x10, previous
  state +0x14, output +0x18, duration +0x1C, elapsed +0x20, and alternate
  state holder +0x24. It returns immediately when previous state is null.
- On completion it assigns the selected 0x60-byte light state, sets elapsed
  to -1, clears previous state, and dispatches execution-field slot +0x1C.
  During transition it calls `0x0024C400` on five four-float fields, then
  interpolates three floats at +0x50, +0x54 and +0x58. The callee consumes
  its interpolation amount in s0 and three state pointers in r0-r2.
- The target pool contains only 1.0f. There is no missing external data
  row in this function, so this is an ABI blocker rather than needs-rows.

## Blocker and next step

Main defines `al::ActorLightCtrl` privately in `alLiveActor.cpp` as:

```cpp
class ActorLightCtrl {
public:
    unsigned char _0[ 8 ];
    int _8;
};
```

This is a 12-byte placeholder. Its public header has only a forward
declaration. No consistent shared complete
declaration exists. A second complete definition in this proposal would
conflict with the existing type; a differently named pointer or offset
casts would hide the conflict rather than resolve it.
This establishes a declaration conflict for reconstruction, not an observed
accepted-byte mismatch or runtime failure. No existing match was retested,
and no demotion or behavioral regression is claimed.

The integration owner should first move a binary-grounded controller
definition into a shared header and reconcile the existing LiveActor
translation unit. The execution-field callback type also remains opaque
in `alActorExecuteInfo.h`. Resume source reconstruction after ABI ownership
is settled; do not edit accepted Factory wrappers to manufacture imports.

Attempts: zero source forms, zero builds, zero canonical checks.
`python make.py eu` and unchanged `tools/check.py --object` were not run.
No object/provenance exists because no C++ source was created. Map bytes
were never changed; no source, header, tool, ledger or rank was modified.
Original code SHA-256 was verified as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
