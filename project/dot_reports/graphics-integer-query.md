# Graphics integer query, root 0038F9F0

This complete guarded NonMatching proposal targets **0038F9F0..00390204, 2,068
whole bytes**, against frozen public main
`56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`. It adds **zero exact roots, zero accepted
bytes, and zero accepted bytes/hour**. Only the integrator may accept it, assign
committed ranks, write the ledger, or move main.

The final clean project build succeeds. The unchanged canonical checker rejects
the committed C++ object's complete section: **2,056 bytes versus 2,068 original
bytes**, exit status 1:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The object defines exactly one nonempty function, `fn_0038F9F0`, with a 2,012-byte
symbol and 44 further section bytes. All source helpers inline. The original
unnamed U row receives a scratch name only during the checker invocation; its
exact original map bytes are restored in `finally`. The proposal changes only its
new source/header and these notes. No shared header, accepted source, map, boundary,
rank, ledger, STATE, tool, compiler flag, or game-data file is submitted.

## Whole-root execution

The final committed, clean-built form passes **66,894 returning ARM execution
pairs**. These compare every byte in the generated 32 KiB state/buffer/output
arena and two four-byte global cells, ordered original-helper entry events, final
FPSCR, successful return, SP, r4..r11, and d8..d15. The total is 2,192,517,744 bytes
compared across repeated fixtures, not that many distinct bytes of game state.
There are 72 original-helper entries and at most 81 executed instructions per run.
No caller-saved integer return value or APSR equality is claimed for this void
output-buffer operation.

The original and candidate each run the entire root. The only callee is an
external tail branch at 00390020 to **00377DAC..00377EB0**, whose actual original
260-byte implementation executes on both sides. It has no further calls. There
are no callback, allocator, GPU, operating-system, or synthetic-callee models in
this replay. Test setup supplies generated state and valid pointers, not modeled
query results.

The selector census runs all 65,536 unsigned 16-bit values and observes 102
supported selectors. Eight generated state variants cover active units 0, 1 and
2; all 32 auxiliary slots; arbitrary copied integer words; byte predicates 0, 1,
2 and 255; depth values 0, 16, 24 and 32; and absent color/depth/secondary objects
inside the original helper. Additional values include 0x10000, 0x7fffffff,
0x80000000, 0xffff0000 and 0xffffffff. Float fixtures cover positive and negative
zero, subnormals, minimum normal, fractions, negative values and the last tested
float below one, under seven FPSCR combinations covering rounding, FZ and DN.
Two directed-rounding cases that can leave the signed-int conversion domain are
classified as diagnostics instead of ordinary returning cases.

These are bounded observations, not exhaustive equivalence over state, arbitrary
32-bit selectors, pointer aliases, invalid active-unit indices, malformed pointer
graphs, concurrent changes, or all floating-point inputs. Standard C++ portability
outside representable float-to-int conversions is not established. No rendering,
GPU submission, scene behavior, or gameplay claim follows.

## Diagnostics, kept outside the returning-pair count

All **56 diagnostic pairs** agree under their explicit comparison criteria:

- **44 returning float diagnostics:** 42 cases using 1.0, large finite values,
  infinities, quiet NaN or signaling NaN across six float-query selectors; plus two
  normalized -1.0 cases under round-toward-minus-infinity. These compare the full
  arena, globals, helper events, FPSCR and return state, with the ordinary ABI
  checks on return. They describe this ARMCC/VFP execution, not defined portable
  C++ conversion behavior
- **Four returning null-pointer controls:** unsupported selector zero with a null
  output pointer; and selectors zero, D56 and 8CA6 with a null primary state
  pointer. Their selected paths do not dereference that pointer. The same
  returning-state checks apply
- **Eight actual CPU-memory faults:** five null-output cases (B45, B70, C22, D56,
  8CA6) fault on a four-byte write at address zero; three null-primary-state cases
  (B45, B70, C22) fault on four-byte reads at 0x38, 0x4C and 0x590 respectively.
  Both sides report the same Unicorn access kind, address and access width, do not
  return, and retain identical arena/global bytes, events and FPSCR up to the fault.
  Fault PC, stack scratch contents and interrupted callee-saved registers are not
  equivalence criteria

There are **zero model rejections and zero instruction-budget exhaustions**. Every
run is limited to 10,000 instructions. A rejected or nonreturning run would not be
counted as an ordinary passing pair. The eight faults are deliberately separate
from successful return behavior, even though their compared outcomes agree.

The combined suites execute **469 of 470** original instruction positions outside
embedded data. The unvisited instruction, **0038FDAC**, is the default pop after
an unsigned less-than-13 jump-table guard. Its enclosing comparisons already
exclude equality and values greater than 13 on this path; the first dispatch also
places the selector above 668D. It is therefore a redundant exit for that bounded
index path. This structural explanation and the selector census are coverage
evidence, not a proof of exhaustive input equivalence.

## Extent, fields and closure

The approved local EU image hashes to
`e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.
The full root hashes to
`470e6f8e9594cb2ca47d385393fe7e3793ccab05f9ef8f653d75d0c8dffd090b`.
Its 470 instructions occupy 1,880 bytes. Embedded data consists of a 40-byte
literal island at 0038FCEC..0038FD14, two 52-byte jump tables at
0038FD3C..0038FD70 and 0038FDB0..0038FDE4, and the 44-byte final pool at
003901D8..00390204. Every immediate branch remains inside decoded code except
the one documented external tail; every PC-relative literal load points into an
identified island; every table target is an instruction address. Pre-island
terminators prevent fallthrough. None of these regions was dropped from the
canonical extent check.

A whole-image direct-call scan finds exactly two direct callers, 0024C168 and
0024C174, both within 0024C134..0024C3F0. Inspected instructions load query values
8CA6 and 8CA7, pass separate stack output buffers and do not consume r0 afterward.
This supports an integer output query with a void interface. It does not prove an
original exported API name, nor exclude indirect callers. All names here describe
observed behavior and remain provisional.

The root imports only the four-byte pointer cell **003E3154** and existing helper
**00377DAC**. Both already have unchanged map rows. The helper independently reads
the existing four-byte cell **003E2E3C**; this is initialized only in replay and is
not a new source import or definition. No data object, table, allocation extent,
or binary contents are reconstructed through a placeholder definition.

`StateView` is a dedicated access-offset view through byte 5C3. Every named field
has a compile-time offset assertion. Copied scalar words, two- and four-element
outputs, booleans equal to exactly one, active texture bindings at 5C/68, the
32-slot auxiliary array at 74, and the binding-set word at F4 follow observed
accesses. Normalized float vectors at 560 and 590 compute
`(value * 4294967296.0f - 1.0f) * 0.5f` before integer conversion. Other float fields
convert directly. The view does not claim that unknown gaps are arrays of any
particular original type, or that it describes the complete enclosing allocation.

The SDK module's existing ARMCC 4.0/902 configuration is used because this root
queries the same graphics-state family as adjacent pending graphics proposals.
That is a provisional module assignment, not proof of the original compiler
build. No 894 compiler was downloaded and no compiler flags changed.

## Declaration compatibility

A fresh source/header scan covers **121 local worktrees** and finds 24 relevant
files. Main 56e9 has no declaration of either query entry or 003E3154. This source
uses the compatible proposed declaration
`extern "C" retail_graphics::RenderControl* dat_003E3154;` without defining
RenderControl or changing the shared pending header. A host C++ syntax check
includes the actual completed graphics-integration and texture-binding-state
headers together with this new header and its C declarations; it passes.

The current graphics-integration, shader-initializer and texture-binding-state
proposals share that opaque pointer. Historical float-state-root, integer-state,
packed-state, root-3910c0, shader-full and shader-partial proposals still contain
other pointee types. Those declarations remain incompatible until their owners
coordinate a correction. This lane does not modify them or claim combined
integration. No other scanned source/header declares 00377DAC or 0038F9F0.

## Attempt history and final validation

Three substantive forms were investigated, without cosmetic register, padding,
volatile, assembly, or compiler-flag tuning:

1. `23fb88016a46491ba567e20e2d45993a56c26f48`: guarded direct writes, 2,056 bytes. The
   canonical checker rejects the extent. Whole-root replay finds that the source
   swapped the constant outputs for selectors 8B9A and 8B9B. The failed object,
   checker result and first divergence are retained locally
2. `60c088b`: correct 8B9A -> 1401 and 8B9B -> 1908 using the original PC-relative
   loads. The extent remains 2,056 bytes. Whole-root replay passes
3. `17e0703`: a grounded common scalar-output exit reflecting the original shared
   store. It passes the same execution suite but emits 2,040 bytes and still fails
   canonical extent. No fourth hypothesis had enough evidence to justify a form

The final source commit **bd9485855a619f6344c03e336573c92619718ea4** restores the
replay-corrected direct-write source from form two. The NON_MATCHING guard is
present throughout. Final `make.py eu -ca` passes in **20.527083 seconds**, from
08:20:59.200456 to 08:21:19.727526 UTC on 2026-10-02. The canonical check,
diagnostic link, final replay, preservation and declaration checks then run
against that final object. Final replay takes **14.319865 seconds**.

Two setup/reporting failures also remain explicit. The first source generator
failed because the new include directory did not yet exist; it produced no source,
and the shell continued into a pristine build. A later manifest command assumed
an optional `data/ver/eu/config.json` existed; it did not. The corrected manifest
records that absence. Neither failure changes an implementation result or hides a
checker rejection. Work began at 08:08:50 UTC; report/package time is part of the
wall interval, not accepted matching throughput.

## All-prior-object preservation

The final build contains **185 physical objects**: **183 prior C++ objects**, this
one new C++ object, and one generated scaffold object. All 183 previous objects,
including ones without canonical checked definitions, preserve every allocated
section byte, all other section attributes/content, symbols and resolved
relocations, compiler identity, normalized compiler command, and all **376
recorded inputs**. All **633 tracked baseline files** are byte-identical.
Repository prefixes in STT_FILE and nonallocated `.comment`, and consequent
symbol/string-table indices, are the only ordinary-object normalizations.

The generated scaffold is audited separately. It adds exactly function aliases
for **00377DAC and 0038F9F0**, plus one zero-filled data alias for **003E3154**.
Every old section, symbol, relocation and per-function CFI byte remains unchanged.
Generated frame-CIE ordinal names normalize only to their owning function. The
source delta is exactly these normal aliases; compiler/command and all other
provenance inputs agree. No alias receives implementation or exact credit.

This is **equivalence-backed preservation**, not a fresh 879-definition checker
pass by this lane. The independently completed pristine 56e9 CLI acceptance run
passed all **859 roots / 879 actual definitions**, clean build exit zero, in
326.578492 seconds. Its unchanged report SHA-256 is
`fe3d675d340993d6cf2119cb61bc1b3d727b4523713a46e1f761978ce0286026`.
The integrator must perform its own acceptance and regression gate.

The ordinary compact linked image remains byte-identical to pristine 56e9,
SHA-256 `54a7660fab711fe149c0c34682324dda6f15f2e6e32bd8f238227bebda956763`.
The root remains U in the map and this image may select its generated stub.
Therefore the actual C++ object is linked separately at the root address only for
execution diagnostics, with both imports resolved to their existing map rows.
That diagnostic AXF hashes to
`478075ec3df92f3d50ec8e40bf2f68b184eeb1f04e8ca41c05a023eafacac2a0`;
its code section hashes to
`31f203fe10ba82acfbc8198320364672b37730286ac2c008077dd19f81c667d3`.
Neither diagnostic linking nor replay is substituted for the canonical checker.

Reproduction scripts and the complete generated-fixture procedure are in
[graphics-integer-query-reproduction.md](graphics-integer-query-reproduction.md).
Ignored local evidence is under `build/graphics-integer-query/`. Only clean-room
C++/headers and source-based notes are included in this proposal; no binary,
original disassembly, game data/assets, credentials, or third-party SDK sources
are submitted.

## Final input and tool hashes

- `lib/CtrSDK/include/retail/GraphicsIntegerQuery.h`: `3de7295ebf69023614a455fc2c5de05dc6d301f144319987065d4f26945d44d3`
- `lib/CtrSDK/sources/gx_IntegerQuery.cpp`: `08ebfbe99071ddd6b25de24def27f996f474204b717464dadaa82b4da9b87315`
- `data/ver/eu/map.csv`: `0a618a11317104c7cae72a2c4ea48472a6959034cf421e51ca7f343f72026ddc`
- `data/config.json`: `5d41e617eb5b26374ed60b5383a0940ce0ce7d046344f7f815b7564144822c45`
- `tools/check.py`: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`
- `tools/pypstem/stepBuild.py`: `b7be5977a0a1afc339eb4c9087b447cdc25b1c42daa3fe23af4b765008b0aac0`
- `build/eu/obj/lib/CtrSDK/sources/gx_IntegerQuery.o`: `a3c54211042ab4628689d60a4640ad7b73b259c17ad7ab14d2ed8eced6d98a11`
- `build/eu/obj/lib/CtrSDK/sources/gx_IntegerQuery.provenance.json`: `f19ed5c2626a55699a43088d6f8dc6af898e87c21e066e06ba7f9f98fa00fd4d`
- `build/eu/code.bin`: `54a7660fab711fe149c0c34682324dda6f15f2e6e32bd8f238227bebda956763`
- `data/compilers/4.0/902/bin/armcc.exe`: `e6aca4ebc280a54065406bcc3a0ae3d9f4d0ac5a4032c3a193adb836c72e70fe`
- `data/compilers/4.0/902/bin/armlink.exe`: `f511d2d087fe0cad6f604ff774b4d75476a7d4949c100ea082eac03a368e967d`
- `data/compilers/4.0/902/bin/armar.exe`: `167e21f8978742de70171cb44fde79f0e39f3521620de20a97eb16de9606a393`
- `data/compilers/4.0/902/bin/fromelf.exe`: `aabb1ad390b8244c74580cb9fa4acacb2c838e318b8e13a62bccd97e5e79e930`
- `data/compilers/4.1/791/bin/armcc.exe`: `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`
- `data/compilers/4.1/791/bin/armlink.exe`: `b9cffb71d7b58f3fd33fa8c4c8af884b75689bb5a9e9014309a08146bba676dc`
- `data/compilers/4.1/791/bin/armar.exe`: `68088194227d542623663a0a675a5d04edab06ae51117f42e6c5b60e65d01f45`
- `data/compilers/4.1/791/bin/fromelf.exe`: `01dd61d20003cd6ceea02f1e7880980aa34d954428e24a676187da3c54098f5e`
- `data/compilers/wibo`: `aee836280b3d7c80c2031410219c907c9824c11be6b128d0744655de0f22280b`
