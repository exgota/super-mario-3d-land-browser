# PackunFlower initialization: 544-byte canonical match

Branch: `dot/root-13d41c-source`. Old `dot/root-13d41c` was not modified.
Base: `64d1bb9f3c97318481de9baa08de473608b9e042` (freshly fetched main).
Source head: `316e68087c528907ab8aeb6d241ad3c4dc7f3180`.
Target: `0013D41C–0013D63C`, pool starts `0013D5E8`, 544 bytes; U on base.
Symbol: `_ZN12PackunFlower4initERKN2al13ActorInitInfoE`.

## Canonical result

Using the existing approved ARMCC 4.1/791 plus wibo installation:

```sh
. /workspace/scratch/73cdb2c524af/mario-dot/development_environment.sh
export DEVKITARM=/usr
python make.py eu
python tools/check.py _ZN12PackunFlower4initERKN2al13ActorInitInfoE --object build/eu/obj/Game/backup/src/Enemy/PackunFlower.o
```

The normal build linked `RE-Pepper.axf` and exported `code.bin`.
The unchanged canonical checker reported:
`M -> O: The complete source-generated function interval matches byte for byte.`
Object SHA256: `f9e91d2b5ea25756ed75ae51efa6f5577d599f117ec1ef146a82f2687d2b97b8`.
Its adjacent `PackunFlower.provenance.json` records direct project ARMCC output.
One natural body form, zero failed canonical checks. Three initial link failures
were setup corrections: reuse the already accepted bool argument symbol, and
leave address-import rows unnamed so the existing scaffold preserves all aliases.
No instruction shaping, compiler-flag changes, oracle changes, or replay used.

## Required scratch names

Only these previously unnamed rows need temporary names; root rank was M:

- `0013D41C`: `_ZN12PackunFlower4initERKN2al13ActorInitInfoE`
- `00279228`: `_ZN21PackunFlowerTransformC1Ev`
- `0027924C`: `_ZN17PackunFlowerTraceC1ERKN4sead14SafeStringBaseIcEE`

Keep `0027CF20` unnamed: accepted sources use both uppercase and lowercase
address aliases. The normal scaffold supplies both only while the row is blank.
All map bytes are restored after verification; no ranks or map edits committed.

## ABI and data evidence

- Constructor `0013D8CC` calls MapObjActor and establishes root fields at
  `60/64/68/74/78/7C/80/84`; independent factory `0039690C` allocates `0x88`.
- Constructor `00279228` copies a Matrix34f then clears its pointer at `30`.
  Independent user `002792A8` updates both; the pointer's target remains opaque.
- Constructor `0027924C` calls MapObjActor, initializes `60/64/68/6C`, and installs
  dispatch in existing row `003CD798–003CD830`. Root allocates `0x70` and uses
  its ordinary LiveActor init slot. The two neutral constructor class names are
  reconstructed descriptive proposals, subject to integrator naming review.
- The `0x28` BlowDown allocation uses the established EnemyStateBlowDown class.
- `fn_0027D1DC(int*, const al::ActorInitInfo*)` exactly reuses Factory's current
  public pointer contract. No reference alias or private-info repair was added.
- Shared `alActorInitializationImports.h` supplies `00280538` and `0027FAB8`;
  existing typed stage-switch, nerve, argument, and offCollide contracts are reused.
- Separate nerve data rows `003F22B4` and `003F22D0` retain separate identities.
  SafeString uses its existing vtable row; strings are ordinary C++ literals.
  No additional data row or inferred table extent is required.

Only the new Enemy source/header and this report are proposed. This is an init-only
class view, not a claim to reconstruct its remaining methods or imported callees.
No accepted class/header changed, and no family-wide/full-map audit was run.
