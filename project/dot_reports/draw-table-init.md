# Draw-table initialization proposal

Existing named target `_ZN2al22ExecuteTableHolderDraw4initEPKcPKNS_12ExecuteOrderEi`, 0024B168..0024B56C, 1,028 complete bytes with an interleaved 92-byte string pool at0024B49C..0024B4F8. Main f1d2bc5832c6086129106122e5606e7eb73ad1bb was fetched before selection; the row was U. No boundary or original byte changed.

## Retained result

Zero exact credit. Retained committed source b28b517 canonically emits 1,028 bytes and remains m with 423 differing byte positions. The difference count is diagnostic, not progress percentage. The final clean `python make.py eu -ca` compiled, linked and exported on the fixed main build pipeline without a warm retry or local tool patch.

Command:
```
.venv/bin/python tools/check.py _ZN2al22ExecuteTableHolderDraw4initEPKcPKNS_12ExecuteOrderEi --object build/eu/obj/lib/al/src/Execute/alExecuteTableHolderDraw.o
```
Final output: `M -> m: The linked candidate differs from the unchanged original interval.`

Checker frontend blob7c0ccd93b7387a2f9afa9b304b5254f34149b318; exact-check implementation blobce8fc0a5d747c1521d84a1ca1fbeaeab09e3bb47 from f1d2bc58. Its complete-extent checks remain unchanged in purpose; only the main-owned trailing-pool code-symbol fix is present. No locally patched main was used.

All three existing exact controls reverify O -> O on the clean rebuilt object: constructor (88 bytes), tryRegisterUser (108), and tryRegisterFunctor (108). These are existing matches, not new contributions.

## Seven bounded source forms

| Form | Concrete hypothesis | Complete bytes | Result |
|---|---|---:|---|
|1,70f0d5b|Direct loops, partial storage allocations, genuine address-named constructor calls, ordinary postincrement appends|1028|m|
|2,a7998c9|Separate buffer store from count increment to mirror target reload order|1036|M|
|3,b1c7573|Four repeated capacity/count/buffer triples represented by private inline containers|1040|M; three existing controls O|
|4,c19ae4e|Real C++ constructor-call ABI using provisional address-coded class identities at independently observed constructor rows|1036|M; no improvement; provisional identities withdrawn|
|5,adae5e1|Shared two-name count helper, analogous to independently visible one-name counter|1052|M; category literals hoisted into saved registers|
|6,52ca524|Category-local count helpers keep literal addresses in loops|1036|M; different count/entry register assignment|
|7,afd0e1a|Early selected-pointer lifetime in first model branch, as suggested by target's separate join|1036|M; no useful change|

Restoration b28b517 is an additional physical clean compile, not an eighth new form. The smallest original proposal is retained; no more cosmetic permutations are warranted absent new source-boundary evidence. No volatile accesses, inline assembly, synthetic helper addresses, object edits, padding, compiler changes or oracle modifications were used. No provisional C++ constructor identity remains in the local map or published source.

## Independent data and call evidence

The existing constructor and ExecuteDirector caller corroborate the 0x50-byte table object. Its public registration routines independently use user count/buffer at34/38 and functor count/buffer at40/44. The recovered init establishes four repeated capacity/count/buffer triples at18/1C/20,24/28/2C,30/34/38,3C/40/44, plus the master count/capacity/buffer at0C/10/14. `_0` is the borrowed table name; model/layout draw-info pointers are at48/4C. Header changes type only the observed pointer words, preserving names, offsets and size.

The six selector strings are ActorModelDraw, ActorModelDrawModelCache, LayoutDraw, LayoutDrawBottom, Draw and Functor. Unknown types still append a null master entry. Model and layout capacity counts combine their two categories; the other two use independently observed counter00242CCC. Classifier00242CC4 reads order+4 and tail-calls the accepted string equality helper. ExecuteOrder is the existing four-word name/type/capacity/extra record, stride16.

Independent initializer bodies corroborate each allocation size and return-this ABI:
-001E7E00 and001E8AA8 call common001E7518, then install distinct vptrs. That base writes name through00243B94, capacity8, countC, buffer10, unknown14 and draw-info18, and returns its original object. Their minimum complete size is28 bytes.
-001E6078 and001E5FCC initialize capacity/count/buffer plus draw-info14, then distinct vptrs, giving24 bytes.
-001E5F24 initializes the common prefix and capacity/count/buffer through10, giving20 bytes.
-001DC890 initializes the common prefix and clears field8, giving12 bytes.

The final source allocates these observed partial storage shapes and calls the genuine address-named initializers, whose return values are independently observed object pointers. It does not invent original class names, vtable boundaries or imported callable addresses. The 28/24/20/12 sizes are also checked by the replay's independent allocation model.

Only local names on eight existing unchanged rows are prerequisites: fn_00242CC4, fn_00242CCC, fn_001E7E00, fn_001E8AA8, fn_001E6078, fn_001E5FCC, fn_001E5F24, fn_001DC890. Map/rank files are not published. Existing scalar/array nothrow allocator and string-equality symbols remain unchanged. Allocation's arbitrary nothrow-tag argument is not treated as a heap parameter.

## Bounded ARM execution

512 original/candidate pairs passed in Unicorn2.1.4 ARM1176 emulation, creating 2,068 objects, with at most 5,658 instructions per root including callees. The replay executes the actual retail six constructors, their base constructors, category counter/classifier and string-equality body. Only ordinary scalar/array allocation is modeled as deterministic and always successful; its kind, size and order are checked, and fresh storage has a nonzero fill pattern.

Full table/input/heap memory and normalized external-call traces agree. An independent model checks master and category capacities/counts, stable group append order, object name/capacity, zeroed inner buffers, optional draw-info fields and the allocation sequence. Cases cover0..12 orders, capacities0..8, all six categories plus unknown, case-mismatched and empty strings, and null/non-null draw-info settings. It checks preserved registers/SP; the unspecified void return value is ignored. Allocation failure, malformed pointers, reentrancy, aliasing outside valid fresh allocations, concurrency and hardware faults are excluded. This is bounded behavior evidence, not physical hardware, exhaustive correctness or matching credit.

Original interval SHA256d282f3fbfe73aaee39de92d3837d51c5c343d60fc3b5bcfaf943f3a3d274fde0. Canonical linked candidate SHA25671132816bccfa1f7f3ccf0a4fca52472bf5da1914d508e6f7a41ffa46c4c0b08. The replay uses the unchanged candidate.axf generated by canonical check.py; no separate relaxed link was needed because the complete sizes already agree. The script is in draw-table-init-replay.md and contains no game bytes.

Latest main observed before packaging:4f70f6154e8d7e0e8b2d7fe1b78f7c82ab4d1654, which adds a new integer-reader packet. This proposal records its actual f1d2bc58 build baseline; main owns intake revalidation. No exact-match claim is made.

Before publication, the Draw header/source preimages and both checker/build implementation blobs were compared against current main4f70f615 and are unchanged from f1d2bc58. The independently existing ExecuteDirector::init also rechecks O -> O with the refined header. This fourth control remains an existing match, not new credit. The replay extracted from the packaged note reran successfully.
