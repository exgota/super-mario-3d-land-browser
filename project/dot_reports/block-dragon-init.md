# BlockDragonGenerator initialization and layout

## Result

One supporting callback matches: `BlockDragonGenerator::startAppear`, locally named at `0x001931B4–0x001931E8`, **52 complete bytes** including the literal pool at `0x001931E0`. The unchanged project checker reports:

```text
U -> O: The complete source-generated function interval matches byte for byte.
```

The requested large target, `BlockDragonGenerator::init`, `0x00192C80–0x001931B4`, **0x534 / 1332 complete bytes**, remains a **NonMatching proposal**, guarded by the project's `NON_MATCHING` convention. The final ARMCC section is 0x540 / 1344 bytes. Its canonical check stops before a byte comparison:

```text
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

This is a rejection, not a measured near-match. Neither the initializer nor its class tables add exact credit. No gameplay, allocation-failure, or runtime equivalence claim follows from the source reconstruction.

The branch starts at exact main `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`, refreshed through GitHub immediately before selection at approximately 19:05 UTC on 2026-10-01. All relevant rows were U. The initial callback check used source checkpoint `50e6f69`. Final source checkpoint `f4df7ca` was clean-built with `python make.py eu -ca`; it linked and exported successfully. At approximately 19:20 UTC the callback independently rechecked as `O -> O: The complete source-generated function interval matches byte for byte.` The initializer still produced the rejection shown above. The EU input SHA256 is unchanged:

`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

## Why this is BlockDragonGenerator

The pre-existing clean header declares BlockDragonGenerator and its init/startClipped/endClipped methods. Independent actor factory `0x00397AE4–0x00397B1C` loads allocation size **0xC4** at `0x00397AF0`, allocates at `0x00397AF4`, and calls constructor `0x00193484` at `0x00397B0C`.

That constructor calls the established MapObjActor constructor, installs primary table point `0x003CEBB0`, and stores secondary points `0x003CEC18`, `0x003CEC24`, and `0x003CEC38` at object offsets 4, 8, and 12. The primary init slot at `0x003CEBB4` contains `0x00192C80`. Constructor storage extends through the float at **+0xC0**, independently confirming the 0xC4 allocation. The inherited opaque header was only 0xC0 bytes; this proposal corrects it.

The initializer's own strings independently agree with that identity: BlockDragonGroup, BlockDragonHead, BlockDragonTail, BlockDragonBody, and BlockDragonQuestionBlock. These are observed actor names. The local C++ child-class and callback method names are descriptive reconstruction names, not recovered debug symbols.

## Recovered object layout

The 0x60-byte LiveActor/MapObjActor base is unchanged. The new generator declaration has the following fields:

| Offset | Source representation | Independent evidence |
| --- | --- | --- |
| 0x60 | opaque helper pointer | Constructor clears it; initAfterPlacement `0x00192A10` allocates a 0x34-byte helper and stores it here |
| 0x64 | actor-group pointer | Constructor clears it; independent clipping/movement consumers traverse its established group fields |
| 0x68, 0x6C | head/tail pointers | Clipping functions `0x00192910`, `0x00192990`, `0x00192B48`, and `0x00192C28` use them; constructor leaves them unset |
| 0x70 | body count | Constructor initializes zero; init records the positive prefix of seven body-type arguments |
| 0x74 | segment spacing float | Constructor stores 104.0; placement on the rail repeatedly adds it |
| 0x78 | move-speed float | Constructor stores 10.0; movement at `0x001932C0` loads it; init reads Arg0 |
| 0x7C | current leading actor | Constructor clears it; movement reads/reassigns this actor at `0x001933E0–0x00193420` |
| 0x80–0x83 | four opaque bytes | Retained unknown storage between independently observed fields, with no claimed type or behavior |
| 0x84, 0x85 | Boolean-sized fields | Constructor stores zero and one as bytes; their roles remain unnamed |
| 0x88–0xA3 | seven signed body types | Constructor stores 1,1,1,0,0,0,0; init reads Arg1 through Arg7 |
| 0xA4 | placement translation vector | Constructor copies the independently established zero vector; initAfterPlacement uses it at `0x00192A40` |
| 0xB0 | clipping-center vector | Constructor copies zero; init passes it to the clipping helper |
| 0xBC | shadow-length float | Constructor stores 900.0; init reads Arg8 and passes it to all child shadow keepers |
| 0xC0 | shadow-offset scale percent | Constructor stores 100.0; init scales child Body offsets by this value times 0.01 |

Only the new generator source uses the added `sead::PtrArray::capacity()` accessor. It returns the existing signed capacity field at offset zero; it changes neither layout nor allocation behavior. Retail uses group +8 for capacity, +0xC for current size, and +0x10 for the element buffer, consistent with the existing LiveActorGroup declaration.

Child constructor bodies independently establish initialized storage after the base. Head and Tail clear +0x60/+0x64; Body additionally clears +0x68; QuestionBlock clears +0x60/+0x64, stores its integer type at +0x68, and clears +0x6C/+0x70. The init allocations are 0x68, 0x68, 0x6C, and 0x74 respectively. Source size assertions encode these observed extents. Only the constructor interfaces and fields required here are modeled; complete child behavior and virtual tables remain unreconstructed.

## Init behavior and source limits

The routine connects the actor to execution, creates its nerve and pose keeper, reads placement/clip settings, and registers a stage-switch callback. If the callback cannot be installed and the actor is still on nerve `0x003F1E50`, it switches to nerve `0x003F1E54`.

It reads speed, seven child types, shadow length, and placement translation; initializes its rail keeper; counts the initial positive child-type prefix; and creates a group with that count plus two slots. Slot zero receives a head, the last slot a tail, type 1 receives a Body, and other types at most 7 receive a QuestionBlock. The source retains the retail common initialization and registration path, including its lack of allocation-failure checks after construction. It adds no validation for type values outside the retail path.

Every child receives its generator pointer, shared creation info, placement quaternion/front direction, shadow settings, and an orientation built from the placement front and negated gravity. The second pass initializes each child rail keeper, accumulates the spacing for the number of following children, puts it on the rail, and updates pose. The generator then initializes clipping and actor-related registration and appears.

The retained ascending spacing loop uses one floating-point accumulator in its compiled output. Retail instead splits additions between two accumulators and adds them at the end. Form 2 reproduced that split but grew the complete function by 12 bytes; no behavioral correction was established for the constructor's fixed 104.0 spacing, so the smaller form is retained. These accumulation orders can round differently for other floating-point inputs. No bounded replay or general equivalence proof was performed, and the initializer's behavior remains unverified. The source keeps the normal Body assignment: retail `0x001931A0` branches to `0x00192EF8`, the shared `mov r6,r0`, before common initialization.

The address-encoded callees remain external functions, not invented implementations. In particular `0x00250D28` is an eight-byte ActorInitInfo wrapper falling through to the placement-quaternion reader at `0x00250D30`; `0x00260F3C` similarly feeds the following rail-position code. Their existing mapped intervals are left untouched.

## Callback and data ownership

The 52-byte callback at `0x001931B4` only tests nerve `0x003F1E50` and, on equality, sets nerve `0x003F1E54`. Its own two literal words at `0x001931E0/+4` independently establish the two objects. The matching callback uses only existing named al function imports and these existing address-encoded data rows. It needs no class-table import or data-boundary change.

The large initializer needs two additional complete data identities that the current map does not represent:

1. The derived actor-group table has two zero ABI-header words at `0x003D6668/+4`, address point `0x003D6670`, and the established `LiveActorGroup::registerActor` at that slot. The complete table is **0x003D6668–0x003D6674**, 12 bytes. Its immediate neighbors have points `0x003D6664` and `0x003D667C`, independently referenced by literals `0x00303C5C` and `0x0015CF78`. The current rows start at address points `0x003D6664`, `0x003D6670`, and `0x003D667C`, so naming a current row as the complete table would shift its ABI identity by eight bytes. The target establishes this group specialization's role; a source-original template name has not been recovered.
2. The callback table has two zero header words at `0x003D5D64/+4`, then invoke `0x0039C7F4` and clone `0x0039C7B4` at address point `0x003D5D6C`. The complete table is **0x003D5D64–0x003D5D74**, 16 bytes. Independent clone `0x0039C7B4–0x0039C7F4` allocates 16 bytes, installs this exact point from literal `0x0039C7F0`, and copies the receiver plus two-word member pointer. Invoke decodes the ARM member-function pointer and tail-calls it. The following table has its own two-zero header at `0x003D5D74/+4` and invoke at `0x003D5D7C`. Current rows again begin eight bytes too late for whole-table names.

The compiler produces ordinary 12-byte and 16-byte vtables for these classes. No local address alias was attached to either mismapped row. No table or function boundary was changed. The final init also references its compiler-generated member-pointer constant section; its layout/import identity must be reviewed once whole-table identities are repaired. The target constant at `0x003BA584` contains callback address `0x001931B4` and zero adjustment.

The generator's complete 152-byte table likewise begins at `0x003CEBA8` and ends at `0x003CEC40`; its installed points are independently established by the constructor above. The following actor constructor independently installs point `0x003CEC48`, loaded from literal `0x00193C84`, and stores that primary pointer at `0x00193C54`. Its two-word header therefore begins at `0x003CEC40`. This table is not imported by the matching callback, and no constructor match is claimed.

## Bounded attempts

Three structural init forms were compiled, below the cap of eight. A preliminary syntax correction gave abstract Nerve imports a concrete declaration; it did not yield a compiled candidate. No compiler flags changed.

| Form | Committed source | Complete init section | Observation |
| --- | --- | ---: | --- |
| 1 | `becbaa1` | 0x540 / 1344 bytes | Explicit member-pointer constant and ascending spacing loop; ARMCC peels/unrolls type and child loops unlike retail |
| 2 | `ae46194`, callback added in `50e6f69` | 0x54C / 1356 bytes | Direct callback expression and countdown spacing loop; retail-like floating accumulator split, but loop peeling, stack lifetimes, register choices, and pool order still differ |
| 3, retained | `f4df7ca` | 0x540 / 1344 bytes | Direct callback expression, restored ascending loop, and the separately exact callback; smaller supported initializer with explicit floating-point-order limits above |

The first check with only the init name applied rejected the unestablished Head constructor symbol. After the constructor identities below were recorded locally, all init forms rejected the non-branch unresolved table relocation. The added callback body then passed its own canonical check. Further init code-generation search is parked because data identity is independently blocked and additional loop/lifetime hypotheses would be speculative.

## Diagnostic names, checker, and reproduction

The following names were applied locally to existing function rows, with all starts, pools, ends, types, and sections unchanged. No map change is included in the branch. Only the checker set the callback's local rank O.

| Existing start | Local source name |
| --- | --- |
| 0x00156F60 | `_ZN15BlockDragonBodyC1ERKN4sead14SafeStringBaseIcEE` |
| 0x0015743C | `_ZN15BlockDragonHeadC1ERKN4sead14SafeStringBaseIcEE` |
| 0x00157914 | `_ZN15BlockDragonTailC1ERKN4sead14SafeStringBaseIcEE` |
| 0x00192C80 | `_ZN20BlockDragonGenerator4initERKN2al13ActorInitInfoE` |
| 0x001931B4 | `_ZN20BlockDragonGenerator11startAppearEv` |
| 0x001A8EB4 | `_ZN24BlockDragonQuestionBlockC1ERKN4sead14SafeStringBaseIcEEi` |

For callback acceptance, only its 0x001931B4 name is needed. The parent must review names and recheck rather than importing local map state.

```sh
. ./development_environment.sh
python make.py eu -ca
python tools/check.py _ZN20BlockDragonGenerator11startAppearEv --object build/eu/obj/Game/backup/src/Enemy/BlockDragonGenerator.o
python tools/check.py _ZN20BlockDragonGenerator4initERKN2al13ActorInitInfoE --object build/eu/obj/Game/backup/src/Enemy/BlockDragonGenerator.o
```

The compiler is the configured ARMCC 4.1 build 791 through the approved wibo. Default Game flags include MPCore, fast floating-point mode, O3/Otime, split sections, and forceinline. No standalone compiler output, edited object, padding, inline assembly, leaked SDK/source, or source-shaped byte array was used.

Frozen source SHA256 values:

- `Game/backup/src/Enemy/BlockDragonGenerator.cpp`: `3ac129cf072a77af0689d7d5fce44532222d95fee51e180e8c5c4e3849fdb46a`
- `Game/backup/include/Enemy/BlockDragonGenerator.h`: `435c3a985a30536b7a97dacfbf1a0282f43aab738f52effc7c4f59733b5d2588`
- `lib/sead/include/container/seadPtrArray.h`: `715908fa6cd6dfd137225ac675c23109b14f840bf544d2548f31390042791d6f`

Unchanged checker SHA256 values:

- `tools/check.py`: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- `tools/low/checkExactBytes.py`: `aef81a3a21c49bb17b8a19f139d02607899257e04dbe461ece2992cc726fc2e2`

Canonical object SHA256 at source checkpoint f4df7ca after the clean build: `9934b522a81864c846e06266f7346930b7dc29c265c232486cdc76cbb6bc7d84`. Its recorded provenance is `build/eu/obj/Game/backup/src/Enemy/BlockDragonGenerator.provenance.json`, generated by `tools.pypstem.stepBuild`. Raw compiler init-section SHA256 is `14f167606b7b1187eea25ac84e4c13cda3c443b5e6e1d4d48159108f1665ca44`; callback-section SHA256 is `5b56e5061acba7f071427c28ecdbfcf2a24e829b69c9f50af8c55ff1902e1b7f`. These section hashes contain unresolved compiler relocations and are provenance identifiers, not independent matching claims.
