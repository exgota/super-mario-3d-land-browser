# Item-spawn dispatcher, 002CDEA4

Apply-clean NonMatching proposal against frozen main `86104d96a7f570383bbfefc5fffad998e134496c`, branch `dot/item-spawn-dispatcher`. The historical repository-visibility hold was lifted by the owner as recorded below. This family adds no exact roots or bytes.

The whole original root is `002CDEA4..002CE700`, 2140 bytes. Its 30-entry jump table occupies `002CDEC4..002CDF3C` (120 bytes), and its six-float literal island occupies `002CE2B8..002CE2D0` (24 bytes). The executable tail ends at `002CE6FC`. The blank map Pool field does not remove either island from the target interval.

## Result and attempts

Two meaningful source forms were compiled by the unchanged project with ARMCC4.1/791. Committed form1 (`4dc280a64bd49997f641c8e546fc17a8b237eb93`) grouped duplicate count/flag cases and emitted1624bytes. Form2 (`1656507d1c9583dff8abcad96b3fb404358d7bf5`) preserves separate case blocks and emitted1936bytes. Both canonical `tools/check.py --object` calls returned1 with `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` There is no byte-difference count: the canonical checker rejects the full extent before equal-length comparison.

The final source commit `a76a8e762ebbb751dac23e484ebf1e82a25bf69f` adds the established `NON_MATCHING` guard. Its1936-byte allocated root is byte-identical to form2, SHA256 `54ee471879f8d5440ebab32d94e53492dd08d08c5fef97c0d6d847152c17250a`. This guard repair is not a third matching form. The compiler also emits an8-byte unwind section, not an additional function. The sole nonzero function definition is the root; local zero-size `__switch$$` belongs to its jump table. All private adapters inline. The emitted root itself includes a120-byte table at offset32 and24-byte literal island at offset988.

The final normal clean build, `python make.py eu -ca`, linked successfully in40.187seconds, starting2026-10-02T06:04:19.478909Z. It built47Game,132al and1SDK C++ objects plus generated stubs. The final canonical check took0.731seconds and produced the same full-size rejection. The temporary spelling `fn_002CDEA4` was assigned only to the existing root row, then the complete map was restored exactly: SHA256 `94ac5673e907db521bf6b4b606d02be64ee7bae8a8fd2f08328b0c30d85b3db9` before and after. No boundary, type, rank, compiler configuration, tool or accepted source/header is part of the proposal.

No third grounded source hypothesis was identified. Original NOPs have no invented C++ meaning; no padding, assembly, synthetic game helper, compiler-flag change or map rescue was attempted. Form1 is retained both in history and `project/dot_notes/item-spawn-dispatcher/form1.cpp.txt`. `attempts.md` preserves the two forms and the guard repair.

## ABI, layout and closure

Independent callsites1195A4 and146D20 supply the integer selector inr0, source actor inr1, position pointer inr2 and a16-byte orientation pointer inr3; neither consumes a result. The root is therefore a void dispatcher with an unsigned selector and borrowed actor/vector/quaternion inputs. Helper2783BC constructs a Y-axis quaternion, multiplies through26A958 and normalizes through249414, independently supporting the orientation interpretation.

The accepted `ItemHolder`, `al::LiveActor`, `al::getSceneObj`, gravity/translation accessors and `setTrans` declarations remain unchanged. Holder bytes50/51/52 are read through an unsigned-character view of the accepted object, avoiding any private-field/header change. Root branches1/2/4 check byte50 and select through2276D8. Branches3/10 negate gravity and launch1/5 records at37.5 and0.9. Branches5..9 launch1/3/5/5/10 records with0.7 except branch8’s1.0, preserving the order of gravity then actor-position access.

Branches11..27 use original pool selectors and null-preserving secondary-interface offsets60/64. The private typed dispatch view invokes slot0, or slot8 for branch23. Branch11 optionally copies the quaternion and performs in-place rotations of−30 then+60 before two appearances. Branches15/17/21 consult27F2C0;17/18 also observe byte51. Branch29 selects through225E70, updates pose orientation, sets translation and calls the accepted LiveActor virtual appearance slot0C. Cases0/28 and every unsigned selector above29 return without a dispatch. The original null selection still reaches a null interface read; the reconstruction does not invent a successful null fallback.

All26 undefined function imports resolve uniquely to existing function rows. Four accepted accessors are used through their actual headers; the22other dependencies remain original external functions. There are no external data imports or new identities, and no out-of-line C++ helper closure. `closure.json` records every import, original row extent, compiled mapping symbol, defined function and build input. The behavior-only replay link binds the unmodified whole compiler object to those established original addresses. It is separate from canonical matching evidence.

A114-unique-head worktree audit found existing declarations for `fn_0027C05C` and pending StreetPass `fn_00271028`. The former retains `void(al::LiveActor*)`. The latter retains exactly `void(dot14414c::Actor*, const dot14414c::Quat*)` via opaque forward declarations and explicit boundary casts; neither private type is redefined. No competing declarations were observed for the other imported C names. The114-head audit and source excerpts are recorded in `declaration-audit.json`; it does not claim access to unseen remote/Mac changes.

## Bounded original-code replay

The final committed root and the unmodified owner binary agree on960 deterministic fixtures. Of these,858 return completely,61 produce matching CPU unmapped-read faults, and41 reject null position/orientation reads inside explicit endpoint models. These outcomes are distinct; the102fault/rejection cases are not successful executions. There are zero trace, modeled-effect, complete mutable-fixture-memory or direct-import-count divergences. All26direct imports are covered.

The fixture set includes all30table selectors, four out-of-range unsigned controls, five flag combinations and three player-state values; per-selector controls vary item availability, signed-byte availability values, pending-record presence/count, cycle index, position aliasing, and null holder/pose/position/orientation. An additional120seeded finite vector/quaternion fixtures cover gravity/count and double-appearance paths. The full fixture generator and per-fixture outcome/hash records are committed. Raw detailed results regenerate in ignored build storage; their final SHA is recorded in `replay-results.json`.

Every direct imported function executes original ARM instructions, including all15root pool selectors, their real pending-count/cycle mutation and original integer division, the quaternion/trig/math paths, scene-object/pose utilities and their ordinary descendants. All actual original code is loaded unchanged from the hash-verified owner executable. The selected item and source pose objects are synthetic, documented fixtures.

Explicit models replace the game-system provider28E688, random-vector provider272A0C, scene-area predicate27F11C, coin effect routing271110, nerve transition280610, and terminal launch operations31A9CC/227F8C. Synthetic virtual endpoints provide gravity, pose-quaternion storage, base-matrix availability/calculation, actor appearance/make-appeared, and item appearance/alternate appearance. The terminal appearance implementations themselves are not validated. Their logs compare interface pointer, position and quaternion values; the real selectors and utilities execute before reaching them. `replay-results.json` enumerates all model addresses and call counts. Hook failures reading null endpoint arguments are separately recorded as model rejections.

Replay setup history is retained in `attempts.md`: a first30-case smoke run lacked the actor base-matrix virtual entry, and the initial Unicorn stop address zero could hide that fetch fault. The fixture gained the observed slot3C endpoint and a distinct stop address before final whole-root replay. No game source or original instruction was changed to cure either harness issue.

These tests do not establish actual scene state, arbitrary object graphs, nonfinite floating inputs, rendering, sound, allocation timing, full appearance implementations or gameplay. The models intentionally bound the environment. Canonical matching remains rejected.

## Prior-root preservation and hashes

The pristine861 baseline independently passed767roots and all787actual definitions. Its report SHA is `a692f9a29d92c3f8021b62a2254b06fb44776f34cfa2f51849364bde66dee374`. This proposal did not mutate that baseline or rerun its checker. Instead, after the final clean build, an exhaustive comparison established equivalence of all179prior C++ objects and372prior input identities, their compiler commands/hashes/provenance, every section, every symbol and every resolved relocation. Only repository path prefixes in STT_FILE/nonallocated comments and consequent raw string-table offsets are normalized. This is equivalence-backed preservation, not a fresh767-root checker pass.

Generated scaffold changes are also explicit:22new address aliases appear for the root and previously unnamed imports. All2095old allocated scaffold sections are unchanged; there are no scaffold unwind entries. The22added aliases are enumerated in `preservation.json`. They are normal generated scaffold and supply no reconstructed behavior or exact credit. The original source map and all previous allocated definitions are preserved.

Final C++ SHA256: `8efd9327183a233d2a764a649f3f0e361cca9508d6e95c2a86f16406eaf68829`.

Final project-built object SHA256: `d01348b54379e25102c00d750035b35a51b3aee6ef9e6d9e777b24acc32d7085`.

Canonical checker SHA256: `e177e0abe130e645e75c5f0dc24abb19dbe83310de55f1f099ec8fc385e07317`. ARMCC791 SHA256: `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`. Complete tool/compiler/source/object hashes are in `input-output-seal.json`. ARMCC894 is absent and was not downloaded or tested; SDK902 remains the normal configured compiler. The original EU SHA remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

Work began2026-10-02T05:49:23Z. The final clean build and reproduction completed around06:06UTC; reporting and sealing continued afterward. No elapsed-time sum over overlapping checks is used. Accepted throughput is zero because the root remains NonMatching.

## Reproduction

Use the named frozen base plus this complete family patch, the owner's private EU data and existing approved toolchain. From the repository root:

```
. ./development_environment.sh
export DEVKITARM=/usr
python make.py eu -ca
python project/dot_notes/item-spawn-dispatcher/probe.py reproduce
python project/dot_notes/item-spawn-dispatcher/link.py
python project/dot_notes/item-spawn-dispatcher/replay.py
python project/dot_notes/item-spawn-dispatcher/preserve.py
```

The probe must return canonical full-size M, not O; it restores map bytes in a finally block. The replay must report960agreements with858returns/61CPUfaults/41modelrejections. `preserve.py` expects the pristine `mario-main861` sibling and its stated independently verified baseline; adjust only that baseline location if reproducing elsewhere. The complete family consists solely of the new isolated C++ file and its report/reproduction notes. No binary, game data, map, ledger, STATE, tools or configuration is included.

## Publication status — 2026-10-02 07:52 UTC owner decision

The owner confirmed that `exgota/super-mario-3d-land-browser` is public and authorized publication of source/header/report proposals on `dot/*`. The previous visibility hold is lifted under the updated `AGENTS.md`, `project/BRIEF.md` rule 13 and `project/DOT_BRIEF.md` at `56e9b4b89fe3c5d391c2948f1ce2b2c50706638e`. Game files, extracted assets, credentials, private download URLs and leaked SDK material remain excluded. Earlier private/publication-hold wording describes the historical checkpoint.

This remains a proposal for the integrator with zero exact-match claims. All build, checker, replay and preservation evidence above concerns frozen base `86104d96a7f570383bbfefc5fffad998e134496c`; it is not current-main acceptance. The recorded failures, replay limits and unresolved type/identity constraints still apply. This publication correction changes notes only: source/header blobs and retained objects are unchanged, and no build or replay was repeated. Only the integrator may accept the proposal, set committed ranks, write the ledger or move `main`.
