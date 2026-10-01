# SwingNeedleRoller initialization: bounded source proposal

Baseline: immutable main `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`,
independently refreshed through the GitHub connector on 2026-10-01 at 19:19 UTC.
Target: existing U interval `0x001855C0`–`0x00185B40`, 0x580 / 1,408 bytes.
Branch: `dot/swing-needle-roller`.

## Result

This is an unaccepted reconstruction, with **zero matched bytes and no
functionally verified NonMatching claim**. One structural C++ form was attempted.
The first physical compile stopped on ARMCC's prohibition on an external object
of abstract `al::Nerve` type. A concrete declaration-only view fixed that type
error without changing the root's structure. Source checkpoint `df43543` builds
through the unchanged `make.py eu`; the normal compact image links and exports.

The direct ARMCC 4.1 build 791 object contains a 1,400-byte root, including its
literal pools, against the unchanged 1,408-byte retail interval. It has no
assembly replacement, instruction byte array, custom compiler flag, modified
object, or external implementation. Source/header changes are local to this
actor; shared headers are unchanged.

The final canonical command, with original data/function boundaries and ranks,
and only the four local function-name aliases below, is:

```sh
. ./development_environment.sh
export ARMCC41INC="$PWD/data/compilers/4.1/791/include"
python tools/check.py _ZN17SwingNeedleRoller4initERKN2al13ActorInitInfoE \
  --object build/eu/obj/Game/backup/src/MapObj/SwingNeedleRoller.o
```

It exits 1 and reports exactly:

```text
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

The missing imports are the two complete compiler virtual tables described
below. The gate does not produce a canonical byte-difference count. The size
observation alone is not a match or a measurement of just eight differing bytes.

A separate temporary local data-ownership experiment supplied the two proposed
whole-table rows. The same unchanged checker then reported:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

That experiment was rolled back. All original map boundaries, pool boundaries,
row classifications, and ranks were mechanically compared with HEAD and are
unchanged. The ownership proposals have not been adopted. No map, rank, tool,
ledger, STATE, flag, or binary change is committed on this branch.

## Independent actor identity and layout

The name `SwingNeedleRoller` is provisional source nomenclature based on the
retail archive string. At `0x0018561C`, init forms the address of the inline
`SwingNeedleRoller` string at `0x00185988`, wraps it in the established
`sead::SafeString` ABI, and calls `al::initActorWithArchiveName` at `0x002801E0`.
The target is the init slot at `0x003CDC7C`, four bytes after the actor's primary
virtual-table address point `0x003CDC78`.

Independent constructor `0x00185DFC`–`0x00185EB0` calls the established
SafeString LiveActor constructor at `0x00280428`. It installs address point
`0x003CDC78` at offset 0 and derived interface address points at +0x68, +0x74,
and +0x88 at actor offsets 4, 8, and 0xC. This establishes ownership of the init
slot. `0x003CDC78` is an address point, not the beginning of the complete ABI
table. The current 152-byte map row also starts at that address point.

The constructor and independent consumer `0x00256C50` corroborate these fields:

| Offset | Evidence-backed role |
| --- | --- |
| 0x60–0x6F | Initial quaternion, constructor copies `sead::Quat<float>::unit` |
| 0x70 | 0x1C-byte swing-state object; its +0x18 float drives quaternion rotation in 0x00256C50 |
| 0x74 | 0x68-byte roller actor, with owner and two byte flags |
| 0x78, 0x7C | Left/right ChainHead actors |
| 0x80 | 0x14-byte actor group containing both banks of chains |
| 0x84, 0x90 | Side and front vectors, initialized from axis vectors |
| 0x9C | Lowest allowed Y coordinate |
| 0xA0 | Chain length; constructor sets 500.0f |
| 0xA4 | Roller length; constructor and default init set 400.0f |
| 0xA8, 0xAC | Roller angle and angular update term, consumed/written by 0x00256C50 |
| 0xB0 | Additional side offset, read from placement argument 5 |
| 0xB4, 0xB5 | Ground-contact and ground-search flags; initialized 0 and 1 |

The header has a compiler-checked size of 0xB8. It reconstructs the init method
and field view; it does not reconstruct the complete set of virtual overrides.
The compiler-emitted root-class vtable is consequently not an accepted runtime
replacement and is not part of this proposal's claim.

## Recovered initialization behavior

Argument 7 sets chain length. Each side gets at least one link, otherwise
`int((length + 30) / 60) - 1`; the group holds twice that count. Init registers
each `WanwanChain` child, then copies the quaternion and constructs swing state.

The default lower Y is actor Y minus chain length. A supplied nonpositive
argument 6 restores that default; a positive value is subtracted from actor Y
and disables ground searching. The source preserves the ordinary comparison's
unordered path instead of inventing a clamp. Argument 5 supplies side offset.

The resource helper reads `RollerParam` under `SwingNeedleRoller`. The object
name selects a BYAML entry; optional `ArcName` and `Length` override default
`NeedleRoller` and 400.0f. Init makes two ChainHead actors, creates the owned
roller, binds its rotation field and quaternion, binds the `Damage` sensor to
roller translation, and sets sensor radius to half roller length minus 25.

The actor's switch callback can replace its starting nerve. Init updates the
model positions, appears the roller, connects its execution group, and sets
clipping radius to `sqrt(rollerLength² / 4 + chainLength²)` before the final
appearance helper. These are assembly-grounded operations, not replay results.

## Local function-name proposals

The following local map edits name existing whole function rows without changing
their boundaries, types, pools, or ranks. Only the root has a proposed game-class
name. The namespace `swing_needle_roller` and its helper class names are explicit
local reconstruction labels, not recovered original symbols.

| Existing start | Local declaration / compiler symbol |
| --- | --- |
| 0x001855C0 | `SwingNeedleRoller::init`; `_ZN17SwingNeedleRoller4initERKN2al13ActorInitInfoE` |
| 0x002741D8 | `swing_needle_roller::ChainModel` constructor; `_ZN19swing_needle_roller10ChainModelC1ERKN4sead14SafeStringBaseIcEEPKcS7_` |
| 0x00274218 | `swing_needle_roller::SwingState` constructor; `_ZN19swing_needle_roller10SwingStateC1ERKN2al13ActorInitInfoE` |
| 0x001D1178 | `swing_needle_roller::RollerModel` constructor; `_ZN19swing_needle_roller11RollerModelC1EPKcPN2al9LiveActorE` |

Independent callee evidence: 0x002741D8 calls LiveActor(SafeString), installs
four actor-interface pointers, stores incoming r2/r3 at +0x60/+0x64 and clears
byte +0x68. 0x00274218 calls NerveExecutor, reads placement arguments into
+0x10/+0x14/+0xC, derives timer +8, and initializes its nerve. 0x001D1178 calls
LiveActor(const char*), installs its actor interfaces, stores the incoming owner
at +0x60, and sets bytes +0x64/+0x65. The selected caller allocates respectively
0x6C, 0x1C, and 0x68 bytes, matching the compiler-checked declaration sizes.

All remaining unrenamed imports use address aliases for existing function rows.
Their names are not semantic-identity claims. Several already have known main
names, including argument readers, invalidateClipping, and setNerve; those aliases
still resolve the same existing rows. No standalone guessed function is created.

## Separate data-ownership proposals and blockers

The group initializer calls established LiveActorGroup construction and then
writes address point `0x003D664C`. Other independent actor initializers at
0x001248FC and 0x0030D024 use that same address point. The sole function entry
is the accepted `LiveActorGroup::registerActor` at 0x001CAB14. The two preceding
words are ABI header zeros. Thus the observed complete table is
`0x003D6644`–`0x003D6650`, 12 bytes. The compiler's ChainGroup table is likewise
12 bytes, with its only function relocation at +8, and the root references it
with addend +8. Current adjacent rows start at 0x003D6640 and 0x003D664C;
neither establishes the actual whole-table start. The local C++ type label does
not identify the original group specialization's source name.

The callback table address point is `0x003D5D0C`. Independent clone
0x0039C544 allocates 0x10 bytes, stores this address point and copies the parent
pointer and two member-pointer words. Independent invoker 0x0039C584 decodes
the ARM member-pointer virtual bit and this adjustment before branching. The
table's entries are those invoker and clone addresses, preceded by two ABI
header zeros. The observed whole table is therefore
`0x003D5D04`–`0x003D5D14`, 16 bytes. Current adjacent rows start at 0x003D5CFC
and 0x003D5D0C. The ordinary FunctorV0M specialization emits a 16-byte table
and references it with addend +8. Naming the existing address-point row as the
whole table would incorrectly add another eight bytes.

The eight-byte member-pointer object at existing row 0x003BF4A4 names callback
0x00185B40 with zero adjustment. That separate callback tests nerve 0x003F2BF8
and changes it to 0x003F2BFC. Independent static initializer
0x00389F18–0x00389F40 initializes those adjacent four-byte nerve objects.
Those existing data rows need no boundary change to serve as address imports.

The proposals above require main's separate data-ownership review. The final
local map retains its original boundaries. No table repair is part of source
publication, and no canonical acceptance is inferred from a diagnostic repair.

## Compiler evidence and remaining work

Source checkpoint: `df43543` (ordinary C++ and header only).
Direct object SHA-256:
`3bb3f6e87ec78a84ec34d086cdaa1e81a8c8faefc5c49bd3e2b2c1ae7c53044b`.
Complete 1,400-byte unrelocated section SHA-256:
`b6e50e65b34dad9a0b4b73fa5aa5d06e13b3ef890f5e892343f997294a1615c6`.
ARMCC 791 executable SHA-256:
`d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`.
The normal build's provenance record reports stable inputs.

The owner-provided EU executable was verified before analysis as SHA-256
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
It was read only. No game data is included in this branch.

A read-only instruction comparison already exposes meaningful source differences:
the ordinary aggregate quaternion assignment emits a block copy where retail
uses four sequential field loads/stores; the local frame is 0x58 versus retail
0x40; callback member-pointer loads are separate words versus retail LDRD/STRD;
and literal-pool placement differs. These remain future source/ABI hypotheses,
not attempted structural forms or byte-equivalence claims. The lane stops at
this bounded checkpoint because the canonical data dependencies require review.

Final clean verification on 2026-10-01 at 19:34 UTC: unchanged
`python make.py eu -ca` exits 0, links `RE-Pepper.axf`, and exports `code.bin`.
The rebuilt canonical object retains exactly the object hash above. The checker
again exits 1 with the same source-closure rejection. The EU oracle hash is
unchanged. After that check, even the four temporary function-name aliases were
removed: the local map is byte-identical to HEAD. Only the three intended
source/header/report files differ in the committed branch from baseline.
