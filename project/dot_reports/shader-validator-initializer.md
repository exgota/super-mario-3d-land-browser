# Shader validator initialization reconstruction

This is a complete **NonMatching source proposal** for
`__shv_initializeShaderValidator`, unchanged EU interval
`00108690..00108F74` (2,276 complete bytes). It adds **zero exact functions and
zero exact bytes**. Branch `dot/shader-validator-initializer` starts from frozen
`754f99a30a337756df5aa01c28b4ed6a977fecd5`. Final source checkpoint: `10e5dd1`.

The normal project build and final clean `python make.py eu -ca` both compile,
link and export successfully with the map unchanged and the proposed root U.
That link is the project's compact scaffold; it does not link the new root's
actual body. The unchanged committed-source checker reports:

```
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

The missing data identity is `dat_00420F4C`, imported by an ordinary relocation.
The current map has no row for it. The checker stops on closure, so it does not
produce a canonical size/differing-byte result. Independently inspecting the
unedited ARMCC section measures 2,364 bytes, 88 larger than the original whole
interval. Neither the size measurement nor diagnostic replay overrides closure.

## Source and observed behavior

The new translation unit and dedicated declarations are
`lib/CtrSDK/sources/shv_InitializeValidator.cpp` and
`lib/CtrSDK/include/retail/shv_InitializeValidator.h`. The body uses the existing
`NON_MATCHING` convention. Its caller at `00107208` passes the state-validator
allocation plus 12; the root writes dirty flags `0x03FDC3FC` to that word.

It emits the original ordered register defaults, initializes nine cached global
words, clears seven 256-word lighting tables and 96 four-word uniform slots,
then allocates and zeroes a 16 KiB temporary. It uploads that zero data through
two original multi-write calls, conditionally releases it, emits remaining
fixed defaults, and traverses 189 cached validator registers. Enabled entries
combine their cached value with their lookup register and byte-enable mask.
There are seven static fill-call sites, executing 277 fill calls per root.

The root consumes the lookup at `00420F4C`. The separate metadata evidence
[identifies the actual producer and exact observed lookup/mask spans](shader-validator-initializer-metadata.md).
The source deliberately imports the missing table instead of substituting an
absolute address or claiming a new BSS definition.

`retail/GraphicsGlobals.h` is copied byte-identically from the completed
graphics-declaration integration proposal, SHA256
`48a02c17d8de9be153d4c21a4bc1a7cdbf1afcd28cd844add35c1e5303031ca5`.
Its ContextSlots declaration retains the current pointer at offset 8. The new
header forward-declares the existing `retail_program_link::ReleaseSlot` and reads
its observed callback at offset zero; it does not introduce an incompatible
one-word declaration for the existing 16-byte row. A combined ARMCC compilation
including the new header, completed shader tail and ProgramLink declarations
passes. No existing shader or other shared header is edited.

The configured module is CtrSDK with ARMCC 4.0 build 902. The named validator
family, caller, GPU-command callees and shared graphics state establish that
module placement. They do not identify the historical compiler. The original
compiler remains unknown; this work does not discriminate 902 from 791 or 894.
No compiler was downloaded and no compiler flag/configuration was changed.

## Attempts and final-source verification

Two meaningful source forms were committed before their normal project builds
and unchanged `tools/check.py --object` invocations:

| Form | Source commit | Complete section | Checker | Functional replay |
| --- | --- | ---: | --- | --- |
| Initial complete body | `7c0c3cf` | 2,368 (+92) | Closure rejected | Historical 430 sequences pass |
| Conditional cached-global loads | `10e5dd1` | 2,364 (+88) | Closure rejected | Final 430 sequences pass |

Form two keeps cached-global loads inside their available-space branches, as in
the original. It is a control/data-access reconstruction, not a register,
volatile, padding or opcode workaround. Work stops at two forms because the
independent identity blocker prevents canonical acceptance. The four-form cap
has not been exhausted. No fifth form or capped-root requeue is involved.

The final-source result is **430 fixture sequences, 850 paired root invocations
and 1,700 root executions**: 420 normal fixtures each invoke the original and
source twice, yielding 840 returning invocation pairs; 10 allocation-fault
fixtures invoke each once, yielding 10 matching fault pairs. The second normal
invocation checks repeated-call state. Fault fixtures stop at the first fault.
Original-manager warmups and all form-one runs are excluded from these totals.

Unicorn's ARM1176 model executes the untouched original root at `00108690` and
the final canonical compiler object linked at `00600000`. The separate
diagnostic symbol file binds original data/callees, including the explicitly
qualified missing BSS table. It never changes the canonical object, dump,
checker or map. The diagnostic link was repeated after the final clean build;
the resulting ELF SHA256 is unchanged, so the recorded replay covers those
freshly rebuilt source bytes.

Each fixture first executes the actual original manager at `001064E4`, which
initializes the lookup, masks and validator defaults. Cases use either those
defaults, disabled/all-enabled masks, random cached words/masks, or specified
flags aliases. Capacity cases cover 0, 4, 8, 12, 64, 100, 512, 4096, 10880,
10900, 10920, 13000, 27000, 28500 and 131072 bytes. Full all-enabled executions
emit 29,752 bytes per call and 59,504 across two calls. Small-buffer cases compare
the entire mapped buffer, including any writes beyond the nominal end; their
allocation contains guard space. The original helper cursor-saturation behavior
is preserved. The flags word aliases either the first command word or the first
cached validator value in two fixture groups.

All reached fill, multi-write and memory-clear bodies execute original code.
The only models are the application allocation callback (returns an aligned
mapped 16 KiB block or null) and optional release callback (records its arguments
without unmapping memory). Their arguments and order are compared. Missing and
failed allocators fault on the original clear through address zero; both roots
produce the same first fault. This is observed behavior, not a recovery claim.

Comparisons cover the complete validator, flags buffer, allocated block,
command buffer, surrounding graphics globals and BSS lookup/masks, callback
events, helper-call counts, cursor, normal return, stack pointer and r4-r11.
Every root has a 400,000-instruction cap and all normal cases reach the real
return. No instruction-cap outcome counts as a pass. No cycle timing, GPU,
rendering, gameplay, asynchronous mutation, allocator reentrancy, arbitrary
pointer/metadata corruption, 32-bit address wraparound or unrestricted alias
equivalence is claimed. Allocation/free lifetimes are modeled as stated.

## Preservation and delivery

The frozen baseline report checked 733 prior accepted roots and all 753 actual
canonical definitions. Its SHA256 is
`c208f39fadf49e66c5c98c97b445881ba2e51ade07b5724610cf0309175ec028`.
This proposal verifies unchanged contents for 473 old tracked source, header,
data/configuration and build-tool files, unchanged provenance inputs/compiler
hashes/normalized commands, and equal allocated sections, bytes, symbols and
relocations for **all 178 old canonical C++ objects**. Every one of the baseline's
753 checks maps to these preserved objects. This is exhaustive preservation
equivalence, **not a new 753-check run**.

The newly discovered imports make the ordinary build generator add scaffold
aliases/data stubs. Those generated C stubs are excluded from canonical C++
matching evidence and provide no source/data credit. The final normal clean
build still links. No generated build stub, compiler, object, executable,
extracted table or game binary is committed. Diagnostic result JSON is included
as explicitly labeled evidence notes.

The accompanying evidence directory contains the final replay results,
historical form-one results, independent metadata audit, preservation manifest,
build/check/toolchain/source hashes, exact checker outputs and reproduction
scripts. Scripts are evidence notes; they do not replace or modify project
tools. Begin reproduction at the repo root with `development_environment.sh`,
build normally, run the canonical checker, then use `link_diagnostic.py` before
`replay.py`; run `metadata_audit.py` separately and pass the frozen baseline
checkout to `preserve.py` when needed. Only the source/header/notes family is
submitted, as an apply-clean patch against the frozen base. Publication belongs
to the coordinator.
