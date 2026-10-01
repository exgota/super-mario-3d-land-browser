# Shader binary ingestion reservation

**Integration qualification:** this branch uses the shader-family word-array declaration for the shared context slots. It conflicts with the separate state-writer header if directly combined; see [the common declaration proposal and required integration checks](graphics-shared-declarations.md). Separate branch validation is not a combined integration claim.

Branch `dot/program-state-root`, based on main `57f902421f6874f132d5ea971d1aa5b224c5a528`, reserves the unnamed U root `0x002478D8..0x00248A48` (4,464 bytes). Initial reservation was committed as `d0988cd`; the completed guarded source/header checkpoint is `09d1771`. This branch claims a bounded NonMatching reconstruction and zero exact bytes.

The independent callers at `0x001D4C54`, `0x002B577C`, `0x002B5858` and `0x002CBA44` supply a five-argument shader-binary upload shape: signed shader count in r0, handle array in r1, format 0x6000 in r2, binary pointer in r3, and binary length at caller sp. The body uses count, handle array and binary pointer; format and length are unused. No caller consumes a return value. The address name will be retained until the public API identity is separately accepted.

The body allocates a 0x24-byte shared resource through the existing allocator slot `dat_003E2654`, copies instruction words and the low word of each eight-byte operand descriptor, and allocates 0xE8-byte stage records. It interprets each binary stage's constant, output, variable and string tables, then attaches the shared resource to handles in the context's 512 shader buckets at +0x808. Independent release root `0x0020F690` confirms resource arrays at +0/+8/+0x10, stage pointer ownership at +0x30/+0x58/+0xE0, resource reference count +0x18, and list links +0x1C/+0x20.

Configured module is `lib/CtrSDK`, ARMCC 4.0 build 902, with unchanged project flags. The owner executable hashes to `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Only the private owner dump is used. Source, headers and this report are the branch's permitted changes; map, tools, compiler flags, ledger, STATE and game data stay untouched.

The active linker worker owns `0x00245D50`; its independently recovered stage contract agrees on 0xE8 stride, constants +0x30/+0x34, outputs +0x38, flags +0x54, uniforms +0x58/+0x5C, attributes +0x60 and strings +0xE0. Neither worker edits the other's tree. Main owns integration and acceptance. The complete source has now passed the bounded verification below; canonical equality still fails.


## Result and acceptance

The final function is ordinary C++ behind `NON_MATCHING` in `lib/CtrSDK/sources/ShaderBinary.cpp`; its independently reconstructed types are in `lib/CtrSDK/include/retail/ShaderBinary.h`. The configured SDK module compiles it with ARMCC 4.0 build 902. No assembly, copied function bytes, instruction interpreter, fabricated helper address, library-source import or compiler-flag change was used.

Four distinct structural forms were compiled through the project's `make.py eu` step:

| Form | Source commit | Complete section bytes | Result |
| --- | --- | ---: | --- |
| Typed first reconstruction, aggregate zero temporary | fd8709f | 4304 | canonical U -> M |
| Lazy allocation expressions and direct member initialization | 470aa0b | 4300 | canonical U -> M |
| Typed eight-byte operand records and intrinsic bulk zero | 3e6b455 | 4316 | canonical U -> M |
| Shared failure-state control and explicit bucket-chain traversal | 8c1a7d0 | 4348 | canonical U -> M |

`09d1771` aligns the external context slot declaration with the linker/validator proposals (`unsigned dat_003E2E40[4]`) and indents the recovered failure scope. Its entire canonical object is identical to form 4. This is interface cleanup, not a fifth source-form experiment. The complete retail interval is 4464 bytes. The final object has 4336 bytes of ARM function symbol plus 12 trailing pool bytes. The checker rejects the 116-byte size difference before equality comparison, so no differing-byte count or similarity percentage is claimed.

The exact checker output is:

```
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Because the root's existing map row has no Symbol, each diagnostic temporarily set only its Symbol to `fn_002478D8`; the checker alone produced the temporary M rank. Original start `0x002478D8`, pool `0x00248A40`, end `0x00248A48`, type and section were unchanged. The exact original map bytes were restored immediately after each check, and the map is absent from this branch's commits. No source closure rejection remains; all five external imports have existing exact-start rows.

Final `python make.py eu -ca` succeeds from a cleared project object/split build, compiles 42 Game/120 al/2 SDK sources, links and exports. The rebuilt canonical object retains the exact SHA below. `git diff --check` passes. The restored map SHA256 is `ac7eef756278c86c41472bab66291afed57b6bcbed2336eb2b75aae157ff7cca`.

Final source SHA256: `2d1855444c363cea45d76e6d80938d66bb7cd5b338b868bd84e6307e0f457ffe`.
Header SHA256: `098ea91b2a671d1f1917112e0b64c143620ec147458df3f5ee9735792b018505`.
Canonical object SHA256: `c6a6b90414fefbbaae3f93f5ec32a441f23f672daeed52218ea5213a5a459807`.
Complete compiled root-section SHA256: `a50312ab32e56bce12ab94353494923f33814213004b500fc815a8843cf4d193`.

## Bounded whole-routine verification

The final source passes 439 returning original/candidate ARM1176 pairs, with no mismatches in six complete memory regions, allocator/free call sequence and arguments, stack restoration or callee-saved registers. Each executable produces 3382 callback events over this corpus. The harness visits all 1087 retail instruction addresses after excluding the two inline jump tables and literal pools. This counts visited instruction addresses, including predicated instructions; it is not exhaustive path or predicate coverage.

Fixtures include 0–3 binary stages, empty/odd/even instruction and operand counts, count -1/0/1/3, all thirteen allocation positions, callback removal between allocations, absent allocation/free callbacks, old resources with one or extra references, shared old resources, hash collisions, head/middle/tail resource-list removal, zero-length tables, boolean/integer/packed-float constants, ignored constant types and out-of-range integer slots, every output mask for semantics 0–11 and 65535, all attribute and uniform classes, matrix/vector divisibility and component suffixes. Odd singleton constant/uniform tables exercise the pre-unroll paths. Format and supplied length are deliberately varied to confirm that this body ignores them.

The whole original roots `0x0028D1F0` (zero-fill entry), `0x0028BA44` (copy) and `0x0020F690` (resource destruction and unlinking) execute for both sides. Their implementations are not replaced. Only the external allocator and free callbacks are controlled: they return disjoint deterministic addresses or null, record arguments, optionally remove the allocator slot, and clobber caller-saved integer registers. Free callbacks record without freeing mapped pages. Allocated regions start with identical nonzero synthetic contents, so writes to untouched/default fields remain observable.

This is a bounded semantic result for resident, aligned, sufficiently sized binary tables, terminated strings and known shader handles. Shift indices are valid (boolean 0–15, outputs 0–6); valid attribute/uniform ranges keep writes in their allocations. The binary, context, handle records, old resources and new allocations do not overlap, except deliberately shared old-resource references and linked-list pointers. Overlapping allocator results, input/output aliasing, callbacks that rewrite arbitrary input/context state, missing handles, invalid pointers, unterminated strings, oversized/malformed tables and out-of-domain shifts are not validated. No renderer, GPU, real application initialization, hardware execution or gameplay equivalence is claimed. Allocation failure leaks already allocated records exactly as this routine does; no invented cleanup was added.

The complete fixture generator, callback contracts, import-only runtime link and reproduction instructions are preserved in `program-state-root-validation.md`. Its linked diagnostic is produced directly from the canonical unedited object at a private test address. It is behavioral evidence only, never canonical exact evidence.

## Recovered contracts and full control flow

The binary header's word +4 is stage count; the following count words are stage-relative offsets, and the program header begins immediately after that offset array. Program fields +8/+C describe relative instruction offset/count; +10/+14 describe eight-byte operand offset/count. Only each operand's lower word is retained. Every stage header is 0x40 bytes; constant records are 20 bytes, output records eight, variable records eight. String-table offsets are relative to their stage header.

The 0x24 resource contains instruction pointer/count +0/+4, operand pointer/count +8/+C, stage pointer/count +10/+14, signed reference count +18, previous +1C and next +20. A shader handle node is 0x1C bytes: resource +0, stage index +4, handle +8, type +C and collision-chain next +18. The active-context slot is word 2 of the existing 16-byte global at 0x003E2E40. Context shader buckets begin +808 and resource-list head +1008. Root 0x00248A48 independently constructs those handle records; root 0x0020F690 independently consumes and releases resource/stage ownership fields.

Stages have type byte +0, flags bit 0 at +1, geometry bytes +2..+5, input/output masks +6/+8, mask popcounts +C/+10, entry/end words +14/+18, boolean constants +1C, four integer constants +20..+2C, packed float constants pointer/count +30/+34, seven output words +38..+50, output flags +54, uniform pointer/count +58/+5C, sixteen two-word attributes +60..+DF, string-table pointer/size +E0/+E4. The independent linker reconstruction at 0x00245D50 corroborates these records.

Control-flow phases are allocation and copy (2478D8–247AE8), stage header/constant construction (247AF0–247DF0), input/output-mask popcounts and semantic output remapping (247DF0–2481C0), string copy and variable classification (2481C0–248934), stage-completion/error selection (248934–24897C), then handle lookup, old-resource decrement/destruction and new-resource insertion (24897C–248A38). Every early allocation failure returns through the same semantic exit. Stage-table failures break the outer construction loop and leave the newly allocated resource unbound.

Output semantic 9 removes its register from the active output mask and emits no component mapping. Other semantic bases are 0->0, 1->4, 2->8, 3->12, 4->16, 5->14, 6->22, 7->0, 8->18; unknown semantics default to 0. A destination output index counts active preceding registers. Selected components replace one byte of a 0x1F1F1F1F default word. Semantic 4 stops after one selected component, 3/5/6 after two, 8 after three. Specific component numbers enable corresponding output-flag bits.

Uniform records are five words: type token, first register, component offset, string offset and register span. The producer computes span using signed integer promotion from the two u16 indices; the consumer uses its bits as an unsigned loop bound. Floats use first-register minus 0x10 and vectors/matrices chosen from suffix width and span divisibility. Integer records use type 0x8B54 and first minus 0x70; boolean records use 0x8B56 and first minus 0x78. Attributes use first <16 and infer vectors or matrices from register span. A recognized `.xyzw` suffix is removed from copied float/attribute names; integer and boolean names remain intact. The parser does not reset its component count at a second dot, so `multi.xy.z` counts three components. This quirk is reconstructed and tested.

## Remaining matching work

Four evidence-based forms leave a 116-byte extent difference. The final form recovers the retail 0x24-byte local stack allocation and bulk resource-zero store, but differs in header/failure spill placement, operand traversal scheduling, output/variable loop register allocation and bucket lookup addressing. These are unresolved code-generation differences, not permission to pad, add assembly, change compiler flags or alter the target interval. Main should integrate the coherent shared types, independently rebuild, and retain the NonMatching guard until canonical equality actually succeeds.
