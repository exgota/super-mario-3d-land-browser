# Skeletal animation construction 001C440C

This proposal was tested against frozen main `86104d96a7f570383bbfefc5fffad998e134496c`, branch `dot/skeletal-animation-construction`. It supplies one isolated ordinary-C++ translation unit and a dedicated observed-layout header. The whole existing U interval is `[001C440C,001C4C80)`, **2,164 bytes**. It remains **NonMatching with zero exact credit**, behind the project's `NON_MATCHING` guard. No map, rank, ledger, STATE, tool, compiler configuration or binary change is proposed.

**Cross-proposal integration hold:** current main has no other declaration of these new neutral C imports, but pending historical sources have incompatible C++ declarations for shared addresses. `LightDataDirector1C6410.cpp` declares `dat_003E23B8` as `unsigned int[]` and `fn_00293088(unsigned int)` returning `sead::Heap*`; this proposal declares a `void*` object and `fn_00293088(void*)` returning `void*`. `retail_EffectResourceInit.cpp` also declares that object as `void*`. `StreetPassObjInit14414C.cpp` declares `fn_0026AC60(Array*,int,void*,int)`, while this proposal uses its separate `PointerArray*` view. The concrete ARM32 call/return registers and three-word array layouts agree, but the C++ declarations are not identical. This report explicitly excludes combining those proposals. None of their files was changed. A shared identity/declaration decision belongs to their coordinator.

The coordinator reported a repository-visibility conflict while this work was underway, and the proposal remained unpublished at that checkpoint. The owner later lifted that publication hold as recorded below; the historical local measurements are unchanged.

## Checker and source forms

Form 1, committed as `db4d31f7b74762ada29c340a36f00d61a7e0a74b` at 05:36:47 UTC, compiled through the normal project build. The root section was 1,596 bytes with a separate 168-byte allocation helper. The unchanged checker reported: `Source closure rejected: Residual helper code or another allocated extent remains after inlining.` This is no match and no accepted partial credit.

Form 2, committed as `f2ac9bfa99129db984eee08463664053bddc49c7` at 05:38:14 UTC, explicitly inlines that allocator helper, as the observed original does. It also preserves the original publication of the single-evaluator pointer before translation-mask setup and virtual model binding. The compiler emits one 1,976-byte root section; every source helper is inlined. The checker reports:

```
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The 1,976-byte section is 188 bytes smaller than the full 2,164-byte row. Closure success is not exactness. There were two meaningful compiled source forms; no cosmetic register, padding, volatile, flag, opcode, object or metadata tuning followed. Commit `9d3f940fee5acaeddc18eb6885e582a268426931` at 05:45:59 UTC adds only the project NonMatching guard. Its canonical object is byte-identical to form 2. The final guarded source was rebuilt and checked again.

The checker wrapper temporarily names only the existing row and existing imported rows. It restores every original map byte in a finally block. The unchanged map SHA256 is `94ac5673e907db521bf6b4b606d02be64ee7bae8a8fd2f08328b0c30d85b3db9`. No bounds, pool positions, types or ranks are committed.

## Identity, placement and ABI evidence

The source sits in the configured `lib/al` module and uses ARMCC 4.1 build 791 with its unchanged normal flags. This is a placement assumption supported by its animation-player wrapper role, neighboring engine utilities, use of the standard project allocator and caller-facing model/animation storage. It does not prove the original compiler or original class name. No alternative compiler/module/flags were used to improve the bytes; build 894 was unavailable and was not installed.

Caller 001C43B8 reads the archive resource's signed count at 64. If that count is positive, it requests exactly 28 hex bytes with the original nothrow allocator at 001C43E4/E8, then calls this root at 001C4404. It passes self in r0, archive in r1, model in r2, allocator in r3, and evaluator count on the stack. This independently supports the 40-byte Player allocation and five-argument ABI. The root returns its receiver in r0. The caller itself runs unmodified in three replay cases.

The root calls base initializer 0024E47C, which clears the animation-table pointer at 4 and only the two bytes at 8 and 9. The proposal preserves the untouched bytes at A and B. The observed root writes a vtable address at 0, model at C, single evaluator at 10, blend at 14, a count/capacity/entries array at 18/1C/20, and workspace pointer at 24. These are descriptive observed fields, not recovered original class names. The existing empty `al::AnimPlayerSkl` declaration and the small `alModelCtr` header do not establish that either is the class viewed here; neither was extended or replaced.

Model+8 supplies a resource whose type mask must contain 40000092. Its E0 field is a self-relative pointer to the skeleton record. That record supplies the workspace count at 18 and translation flag at 28. Model+220 is a heap-owner argument; +228 is the model context; +22C selects a binding-table row. Only the consumed prefix is asserted. No whole original model allocation size is claimed.

Allocator virtual slots 8/C supply allocate/release. The root requests a 44-hex-byte blend and initializes three four-word vectors at 10,20,30, then bytes 40=0,41=0,42=1; byte43 remains untouched. Allocator at 4 and model at 8 agree with the independent 0033BA3C transform-blend proposal. That proposal's children14/18, weights24/28, cached34 and control40..42 match this constructor's observed writes exactly. Its header stays separate and unchanged.

The actual blend table 003D8500 points at setter 00298878 in slot C, which writes its argument to object+8. The actual evaluator table 003D8414 points at 002A69E8 in that slot, and the original setter/binder runs in replay. The evaluator creation entry 002A63D8 consumes resource pointer, signed model count, maximum track count and a byte cache flag in a 16-byte argument record, and requests a 70-hex-byte object. Our Evaluator view is only the prefix through translation-disable byte5D; its full size is not asserted. The model context resource's 10 field supplies the model count.

Shared import 0026AC60 accepts a three-word signed count/capacity/buffer object, count in r1, heap in r2 and alignment in r3. Its instructions at 0026AC90..98 independently establish offsets 0/4/8. The pending StreetPass view has those same fields and size. Entry 00293088 ignores its incoming r0, loads the current thread state through the global heap-manager path, and returns one pointer in r0. This explains ABI compatibility with the pending LightDataDirector declaration without resolving its C++ type identity.

Only existing whole data rows are imported, and declarations are unsized where appropriate. No resource/table bytes are copied. The `SkeletalAnimation` string is a semantic source literal reconstructed from this function. The three vtable writes during construction do not assert those tables' complete C++ class identities.

## Full interval and behavior

Executable words occupy 001C440C..001C4900 and 001C4930..001C4C7C: 528 ARM instructions / 2,112 bytes. The 48-byte interior pool at 001C4900..001C4930 contains addresses, the local string, and float constants. The last four bytes at 001C4C7C are another data import. The old Pool field is internal, so no tail was omitted.

The constructor builds its workspace, records every animation's name/resource pair, and computes the maximum signed track count starting at zero. Counts at most one create a single evaluator. Larger counts create a blend, three allocator-backed vectors, an engine pointer array, and one evaluator per count. Weight and cached-weight vectors begin with one followed by zeros; children are appended separately. Failed vector allocation leaves the partially constructed object for the original subsequent operations. The proposal does not insert new null checks or recovery policy.

Each evaluator conditionally receives a translation-disable byte based on skeleton flag2, then binds the model. The multi-evaluator path finally writes the blend into the selected binding-table slot only if index>=0, index<observed row count and stride>0. The inlined append also reconstructs the allocator/ownership/capacity growth path, although normal construction never requires growth after successful preallocation.

## Whole-root differential replay

The final committed source passes **292 returning fixtures and 43 separate matching CPU-fault fixtures**, for 335 paired fixtures total. Three returning fixtures invoke the original direct caller. Three others perform the constructor twice in each machine without resetting heap, globals, or working memory between calls; they count as three fixtures, not six. No instruction-budget exhaustion occurs.

The initial returning set has 240 deterministic randomized fixtures: counts -3,0,1,2,3,4,8; zero through six archive records; signed track counts -2..6; workspace/model counts0..6; translation flags0,1,2,3,FFFFFFFF; valid and invalid binding indices/strides; default/FZ/DN and nondefault rounding modes. The extended set has 95 fixtures, comprising52 returns and43 CPU faults. Within it,65 one-allocation-rejection fixtures yield37 returns and28 CPU faults;3 caller fixtures return;18 null/type/resource/name controls yield3 returns and15 CPU faults;6 explicitly bounded alias controls return;3 repeated-construction fixtures return. Alias controls cover repeated archive-record pointers and the final binding output aliasing the receiver. They do not prove general pointer-alias equivalence.

Both sides run ARM1176/VFPv2 in Unicorn with the same fingerprinted full EU image. The original function and all its actual constructor, resource, string, evaluator-creation, model-binding, vector, matrix and guard callees execute their original instructions. The candidate is a diagnostic armlink of the unchanged canonical object at 00500000; only root entry dispatch changes. The diagnostic link is separate from the normal compact image and adds no acceptance credit.

The harness models exactly the thread-dependent heap lookup at 00293088 and two fixture-selected allocator callbacks plus release. The model is a deterministic aligned bump allocator with optional failure of one numbered allocation, seeded untouched storage and recorded allocation/release traces. Thus these are construction semantics under that explicit allocation model, not validation of the thread scheduler or real heap implementation. No evaluator creation or virtual model-binding routine is replaced by a model.

Comparison checks all512KiB of constructed memory, all owner-image globals from003E0000..00500000, complete returning r0, returned SP, r4..r11 and d8..d15 canaries, cumulative FPSCR exception bits9F, original-callee invocation counts, string content, allocations, and releases. For CPU faults it compares access kind, fault address/width and all compared memory/trace state; it does not equate instruction PCs or instruction counts. All43 fault pairs agree under that definition. No fault divergence is hidden in the returning count.

One address difference is expected: the actual resource constructor retains a self-relative reference to the caller's source string. The two links place the identical `SkeletalAnimation` literal at different addresses. The harness verifies the resolved text on both machines and normalizes only that one relocation word per resource in the comparison copy. It never patches target, candidate, or live emulated memory for this normalization.

Coverage reaches466/528 original root instructions and executes no interior/tail pool word. The62 uncovered words are three old-vector-release arms after fresh zero initialization, two loop-index-wrap alternatives, and the generic child-vector ownership/growth path unreachable in these ordinary preallocated fixtures. Full recovery of the row is not all-input equivalence. General pointer overlap, hostile allocator mutation, concurrency, allocation-size/signed arithmetic overflow, arbitrary callbacks, enabled FP traps, resource corruption beyond the listed controls, exact fault PCs/cycles, original allocator correctness, real animation assets, full game initialization, rendering, gameplay and native-port replay remain unverified.

## Preservation and reproducibility

The pristine861 baseline's unchanged gate passed767prior roots/all787actual definitions in596.501791275seconds. Its report SHA256 is `a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374`. This proposal reuses that evidence only through an **equivalence-backed preservation comparison, not a new787-definition checker gate**.

All180prior objects were examined, including objects with no checked definitions. All179ordinary canonical objects retain allocated bytes/metadata, complete symbols/relocations and compiler/input provenance. Every prior allocated extent, defined symbol and allocated relocation in the generated scaffold object also preserves. The unchanged scanner adds exactly14scaffold extents:9neutral function aliases and5data aliases for the new TU. These are automatic weak placeholders, not accepted reconstructed definitions. Their exact names are enumerated in `skeletal_animation_construction/preservation.json`. All373prior dependency inputs and625pre-existing tracked files were verified, with generated stubs.c explicitly treated as the expected changed generated input. No previous source/header/config/tool changed.

A normal build and final clean build compile, archive, link and export. The root remains U in the restored map, so compact-link success may use its generated stub and does not establish that the candidate body is in the normal game image. The separate diagnostic body link establishes only the replay path above.

Source SHA256: `99396960f5e0d4cacbca9e2268868567d42c487e914303a31771e65e6a15c35f`.
Header SHA256: `423cad401f65dde8a44681e737c5cc8d64614dadb35fa68055ccf65a1d4fe26c`.
Canonical object SHA256: `ac6875664a5816b707e0148ff2af8acd4d12e1a876adf8ee6abe42914187973e`.
Original SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

From the repository root, with the owner-local private inputs and approved installed dependencies/toolchain:

```sh
. ./development_environment.sh
export DEVKITARM=/usr
python project/dot_reports/skeletal_animation_construction/reproduce.py /absolute/path/to/pristine-mario-main861
```

The optional final argument enables the exhaustive old-object comparison against the verified pristine861 baseline. Without it, the clean build, original checker, diagnostic body link and all335replay fixtures still run. `manifest.json` freezes submitted source/header/tools/config/compiler inputs; the scripts validate hashes and leave generated outputs under ignored build/. No game bytes or objects are included.

Initial harness verification exposed the expected retained-string relocation, and the comparison was corrected to verify semantic pointer identity as described. Initial preservation probes rejected automatic added scaffold aliases and one expected-section spelling; the final comparison explicitly inventories those additions. These were harness assumptions, not source behavior fixes or concealed checker successes. The two meaningful source-form outcomes remain unchanged.

Work began at05:31:29UTC on2026-10-02. The final clean reproduction finished validation at05:53:44UTC, 1335.498seconds after the task started. Its clean build took36.618seconds, canonical probe0.860seconds, diagnostic link0.022seconds, returning replay7.007seconds, extended replay3.164seconds and preservation8.360seconds. These sequential measurements are not summed with overlapping earlier work. `skeletal_animation_construction/validation_summary.json` freezes the measured outputs; the outer local seal records final head and full-patch hash. Accepted bytes remain0; accepted throughput is0bytes/hour. The work did not claim a milestone or gameplay progress.

## Publication status — 2026-10-02 07:52 UTC owner decision

The owner confirmed that `exgota/super-mario-3d-land-browser` is public and authorized publication of source/header/report proposals on `dot/*`. The previous visibility hold is lifted under the updated `AGENTS.md`, `project/BRIEF.md` rule 13 and `project/DOT_BRIEF.md` at `56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`. Game files, extracted assets, credentials, private download URLs and leaked SDK material remain excluded. Earlier private/publication-hold wording describes the historical checkpoint.

This remains a proposal for the integrator with zero exact-match claims. All build, checker, replay and preservation evidence above concerns frozen base `86104d96a7f570383bbfefc5fffad998e134496c`; it is not current-main acceptance. The recorded failures, replay limits and unresolved type/identity constraints still apply. This publication correction changes notes only: source/header blobs and retained objects are unchanged, and no build or replay was repeated. Only the integrator may accept the proposal, set committed ranks, write the ledger or move `main`.
