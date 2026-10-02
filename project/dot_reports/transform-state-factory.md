# Transform-state factory 002A2C9C

This proposal was tested against frozen main `86104d96a7f570383bbfefc5fffad998e134496c`, on `dot/transform-state-factory`. It reconstructs the complete existing U interval `[002A2C9C,002A34D4)`, **2,104 bytes**, in one new ordinary-C++ translation unit and a private observed-layout header. It remains **NonMatching with zero exact credit**. No accepted source/header, map, rank, ledger, STATE, tool, compiler configuration or binary change is proposed. The `NON_MATCHING` guard was present in the only compiled source form and the final clean reproduction.

The canonical checker refuses the candidate's unresolved data identities before extent grading. Separate diagnostic execution agrees on **415 paired fixtures:382 returns and33 matching CPU faults**. Those results do not cure the canonical refusal or establish byte exactness. Publication was held at the original verification checkpoint after the coordinator reported the repository-visibility conflict. The owner later lifted that hold as recorded below.

## Exact checker evidence and source history

Source form1 was committed as `32af50ad3d5949e83bac26db5d75b077745a9973` at06:00:50UTC on2026-10-02 and built through the normal project `make.py eu` step. It emits one code section `i.fn_002A2C9C`, 1,648 complete bytes, plus ordinary eight-byte ARM unwind metadata. The function symbol's size is1,644; its final four-byte literal is part of the1,648-byte section. All C++ implementation helpers inline; there is no second emitted source-helper code section.

The unchanged project `tools/check.py fn_002A2C9C --object build/eu/obj/lib/al/src/Model/alTransformStateFactory.o` reports:

```
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

The final clean reproduction produces the same object hash and the same refusal. The three undefined data address spellings430C68,430CF0 and430D20 lack canonical map rows. Four direct function imports and four other data imports have existing rows. The checker reaches neither a successful canonical source/data closure nor its extent comparison. The1,648-byte emitted section is diagnostically456bytes smaller than the full2,104-byte original interval; that comparison is an ELF inspection, not a checker grade. No mismatch-byte count, matching claim, exact credit or accepted partial credit follows.

The checker wrapper changes only symbol spellings on nine existing rows, including the root and already-named291470. It does not add a row or edit boundaries, pools, types or data ownership. It restores every original map byte in a finally block. Restored SHA256 is `94ac5673e907db521bf6b4b606d02be64ee7bae8a8fd2f08328b0c30d85b3db9`.

There was one meaningful compiled source form and no cosmetic register, padding, volatile, compiler-flag, object or oracle tuning. Missing canonical data identity prevents acceptance, and no grounded second source hypothesis warranted spending another form. The complete source remains in its original commit; later commits add only notes and reproducibility artifacts.

Physical operational history is retained: the first shell write found that this base lacked the new Model directory, so no source compilation occurred until it was created; two early diagnostic link commands failed because the symbol file lacked the required SYMDEFS marker; the corrected link succeeded, after which an optional disassembly command found no arm-none-eabi-objdump in PATH. Capstone then decoded the emitted ELF directly. These are recorded setup failures, not new source forms or checker successes. No tooling was installed, and no object was edited.

## Whole interval, ownership and layout

Original executable words occupy002A2C9C..002A3170 and002A3198..002A34D0:515ARM instructions/2,060bytes. The internal pool002A3170..002A3198 contributes40bytes, and002A34D0..002A34D4 contributes4bytes. The map Pool is internal; all executable tail code through002A34CC is accounted for.

Independent caller002A01AC supplies the resource inr0, a word from its argument record+4 inr1, a sign-extended byte inr2, the address of a four-word owned-array descriptor inr3, and the allocator in the first stack slot. The factory returns its newly allocated object pointer. The original public class name and source-language integer/enum types remain unknown; these are descriptive ARM32 views.

Allocator virtual slots8/C allocate(size,alignment) and release(pointer). The root first requests a60-hex-byte receiver aligned4, then reads resource+18 separately for three arrays with element sizes40/30/30hex and alignment32. Zero counts suppress array allocation. Each temporary descriptor contains allocator,begin,end,capacity/flags. New capacities keep only the low30bits. The signed initialization loop re-reads resource+18 and fills a calculated transform followed by two identity matrices per iteration. A null allocation with a positive count is not given a new recovery policy: the first null destination is skipped, subsequent pointer progression can fault, exactly as observed in the bounded replay.

If receiver allocation failed, it still builds the temporary arrays, then releases remaining arrays in reverse order and returns null. On success it first transfers the caller's descriptor into a temporary, clears the donor's begin/end and low30capacity bits, and retains its top two flag bits. It transfers the three local arrays through value parameters, initializes the receiver's base, and then transfers all four descriptors into receiver fields1C/2C/3C/4C. The receiver's source pointer at5C retains the original resource. Generic element-destruction loops and allocator cleanup remain ordinary C++; no original loop is replaced with an instruction or byte stand-in.

The receiver has vtable0, allocator4, resource8, zero wordC, two base-storage pointers10/14, and byte18. Bytes19..1B remain untouched. Construction first installs table3D86BC, calls original2B349C with the allocator and forwarded word/mode, then installs3D82D0. The base helper performs its own allocations and initialization in replay. Both tables remain unsized borrowed imports of their existing whole60-byte map rows; no table bytes are defined.

The source sits in lib/al under its configured ARMCC4.1build791 and normal flags. This placement is an assumption consistent with the model/animation factory neighborhood, not proof of the original compiler or class identity. Approved902 remains available for the baseline SDK object. No alternative module/flags/build894 was used or installed.

## Separate data identity and declaration evidence

The independent original-binary audit is preserved in `transform_state_factory/global_identity_audit.md`. It makes no canonical metadata proposal or source modification. Identity/accessor254890 and constructor1E4788 independently establish a48-byte initialized/consumed identity-matrix span430C68..430C98, guarded by mapped word3F389C.

Independent2164F8 and261594 initialize the same64-byte transform span430CF0..430D30 under mapped word3F38A8: a48-byte identity matrix, three scale floats1 and flags7E1. **430D20 is the interior scale view430CF0+30, not evidence of a separate global allocation.** The source's unsized float import names that observed address view; the diagnostic symbol file aliases it to the interior address. It is not a proposed separate canonical row or an assertion of independently owned storage. Adjacent initialized storage has different guards, which strengthens separation of consumed spans but does not recover original linker symbols, enclosing allocation boundaries or padding. All original allocation/linker extents remain unresolved.

Main861 has no competing active C declaration of these imported neutral addresses. The pending root33BA3C proposal does: fn291470 uses `transform_blend::Matrix34*` and its const counterpart, fn28A998 uses unsigned*, dat3F389C is unsigned, and dat430C68 is that same Matrix34. This proposal explicitly imports those same nominal types through using declarations. The Matrix34 and Transform definitions are token-identical to that proposal, rather than merely equal-sized. Its Allocator and AllocatorVtable definitions are token-identical to the skeletal-construction proposal's `skeletal_construction` types. The recorded ABI audit verifies all four definition-token comparisons. Neither prior proposal was edited.

The historical inactive Effect packet/response declares fn291470 using references to a different nominal type; those declarations cannot be revived unchanged together with this family. ABI-compatible pointer registers alone do not establish C++ declaration identity. This work does not claim a combined integration or build of pending proposals. New private Root/Resource/Array views and inferred helper declarations are limited to this translation unit family; no public layout is replaced.

## Diagnostic replay and explicit initialization

The final replay runs the unmodified owner EU image under Unicorn ARM1176/VFPv2. Its SHA256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. The candidate is a separate diagnostic armlink of the unchanged canonical object at00500000. Only root-entry dispatch changes. Every actual direct original callee254890,28A998,291470 and2B349C executes original instructions on both sides. The linker bindings at missing original data addresses belong only to that diagnostic executable; they neither change canonical metadata nor bypass its refusal.

The only modeled operations are fixture-selected allocator callbacks and release. Allocation is a deterministic aligned bump allocator over seeded memory, optionally rejecting one numbered allocation or a wrapped request above30000hex bytes. The model records full size/alignment/result and release traces. The33extended fixtures also include bounded receiver/descriptor/storage overlap and callbacks changing the subsequently observed resource count. These checks do not validate the game's real allocator, threading or arbitrary hostile callbacks.

Initialization is explicit. Each fixture writes nondefault patterns into the observed matrix and transform spans and assigns the guards independently from0,1,2,3,FFFFFFFF. The word patterns include signed zero,1,-1,a signaling NaN,a quiet NaN,10,and negative infinity. Their exact values and initialization order are in replay.py. This includes deliberately inconsistent/pre-set-guard controls as well as zero-guard initialization cases; these are test states, not a claim about real startup memory.

With zero guards the original lazy initialization runs: the factory calls254890, which initializes430C68 under3F389C; then the factory initializes430CF0 under3F38A8 with scale ones and flags7E1. The factory's inline matrix lazy initialization also executes when appropriate, such as a pre-set transform guard with zero matrix guard. With nonzero guards the actual28A998 behavior determines whether seeded values remain; a guard with low bit clear but nonzero whole value does not initialize. No startup routine or original allocator boundary is invented or simulated. Independent2164F8/261594 are static evidence, not extra replay initialization hooks.

The first set has382paired fixtures:240deterministic randomized returning cases;128numbered allocation-rejection cases(104returns/24faults);9null allocator/resource/descriptor cases(9faults);2negative-count cases(returns under the explicit huge-request rejection rule); and3repeated-construction fixtures(returns). A repeated fixture performs two calls without resetting heap, globals or working memory, and is counted once. Random choices include counts0/1/2/3/5/8, forwarded words0/1/2/5, signed modes-128/-1/0/1/2/127, all four top-bit flag combinations, empty/nonempty donor spans, three fill bytes and default/FZ/DN/nondefault FP rounding states. Totals:349returns,33matching faults,zero divergence or budget exhaustion.

The additional33paired fixtures all return and agree:9receiver overlaps with the descriptor or its storage;6separate descriptor placements;6null donor-storage cases with zero/nonzero allocator; and12allocator callbacks changing resource count to0/1/3 at four allocation positions. Final combined totals are415fixtures,382returns,33faults. These bounded aliases do not prove general pointer-overlap equivalence.

Comparison uses all512KiB of fixture memory and all1,179,648bytes of original-image globals003E0000..00500000, full allocation/release traces, actual direct-callee call counts and cumulative FPSCR exception bits9F. Returning cases also compare r0 and validate returned SP,r4..r11,d8..d15 canaries. Fault cases compare fault access kind,address,width and all compared memory/trace state, without equating fault PCs or instruction counts. **No normalization or output masking is used for memory equality.** All33fault pairs agree under this definition; none is hidden in the returning count.

Coverage reaches446/515original root instructions, with no pool word executed. The69uncovered words consist of27instructions in the second identity-initialization arm (the preceding path already tested/set the same guard under this nonconcurrent model), and42instructions in value-temporary cleanup loops/releases after ownership was moved out. Their source semantics are present. Full interval recovery is not all-input equivalence. Arbitrary callbacks/concurrency, general pointer overlap, stack aliasing, huge allocation/count overflow beyond the two rejection controls, enabled FP traps, exact faults/cycles, native C++ portability of these pointer operations, original allocations and real animation assets remain unverified. No gameplay, rendering, browser or milestone claim follows.

## Preservation, hashes and reproduction

The coordinator's pristine861 gate passed767prior roots/all787actual definitions. Its report `mario-main861/build/dot-baseline-861/report.json` has SHA256 `a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374`. This proposal supplies **equivalence-backed preservation against that baseline, not a fresh787-definition checker pass**.

All180oldobjects were inspected. All179old canonical C++ objects preserve allocated bytes/section metadata, complete symbols/relocations and compiler/input provenance. Prior allocated scaffold extents,symbols and relocations also preserve. Exactly eight new generated scaffold aliases appear: i.fn_002A2C9C,i.fn_00254890,i.fn_0028A998,i.fn_002B349C,.constdata_dat_003D82D0,.constdata_dat_003D86BC,.sdata_dat_003F389C,.sdata_dat_003F38A8. The pre-existing291470 alias stays unchanged. No scaffold identity is generated for the three missing addresses. These automatic placeholders add no reconstructed definition credit.

All373prior dependency inputs and625pre-existing tracked files verify unchanged, with generated stubs.c explicitly treated as the expected changed scaffold input. The one new canonical C++ object is alTransformStateFactory.o. No baseline file, map or build was mutated. Preservation details and hashes are in the committed evidence snapshots.

A final clean normal `make.py eu -ca` compiles,archives,links and exports. The root remains U in the restored map, so compact-link success does not establish inclusion of the reconstructed body. The separate diagnostic body link establishes only the replay path above.

- Source SHA256: `9999decab3a89fe2f6c931b33dff19056889b3f709f22206212af2b990d947a4`
- Header SHA256: `068413445386a649e5b0091a039216b63a9d7f5a3e56aacf35bebf6b53703585`
- Canonical object SHA256: `d217fdffbffdc85074710deecef41516ef53e24a5084c11d381746a9f26e6c43`
- Original SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

Run from the repository root with the original private local inputs and approved installed dependencies:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python project/dot_reports/transform_state_factory/reproduce.py /absolute/path/to/pristine-mario-main861
```

The optional baseline path enables the exhaustive old-object/provenance comparison. Without it, source/input hashes, clean normal build, unchanged canonical refusal, separate diagnostic link and all415fixtures still reproduce. audit.py optionally accepts the root33BA3C and skeletal-construction checkout paths to repeat their nominal-type token comparisons. Scripts write generated outputs only under ignored build/; no game data or objects are included in this proposal.

Work began05:55:16UTC on2026-10-02. Final clean reproduction finished06:10:49.247159UTC,933.247seconds after assignment. Its measured stages were40.202seconds clean build,1.297canonical probe,0.014diagnostic link,7.171first replay,0.854extended replay,0.120object audit and9.026preservation. These sequential stage measurements are not summed with overlapping earlier work. Evidence/report sealing follows that verified source checkpoint and does not change the source or object. Accepted bytes remain0 and accepted throughput0bytes/hour.

## Publication status — 2026-10-02 07:52 UTC owner decision

The owner confirmed that `exgota/super-mario-3d-land-browser` is public and authorized publication of source/header/report proposals on `dot/*`. The previous visibility hold is lifted under the updated `AGENTS.md`, `project/BRIEF.md` rule 13 and `project/DOT_BRIEF.md` at `56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`. Game files, extracted assets, credentials, private download URLs and leaked SDK material remain excluded. Earlier private/publication-hold wording describes the historical checkpoint.

This remains a proposal for the integrator with zero exact-match claims. All build, checker, replay and preservation evidence above concerns frozen base `86104d96a7f570383bbfefc5fffad998e134496c`; it is not current-main acceptance. The recorded failures, replay limits and unresolved type/identity constraints still apply. This publication correction changes notes only: source/header blobs and retained objects are unchanged, and no build or replay was repeated. Only the integrator may accept the proposal, set committed ranks, write the ledger or move `main`.
