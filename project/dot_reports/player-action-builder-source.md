# Complete source proposal for the player action graph builder

This continues `largest-root-layout.md`. The verified two-constructor checkpoint `f15d147` remains unchanged on its own branch. `dot/player-action-builder-source` adds a complete ordinary C++ definition for the original `0x001A9574..0x001B4658` interval, under the descriptive name `PlayerActionGraphBuildOutputs::build(Player*)` and the project's `NON_MATCHING` convention.

The configured ARMCC 4.1/791 build emits one function section of 28,856 bytes, versus 45,284 original bytes. Every inline source helper disappears into that single function; no new helper address is invented. There is no assembly, instruction interpreter, emulated register array, or function-byte stand-in. This is a nonmatching proposal and adds zero exact bytes for the builder.

## Source structure and remaining names

The body uses normal `new` expressions for 370 scalar allocations, 60 action nodes, all/any/not conditions, and the final graph. Thirteen provider constructions remain nested inside their parent `new` expressions, preserving the original allocation order and the parent-allocation null guard. Two normal transition-group objects own their pointer arrays; six entries populate those groups, and 94 calls to their inlined loop apply them to nodes. The remaining 431 edge additions are explicit `PlayerActionNode::append` calls. The two deliberately shared action instances, nine output handles, and three post-constructor component writes are retained.

The headers declare observed external constructors using neutral names containing their original addresses. Their declared base types, allocation extents, argument positions, nullable interface offsets, and root-visible fields follow the binary evidence. Unrecovered fields remain opaque storage. These names are identity proposals, not recovered source spellings. The 13 small provider classes preserve their observed virtual-slot counts; their slot return signatures are still unrecovered, and this builder never calls those slots. No placeholder method body is supplied.

`proposed_imports.csv` records every symbol/address pair used for the diagnostic link. Each function import is an existing observed constructor, append routine, allocator, or getter. Each data import comes from an observed vtable address point, with its eight-byte ABI prefix explicitly distinguished. No original function boundary or main-owned metadata is changed in the committed proposal.

## Configured build and canonical gate

Source form 1, committed as `0780af0`, compiles through the project's unchanged `make.py eu`; the normal compact scaffold links and exports while this root is still excluded as U. That scaffold build does not mean the builder's metadata has been accepted.

An unchanged `tools/check.py --object` invocation first rejects unresolved constructor identities. With those observed function identities proposed only in the local map, it reaches the remaining data gate and reports:

```
Source closure rejected: An unresolved source helper is referenced by a non-branch relocation.
```

Those unresolved relocations are the emitted provider/condition virtual tables. No checker relaxation, altered object, false rank, or shifted interval is used. The 22-address-point ownership worklist from the earlier report still applies, as does the separately evidenced incorrect stream-constructor name at `0x002D3F74`.

## Bounded semantic validation

A diagnostic ARM link places the unchanged project-built object at the original root address and imports its observed external function/data identities. This link is separate from the checker and cannot grant exact credit. The configured object contains no additional allocated implementation sections besides its single function (and ordinary exception-index metadata).

The source-generated root and original root were each executed under ARM1176 Unicorn with identical observable external-call stubs. Constructors return their receiver; allocator stubs supply aligned storage or the selected failed request; configuration getters return controlled values; append calls are observed. The comparison checks complete ordered external call targets and their relevant register/stack/float arguments, every requested allocation extent, final allocated bytes, all output handles, and return/fault outcomes. Temporary-reference values and string contents are compared without requiring identical stack/string addresses.

All 435 paired cases agree:

- Four successful cases with different callback values and ABI caller-saved clobber patterns
- 57 cases setting each Player field from `+0` through `+0xE0` to null in turn
- 374 cases failing each root-local scalar or array allocation request in turn
- 425 paired returns and ten paired root-memory faults

Heap storage begins with nonzero `0xA5` bytes, so an unwritten capacity slot is not accidentally treated as an initialized zero. The successful trace agrees on all 1,656 external calls and their arguments, 374 allocation requests, 721 ordered edge calls, 207 composite-child append calls, final object bytes, nine handles, and returned graph.

This is caller-side compositional evidence, not gameplay, full-callee equivalence, or proof that allocation failures are safely handled by the real external callees. Some callees would fault or have effects not represented by these observation stubs. The matrix deliberately preserves that limit and compares the complete builder behavior within it.

## Affected-source preservation

The two main-accepted roots whose recorded build inputs include changed headers are `PlayerActor::getProperty` and `PlayerActionGraph::move`. Both repeat `O -> O` with the unchanged canonical checker after the source/header additions. The two earlier constructor proposals at `0x00181558` and `0x001B4658` also remain byte-exact. The `PlayerActionNode::getAction` helper remains unchanged and is covered by move's canonical closure check. This is the affected set, not a full 569-root audit.

## Noncanonical optimization investigation

Two temporary, uncommitted `#pragma O0` probes tested the observed large-frame/spill pattern. They are diagnostics only; no exact checker claim was attempted under either override, and neither override remains in the submitted source.

| Probe | Root bytes | Root stack frame | Limitation |
|---|---:|---:|---|
| Configured `-O3 -Otime` candidate | 28,856 | Optimized frame | Published source form; 435 paired semantic cases |
| `O0` before headers | 44,880 | `0x8C0` | Adds an unobserved abstract-condition vtable; no original-address replay |
| `O0` after headers | 45,472 | `0x8C0` | Isolated linker rejects generated VFE metadata (`L6647E`); no replay |
| Original | 45,284 | `0x8B0` | Unchanged oracle interval |

The after-header probe is 188 bytes longer than the original and removes the extra abstract-condition vtable. This supports investigating different optimization treatment for the root and its inline constructors. It does not establish original flag provenance or authorize canonical intake of an override. The configured source and build are restored; per-file optimization provenance remains a separate review item.

The current deliverable therefore separates three facts: complete reviewable C++ construction source exists; its configured build has strong bounded caller-side semantic agreement; canonical matching remains blocked by metadata and a substantial code-generation difference under the configured flags.

## Restored configured checkpoint and reproduction

The final configured clean build links and exports. Its complete canonical builder object SHA256 is `5faee4994931fc9f54f130109f29f23da6acbc29e42512a6aff624f70d896436`, byte-identical to the object used for all 435 paired cases. Adding the `NON_MATCHING` guard and declaration comments does not change the object. All four affected/dependency canonical checks repeat successfully after restoration. There is no optimization override in the committed source or configuration.

`player-action-builder-source/replay_recipe.md` contains a self-contained, repository-relative analysis recipe using the committed ABI/import CSVs and the project-built object. Its four-case smoke mode was also verified after packaging. The full mode reproduces the 435-case matrix with the same stated limitations. It writes only ignored diagnostic build outputs and changes no project tool or oracle.

Three distinct full-root code-generation forms were inspected: the configured candidate and the two explicitly noncanonical optimization probes. Restoring the configured build reproduces form 1 exactly. Five of the eight real compile-diff forms remain available for a later, justified source/metadata/flag-provenance revision.
