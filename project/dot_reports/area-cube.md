# AreaShapeCube volume test

Source baseline: main `9fba58fa1db1aaabad80d2b77d87139bd83b80e6`.
Target: `_ZNK2al13AreaShapeCube10isInVolumeERKN4sead7Vector3IfEE`,
`0x00330868`–`0x0033091C` (180 bytes, including the 24-byte literal pool).
The original executable SHA-256 was independently rechecked as
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
All binary observations below use that executable and its supplied exheader.
No external implementation, removed library, assembly replacement, or embedded
instruction bytes were used.

## Status

Candidate 6, committed locally in `db09380`, passed the unmodified project
checker on the complete 180-byte function interval, including its literal pool.
The checker reported `m -> O`. The build used the configured ARMCC 4.1 build 791
and recorded committed-source provenance. This result used the tools from the
stated baseline and the independently evidenced import identities below.

The source corrects the retained fragment's volume comparisons and retains the
existing `NON_MATCHING` guard for integration. The private selector is an unsigned
byte, while its constructor retains the Boolean API. Source and header are frozen
after the passing checkpoint. The main lane is responsible for accepting the
import names and rechecking in its current checkout.

Two local map additions were required: naming the existing
`AreaShape::calcLocalPos` row and adding the previously unmapped float zero-vector
BSS object. No existing function boundary was changed. No map, rank, ledger,
configuration, or tool changes are part of this source proposal.

## Source change

`lib/al/src/AreaObj/alAreaShapeCube.cpp` keeps the existing zero-vector copy,
`calcLocalPos` call, and cube-base-dependent Y bounds. Each coordinate of the Y
bounds vector is chosen separately by comparing the selector with `true`, matching
retail's comparison against 1. The private selector has unsigned-byte storage,
matching the observed load. Its constructor still accepts `bool`.
It rejects a point if Y is
below the lower bound or above the upper bound, or X/Z is outside `[-500, 500]`.
Otherwise it returns true.

The old fragment used strict inner comparisons and mistakenly tested Y instead
of X for the second horizontal upper bound. Neither behavior agrees with retail.
The reconstruction uses ordinary floating-point comparisons and no type punning.
The constructor body and signature are unchanged. The dedicated cube header's
private selector changes from `bool` to `u8`, preserving its offset, size, and all
constructor-established values. No shared base-class or sead header is changed.

For finite local coordinates, all six faces are included. Base-aligned cubes use
Y `[0, 1000]`; center-aligned cubes use Y `[-500, 500]`. Both use X/Z `[-500, 500]`.
The return from `calcLocalPos` is intentionally ignored, as in the original.

## Instruction evidence

The original first loads a pointer from pool word `0x00330904`. That word is
`0x004305F8`. Instructions at `0x00330878` and `0x00330884` copy exactly three words
from that object into the local stack vector. `r4` preserves `this`, `r2` receives
the input-vector address, and `r1` receives the local-vector address. The only call
is at `0x0033088C`, directly targeting `0x00216D9C`.

At `0x00330890`, a byte is loaded from `this + 0x14` and compared to 1. Conditional
loads choose these four literal-pool floats:

| Pool address | Float | Use |
| --- | --- | --- |
| `0x00330908` | 0 | Base-aligned lower Y |
| `0x0033090C` | -500 | Center-aligned lower Y |
| `0x00330910` | 1000 | Base-aligned upper Y |
| `0x00330914` | 500 | Center-aligned upper Y |

The Y float comes from `sp + 4`. `vcmpe.f32 s2, s0` at `0x003308AC`, followed by
`blo` at `0x003308B4`, rejects only a value below the lower bound. The second
comparison at `0x003308B8`, followed by `bgt` at `0x003308C0`, rejects only a value
above the upper bound. This is the rejection-form predicate in the source.
The VFP conditions also leave unordered Y comparisons on the successful path;
this detail should be preserved when assessing generated code rather than
assuming strict IEEE positive-form conjunctions are interchangeable.

X is loaded from `sp + 0` at `0x003308C4`; Z is loaded from `sp + 8` at
`0x003308E0`. Both undergo the same two integer comparisons, an ARMCC optimization
of comparisons to these constant floats:

- Compare the raw word unsigned against `0xC3FA0000` (-500.0f); reject if higher
- Compare it signed against `0x43FA0000` (+500.0f); reject if greater

The second constant is formed by adding `0x80000000` to the first. Neither equality
branches to failure. The function sets `r0 = 1` only after passing the Z checks;
all failure edges share `r0 = 0` at `0x003308F8`.

These original integer comparisons reject both signs of X/Z NaNs. The source
does not manually reproduce the optimizer's representation test: compiler output
must determine whether the original constant-comparison lowering is recovered.

## Independent zero-vector identity

The address `0x004305F8` is not inferred merely by aligning a candidate relocation
with this target. The separately mapped static initializer
`__sti___14_seadVector_cpp` (`0x00383870`–`0x00383E00`) establishes the object:

- `0x003839A4` loads `s0` from `0x00383D68`, whose word is `0x00000000`
- `0x00383AC8` loads `s1` from `0x00383D70`, whose word is `0x3F800000` (1.0f)
- `0x00383B00` loads `r0` from `0x00383D80`, whose word is `0x004305F8`
- `0x00383B04`, `0x00383B08`, and `0x00383B0C` store `s0` at offsets 0, 4, and 8

The same initializer immediately constructs adjacent float triples as follows:

| Address | Values | Identity |
| --- | --- | --- |
| `0x004305C8` | (1, 0, 0) | `sead::Vector3<float>::ex` |
| `0x004305D4` | (0, 1, 0) | `sead::Vector3<float>::ey` |
| `0x004305E0` | (0, 0, 1) | `sead::Vector3<float>::ez` |
| `0x004305EC` | (1, 1, 1) | `sead::Vector3<float>::ones` |
| `0x004305F8` | (0, 0, 0) | `sead::Vector3<float>::zero` |

An independent named getter corroborates this: `ActorPoseKeeperBase::getRotate`
at `0x00334F10` returns the word at `0x00334F18`, which is `0x004305F8`. The existing
clean header declares a three-float `Vector3<T>` and its static `zero` member;
`lib/sead/README.md` already records these addresses from earlier investigation.
The recovered original data symbol is `_ZN4sead7Vector3IfE4zeroE`, size 12.

The executable map currently stops at `0x003F39D4`, so this BSS object has no map
row. Its absence is a data-identity coverage gap, not an incorrect function
boundary. The supplied exheader places initialized data at `0x003E1000`, with
0x129D4 initialized bytes and 0x3D684 BSS bytes; `0x004305F8` is within that BSS
allocation. The object is initialized by ordinary runtime stores above, and no
bytes from that region are assumed to be present in the executable file.

## Independent class layout and helper identity

The named `AreaShape` base constructor at `0x0024E028` stores:

- Virtual table pointer at +0
- Null base-matrix pointer at +4
- Three 1.0f scale components at +8, +0xC, and +0x10

The named `AreaShapeCube` base-object constructor at `0x001C50EC` calls this base
constructor, then stores its Boolean parameter to byte +0x14. It loads the cube
virtual-table address point `0x003D62C0`, where the first entry is the volume-test
address `0x00330868`. These independent constructors verify the layouts already
declared in `alAreaShape.h` and `alAreaShapeCube.h`.

The only volume-test callee, `0x00216D9C`–`0x00216E3C`, is an existing unnamed map
row. It takes `this`, an output vector, and an input vector in r0/r1/r2. It checks
the three scale components at `this + 8/+0xC/+0x10` using the epsilon 0.001f from
`0x00216E38`. The helper at `0x0026D698` used for those checks computes absolute
value and compares it to the epsilon. Any failed scale check returns false before
writing the output.

The successful path reads the base-matrix pointer at +4, passes output/matrix/input
to `0x001D7978`, divides the result componentwise by the three scale components
through `0x0027CC48`, and returns true. Direct inspection establishes the former
helper as translation subtraction followed by multiplication by the transposed
matrix basis, and the latter as three componentwise floating-point divisions.
The original null-matrix path conditionally copies input to output, then still
falls through to the matrix helper; no safer behavior is invented in this report.

An independent cylindrical-volume routine at `0x003341F8` calls the same helper
at `0x00334214` with an initialized local three-float vector, then tests its Y and
XZ radius. This corroborates the world-to-local area-coordinate role.

These observations identify the declared member `al::AreaShape::calcLocalPos`.
The ARMCC object confirms its mangling as
`_ZNK2al9AreaShape12calcLocalPosEPN4sead7Vector3IfEERKS3_`.
No implementation of this helper is supplied or imported from external library
code.

## Compile and check checkpoints

The parent lane built committed candidate `f4e8a71` through the project build.
The object is `build/eu/obj/lib/al/src/AreaObj/alAreaShapeCube.o`, accompanied by
the project's `.provenance.json` record. Its function section is 180 bytes.
Read-only Capstone inspection showed that all predicate operations, registers,
branches, frame setup, and frame teardown follow the retail sequence. The
Boolean selection differed: candidate `cmp r0, #0` with NE-first conditional
loads versus retail `cmp r0, #1` with EQ-first loads, also changing the order of
four float literals. The second candidate addresses only that difference.

The project checker command for the first object was:

```sh
python tools/check.py _ZNK2al13AreaShapeCube10isInVolumeERKN4sead7Vector3IfEE \
    --object build/eu/obj/lib/al/src/AreaObj/alAreaShapeCube.o
```

The parent lane reported the actual checker output:

```text
U -> M: No established original address for _ZN4sead7Vector3IfE4zeroE.
```

This result is an import-resolution failure, not a byte-exact comparison.

Candidate 2 (`d906d07`) changed the Boolean selector to `mIsCubeBase == true`.
For this local verification only, the parent lane named the existing helper row
and added the 12-byte BSS zero-vector row using the independent evidence above.
Neither import addition changes any existing function boundary. The unmodified
checker reported:

```text
M -> m: The linked candidate differs from the unchanged original interval.
```

Read-only inspection of the resulting `candidate.axf` found precisely six
different bytes. The two middle float literals appeared in the opposite order,
and the corresponding load offsets tracked that different order:

| Address | Candidate 2 word | Retail word |
| --- | --- | --- |
| `0x003308A0` | `0x1D9F0A1A` | `0x1D9F0A19` |
| `0x003308A4` | `0x0DDF0A18` | `0x0DDF0A19` |
| `0x0033090C` | `0x447A0000` (1000.0f) | `0xC3FA0000` (-500.0f) |
| `0x00330910` | `0xC3FA0000` (-500.0f) | `0x447A0000` (1000.0f) |

Candidate 3 calculates `bottomY` and `topY` with separate scalar ternaries, in
that order, rather than selecting between two constructed `Vector2f` values.
This follows the original's lower-bound-then-upper-bound literal order without
any instruction replacement, bit reinterpretation, or compiler-flag changes.

The parent committed candidate 3 as `aa2e3bf` and built it through the project.
The unmodified checker reported:

```text
m -> m: The linked candidate differs from the unchanged original interval.
```

Read-only inspection of the linked candidate confirmed that all literal-pool
ordering and loads now use the retail ordering. The sole differing instruction
was at `0x00330890`: ARMCC emitted `ldrsb r0, [r4, #0x14]` (`0xE1D401D4`) instead
of retail's `ldrb r0, [r4, #0x14]` (`0xE5D40014`), accounting for three differing
bytes. The source has Boolean storage, while the command has `--signed_chars`;
reusing the Boolean in two scalar comparisons changed ARMCC's load choice.

Candidate 4 uses `const u8 isCubeBase = mIsCubeBase` and compares that unsigned
local in the same two scalar ternaries. This expresses the independently observed
unsigned-byte load without changing the field layout, using a raw-memory alias,
or changing compiler options. The parent committed it as `69f929c`. The checker
reported the same `m -> m` byte-difference result. Read-only linked inspection
showed that it recovered `ldrb`, but the pool ordering returned to candidate 2's
same six differing bytes.

Candidate 5 (`eb64b00`) constructed the `Vector2f` directly from two separate
coordinate ternaries, each reading the existing Boolean field. The checker again
reported `m -> m`; read-only linked inspection showed the same sole `ldrsb`
difference as candidate 3. It did preserve the original pool order.

Candidate 6 retains candidate 5's direct field expressions and changes only the
private field declaration from `bool` to `u8`. The observed storage is one byte
at +0x14, all known writes are from the Boolean constructor parameter, and the
original read is explicitly unsigned. This reconstruction preserves the class's
layout and Boolean public API and does not rely on raw-memory aliasing. The
precise original C++ typedef is not claimed; the unsigned-byte representation is
what the binary establishes.

The parent committed candidate 6 in `db09380` and rebuilt it through the project's
configured ARMCC 4.1/791 source-to-object step. The same unmodified checker command
then reported:

```text
m -> O: The complete source-generated function interval matches byte for byte.
```

Read-only inspection of the latest linked `CANDIDATE_CODE` independently found
180 bytes and zero differences. Its SHA-256, also the original interval's hash,
is `4895d950ed329cf5f0d0ccc4f9f0c4b98a3ed57545e26e14a9694a9984b10048`.
The direct ARMCC object's SHA-256 at this checkpoint is
`154d7c1eb795c3b84375c38c780b358bd267e05618cca971647c62a621840118`.
Its project provenance record reports `inputs_stable: true` and the configured
`data/compilers/4.1/791/bin/armcc.exe`.

| Candidate | Local commit | Project-check result | Linked byte differences |
| --- | --- | --- | --- |
| 1 | `f4e8a71` | `U -> M`, unresolved zero-vector import | Not compared |
| 2 | `d906d07` | `M -> m` | 6 |
| 3 | `aa2e3bf` | `m -> m` | 3 |
| 4 | `69f929c` | `m -> m` | 6 |
| 5 | `eb64b00` | `m -> m` | 3 |
| 6 | `db09380` | `m -> O` | 0 |

## Verification limits and integration

`tools/check.py --object` first verifies committed source and direct project
ARMCC build provenance. `tools/low/checkExactBytes.py` then resolves external
addresses only from `map.csv`; neither tool has a `--data-symbols` override.
With the unchanged baseline map, the zero-vector import rejects. The parent
applied only these independently supported import prerequisites locally for the
passing check:

```csv
0x00216D9C,0x00216E38,0x00216E3C,          ,U,f,_ZNK2al9AreaShape12calcLocalPosEPN4sead7Vector3IfEERKS3_,
0x004305F8,          ,0x00430604,          ,U,db,_ZN4sead7Vector3IfE4zeroE,
```

The first row only names an existing interval. The second adds a 12-byte BSS
object whose identity and extent follow from the independent static initializer,
adjacent objects, and getters. It does not extend or move an existing function.
Both imports are ordinary undefined-symbol relocations in the ARMCC output;
no absolute game address is embedded in the reconstructed C++.

The main lane should review the evidence and accept those two identities, build
the source with ARMCC 4.1 build 791 through its project build, and run its checker.
The only deliverable code files are `lib/al/src/AreaObj/alAreaShapeCube.cpp` and
`lib/al/include/AreaObj/alAreaShapeCube.h`, plus this report. Local map/rank edits
used by the checker are deliberately excluded from publication.

## Current canonical checker revalidation

On 2026-10-01, a clean worktree of main `a360142fbddd9ab875ab68314c55445ade05fd00` was reconstructed and its entire Git tree verified. The proposed source/header edits were applied and committed as local verification checkpoint `eace3c0`. The unchanged current project build (`python make.py eu`, ARMCC 4.1/791 for game code) compiled, linked, and exported successfully. The current canonical `tools/check.py --object` commands were then rerun on those freshly built objects. No checker/tool changes were made. The same independently evidenced local-only map identities were supplied; no existing function interval changed.

The cube volume function reports `U -> O: The complete source-generated function interval matches byte for byte.` All 180 bytes pass. The main lane must still review and adopt the documented helper/BSS identities before its own acceptance.
