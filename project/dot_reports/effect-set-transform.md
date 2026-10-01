# EffectSet transform update: partial packet follow-up

Target: `fn_001EA220`, EU `0x001EA220..0x001EA31C`, 252 bytes. No exact match
or functionally verified NonMatching result is claimed. The original packet
`project/pro_requests/001EA220.md` is unchanged. This lane enrolled its variant 4
canonically and tested one genuinely new source/API hypothesis, which failed.
The typed constructor baseline is restored and frozen for the final recheck.

## Canonical baseline enrollment

Commit `2f4e03633fc10141350b96175545d79ab1371cad` introduces guarded source
`lib/al/src/Effect/alEffectSetTransform.cpp` and its public declaration in
`lib/al/include/Effect/alEffectSetTransform.h`. It preserves packet variant 4's
constructors, branches and reloads. This is not a ninth independent source form
or a new structural discovery; the eight prior packet variants remain distinct
historical experiments under 791 and 894.

The affine-vector call uses the already mapped genuine
`sead::Vector3CalcCtr<float>::mul(nn::math::VEC3&, const nn::math::MTX34&,
const nn::math::VEC3&)` at `0x0027CB9C`, avoiding a guessed C alias. The minimal
addition to `seadVectorCalcCtr.h` is this overload and its storage-header include.
New `nn/math/math_MTX34.h` declares three rows of four floats, independently
established by the affine-vector body and copier at `0x00291470`. Local views
use defined offset-zero base conversions; size and field offsets are asserted.
Existing VEC3/Vector3 behavior is unchanged.

The coordinator committed and compiled this source through the normal ARMCC
project build. The unchanged strict checker compared the full target interval:

```text
U -> m: The linked candidate differs from the unchanged original interval.
```

Log: `/tmp/mario-main21b-effect-transform-check1.log`. Read-only inspection
confirms the 252-byte raw section is exactly the packet's pattern C. Entry and
configuration still use r7/r6 instead of retail's r6/r7; vector setup remains
r0/r2/r1 instead of r0/r1/r2. The genuine typed API changes no section bytes.

- Source SHA256: `358bdc404ab0cd2e6672e69b05dbf9e17a369f96d1c45930a509bcb983b03d93`
- Object SHA256: `c6605b5437800ae381e9573fec60d954a7cde0f1476d4878edd89e3fa86131e0`
- Section SHA256: `3070ed117843d5426a1ba6025dab579d6a5c305c56a401695c7425aaa4ed03d0`

## New value-returning expression: negative result

Commit `551549ecd1724bfd3c92624471e19ed3f22e2c8a` tested a matrix/vector
multiplication operator returning a local translation by value. Its default
constructor and friend operator replace only the transforming constructor;
the matrix-copy constructor, helpers, branches and shared headers are unchanged.

The API hypothesis was recorded before editing. The permitted
[open-ead/sead README](https://github.com/open-ead/sead/blob/master/README.md)
was reviewed for provenance first. Its
[Vector3 API](https://github.com/open-ead/sead/blob/master/include/math/seadVector.h)
provides a friend matrix/vector operator returning a local by value. Retrieved
header blob: `8de5ca320b1b60b2d28b37d010e008c7b3032ef2`; README blob:
`1f7047ce235b6d27d9f447e30aef894ab88df5d1`. Only API shape was used. The 3DS
operation, fields and callee remain reconstructed from the owner's executable.
The README warns that some inline names are guesses; this is a plausible source
hypothesis, not identification of the original EffectSet expression.

The committed candidate compiled, compact-linked and exported through the
normal project build. The unchanged strict checker reported:

```text
m -> m: The linked candidate differs from the unchanged original interval.
```

Log: `/tmp/mario-main21b-effect-transform-check2.log`. The section is still 252
bytes but adds `vpush/vpop {d8,d9}`, loads returned components into s16-s18 before
the matrix copy, and stores them afterward. Retail has no saved VFP frame and
reloads three stack words after the copy. The candidate retains the wrong
entry/configuration allocation and prepares vector arguments as r2/r1/r0.
It fixes neither remaining issue and worsens the temporary representation.

- Source SHA256: `3de0bbfc03729af7fdd4dc764f119783a35ad5c627a8751ab814ff953e2567ae`
- Object SHA256: `8e6b7314442b63deda3711ccbe7939080eadeffb26da9db31017d396b97b59fb`
- Section SHA256: `0bae772039f3b3c272a28a0cabd9adbfe0445a1cbcc88344d9d2554d4449dbb4`

The source form changes temporary escape/copy behavior: ARMCC can preserve the
returned components in VFP registers across the matrix copy, unlike retail's
address-exposed stack temporary. This narrows the source model without proving
the original constructor or aliasing contract. No further probe is justified
by declaration reordering, reference bindings or arbitrary register guesses.
An independently identified wrapper/API with the needed lifetime or aliasing
contract would be useful new evidence.

Read-only diagnostics are `/tmp/effect_transform_baseline_{disassembly.txt,
diagnostic.json}` and `/tmp/effect_transform_value_return_{disassembly.txt,
diagnostic.json}` on the dot computer. No object bytes were altered, and these
diagnostics are not acceptance evidence. The coordinator owns commits, builds,
strict checks and publication. This lane edits no maps, ranks, boundaries,
tools, compiler flags, ledger or STATE files. No functional execution was run.
The target binary hash remains
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Latest packet-baseline verification

The retained source was applied to exact main8ca3fc0f2aa6c8fe0d870cf4992b9f6029286a13 and committed before the normal project build. Its checker/tool files are unchanged in the subsequently fetched af853bf3e12865dbbf1c2c467868fac318d0a7b8. All197 previously accepted functions passed before the proposal batch and again after the clean vector/matrix declarations, restored default vector assignment, published string header, and three guarded proposals were built. Evidence summary:197/197, no new exact credit from these three roots. The full compact build linked/exported after the ordinary provenance-dependent scaffold retry. Later main additions remain subject to main's own intake revalidation.

Retained typed baseline checkpoint42c2596 on current main: `python tools/check.py fn_001EA220 --object build/eu/obj/lib/al/src/Effect/alEffectSetTransform.o` returned `U -> m: The linked candidate differs from the unchanged original interval.` New data types and method declarations are complete storage/API declarations; no Vector3 assignment change is needed by this branch. The main packet stays unchanged.
