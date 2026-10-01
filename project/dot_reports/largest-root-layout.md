# Player action graph construction at 0x001A9574

Checkpoint from `dot/largest-root-layout`, based on main `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`. The original interval is `0x001A9574..0x001B4658` (45,284 bytes). Its first pool marker is `0x001AB32C`; that marker is not the end of the code. The root remains unimplemented and unmatched. There have been zero root compile-diff forms and no checker claim.

The EU input SHA256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Analysis uses only that executable and the existing clean repository. No original boundaries, map identities, ranks, shared tools, or accepted-source definitions are changed in this checkpoint.

## Identity established independently of the prologue

The sole direct ARM branch to this root found in the executable is the call at `0x00309840`, inside `0x0030933C`. It constructs a stack object at `sp+0x1C` by calling `0x001B4658`, passes that object in R0 and its persistent R4 receiver in R1, and stores the returned pointer at receiver `+0x48`. The existing clean `Player` header identifies that field as `mActionGraph`. The root's last allocation is 12 bytes, calls `0x00181558`, sets its first word to one of the newly constructed action nodes, and returns it. The existing named `PlayerActionGraph::move` at `0x0018153C` independently reads graph `+0`, node `+4`, then calls action vtable slot `+0x10`.

This supports a graph-construction function taking `(output handles, Player*)` and returning `PlayerActionGraph*`. It does not establish its original class or source-level function name. The first parameter is a distinct 0x24-byte output-handle object, not the Player receiver. It has no vtable store in the observed constructor. Its `+8` field is deliberately untouched by that constructor and is assigned by this root.

The result begins at the third allocated action node, `node02`, wrapping the action constructor at `0x00174C18`. The 60 graph nodes wrap 58 distinct action instances. `node43` and `node45` share the action constructed at `0x0019EFB4`; `node44` and `node46` share the action constructed at `0x00174840`.

## Full-interval control flow

A worklist traversal starts at `0x001A9574`, follows both conditional successors and unconditional ARM branches, and treats calls as returning. It reaches 11,284 instructions / 45,136 bytes. All 148 remaining bytes belong to four skipped pool/string ranges:

| Range | Bytes | Contents |
|---|---:|---|
| `0x001AB32C..0x001AB36C` | 64 | 16 referenced virtual-table addresses |
| `0x001AC174..0x001AC188` | 20 | Four virtual-table addresses and float 1.0 |
| `0x001AE570..0x001AE574` | 4 | One virtual-table address |
| `0x001AF6C8..0x001AF704` | 60 | One virtual-table address and three strings |

The embedded strings are `TailAttackSquatGround`, `TailAttackSquatAir`, and `SquatEnd`. Their PC-relative address expressions must remain inside the whole root when checking it. There are 1,456 static direct call sites to 133 distinct targets, plus four indirect calls. No unresolved computed branch or additional return is present in the reachable control flow. The unique final return is at `0x001B4654`.

The root's 94 unsigned counted loops all apply one of two reusable transition groups to a node. Their comparisons use the group's current count, not its capacity. The 420 equality branches comprise allocation guards and nullable interface adjustments; they do not make player-input-driven choices among different graph topologies.

## Bounded construction trace

An ARM1176 Unicorn trace executes the complete original root with unique nonnull Player component sentinels and successful root-local allocations. External calls are observation points, not execution of their implementations: ordinary constructors return their receiver; allocation stubs return unique aligned storage; the four indirect configuration getters return controlled values. The getter at `0x00308FC8` applies its independently inspected nullable `+4` interface adjustment. This is a structural trace of the builder, not a game execution test, equivalence test, or proof of the callees' behavior.

The trace reaches the final return in 16,832 executed instructions including hook entries and observes:

- 370 root-local scalar allocation requests totaling 7,616 requested bytes
- Four array allocation requests totaling 56 requested bytes
- 60 node constructor calls at `0x0025213C`
- 79 all-condition composites and 186 child append calls
- Ten any-condition composites and 21 child append calls
- 24 negating-condition constructor calls at `0x00252080`
- 721 ordered edge append calls at `0x00251E28`

Those 721 edges come from 431 direct append sites, 51 four-entry loop applications, and 43 two-entry loop applications. The 525 static edge-call sites must not be mistaken for the dynamic edge count. These allocation totals exclude allocations inside the observed external callees, such as edge/list-node construction.

Machine-readable notes in `largest-root-layout/` preserve the allocation identities, all 60 node/action associations, all 721 ordered edges, and all 207 ordered composite-child associations. `allocationNNN` IDs follow root-local allocation-call order. `nodeNN` IDs follow node-constructor order. Addresses in those notes are evidence anchors, not proposed original names. Inline-object fields come only from writes performed by the root; fields that callees would initialize are not fabricated.

## Layout evidence

`PlayerActionNode` is 0x14 bytes: the constructor at `0x0025213C` installs the address point `0x003CC3F8` at `+0`, action pointer at `+4`, and a 12-byte intrusive-list header at `+8`. The header has self pointers at `+8/+0xC` and count zero at `+0x10`.

The edge append routine at `0x00251E28` takes `(node, condition, destination)`. It allocates 0x18 bytes and initializes links at `+0/+4`, a self pointer at `+8`, list owner at `+0xC`, condition at `+0x10`, and destination at `+0x14`. It inserts into the source node's list at `+8`. The order in `edges.csv` is therefore meaningful.

The composite constructor `0x00252054` installs address point `0x003D10DC`. Its check implementation at `0x001B8B7C` starts true and visits every child, accumulating AND. Constructor `0x00251FC0` installs `0x003D0858`; check `0x001A7290` starts false and visits every child, accumulating OR. Neither loop short-circuits. Both have a 16-byte offset-list at `+4` with stored node offset four at `+0x10`. The negating condition installs `0x003D0BB8`; `0x001A9528` calls child check and XORs its boolean result with one, while `0x001A9544` forwards setup.

The reusable transition-group object is 16 bytes: condition-pointer array at `+0`, destination-node array at `+4`, capacity at `+8`, and current count at `+0xC`. The first group allocates capacity five but appends only four entries; its fifth slot is never read by this root. The second allocates capacity two and appends two entries. Both arrays are separately allocated. No bounds check appears in the six inlined append operations.

| Output offset | Final value |
|---|---|
| `+0x00` | `node00` |
| `+0x04` | `node21` |
| `+0x08` | `node02`, also the graph's initial node |
| `+0x0C` | `node01` |
| `+0x10` | First action (`0x00171A78`) interface at `+8`, nullable |
| `+0x14` | Second action (`0x001A46C8`) interface at `+4`, nullable |
| `+0x18` | Action `0x001B79AC` interface at `+0x5C`, nullable |
| `+0x1C` | `node45` |
| `+0x20` | `node46` |

`PlayerActionGraph` is 12 bytes: current node at `+0`, another pointer initialized to null at `+4`, and a setup-needed byte initialized to one at `+8`. The operation at `0x001814B4` reads that byte, calls the current action's setup slot and every outgoing condition's setup slot, then clears the byte. That independently grounds the flag's role. The meaning of the second pointer is still open.

## Existing identity conflict requiring separate main review

The map row at `0x002D3F74..0x002D3F9C` is named `_ZN4sead20BufferWriteStreamSrcC1EPNS_9StreamSrcEPvj`. This constructor installs `0x003D1C6C`, stores R1/R2/R3 at `+4/+8/+0xC`, and clears `+0x10`. The installed table's check slot is `0x002D3D60`; its setup slot `0x002D3F68` clears that same counter. The check performs player vector/velocity calculations and accesses player interfaces. This root allocates that 20-byte object among its conditions and uses it in composites/graph edges. The existing stream-class name is inconsistent with both independent caller and vtable behavior.

Do not bind new C++ player-condition declarations to the stream name. A separate evidenced identity correction is needed; the original class name remains unresolved. The map row and its bounds are unchanged here.

## Next reconstruction stage

Recover the external constructor parameter contracts and address-point ownership for the inlined condition/action interfaces, using the node and allocation inventories as the worklist. The first 0x24-byte common action context stores nine Player fields in this order: `+0, +0x1C, +0x60, +0x24, +0x14, +0x0C, +0x10, +0x58, +0x78`. VFP load/store transport alone is not proof that a field is a float; the caller also transports ordinary pointers through VFP registers.

Then express construction as normal C++ allocations, composites, and ordered node transitions, retaining the two reusable groups and their 94 source loops. Preserve all 60 node identities and the two deliberate shared actions. Do not replace the root with a partial body, a table-driven instruction interpreter, or an assembly translation. The 0x8B0 stack size and extensive spills suggest restricted optimization, but no compiler flag change is justified until a real complete source form is built. All original interval bytes, including interleaved pools, remain in scope.

## Source checkpoint: two graph-construction dependencies

Commit `5a4374a` adds ordinary C++ for the two directly observed construction dependencies. Both first source forms build through the unchanged project `make.py` using configured ARMCC 4.1/791. The build links and exports. The unchanged checker reports the complete committed-source intervals byte-exact:

| Original interval | Local identity proposal | Complete bytes | First-form result |
|---|---|---:|---|
| `0x00181558..0x00181570` | `_ZN17PlayerActionGraphC1Ev` | 24 | `M -> O` |
| `0x001B4658..0x001B4680` | `_ZN29PlayerActionGraphBuildOutputsC1Ev` | 40 | `M -> O` |

Commands run after committing source:

```
. ./development_environment.sh
TMP=/tmp python make.py eu
python tools/check.py _ZN17PlayerActionGraphC1Ev --object build/eu/obj/Game/backup/src/Player/PlayerActionGraph.o
python tools/check.py _ZN29PlayerActionGraphBuildOutputsC1Ev --object build/eu/obj/Game/backup/src/Player/PlayerActionGraphBuildOutputs.o
```

Each checker prints `M -> O: The complete source-generated function interval matches byte for byte.` The two existing empty-symbol map rows were given those symbols and rank M only in the local worktree, with all bounds untouched; only the checker promoted them locally. No map edits are committed. `PlayerActionGraphBuildOutputs` is explicitly a descriptive reconstruction name, not a recovered original spelling. Its intentionally uninitialized `node08` exactly preserves the original constructor.

These 64 bytes are dependency proposals for independent main intake. They do not reduce the unmatched extent of the 45,284-byte root. The root still has zero compile-diff forms and no match claim.

A subsequent clean `TMP=/tmp python make.py eu -ca` also links and exports. Both canonical checks repeat `O -> O: The complete source-generated function interval matches byte for byte.` The modified graph header is included only by `PlayerActionGraph.cpp`; before this work that translation unit defined only the U-ranked `PlayerActionGraph::move`. The new output header is included only by its new source. Thus no previously accepted root depends on a changed header or translation unit, and no accepted-root preservation check is required by this source delta. The clean rebuild is not represented as a full 527-root oracle audit.

The allocation inventory records only fields actually written by this root, for every allocation including externally constructed objects. In particular it retains three post-constructor stores of `player[0xB4]`: action allocations 037/110/128 receive that value at offsets `0x1C/0x20/0x1C`. An unlisted word is unobserved, not an asserted zero. The unused fifth slots of the capacity-five transition group remain unobserved.

## Constructor contracts and table-ownership worklist

The next static call-site pass identifies 348 external constructor calls to 126 distinct targets, covering the root's non-inlined object initializers. `constructor_contracts.csv` records the receiver allocation extent and the observed R1-R3, outgoing-stack, and VFP argument positions. `constructor_calls.csv` records every supplied value as a Player-field source, prior allocation/interface, immediate, local string, or temporary passed by address. This is a caller-side ABI worklist, not a claim to have recovered every source-level class or parameter type. All repeated calls to the same target agree on the observed argument-position count and allocation size.

Three calls to `0x00251FEC` pass temporary values 3, 5, and 6 by address. That constructor immediately loads the pointed-to value and stores it at object `+8`; it does not retain the temporary pointer. The two calls to `0x001BAC1C` supply a property pointer in R1 and a float in S0 (literal 1.0, or configuration getter slot `+0x108`); its body squares S0 before storing it. Other VFP transport in this builder must not be promoted to a floating parameter merely because it uses a VFP register.

`field_writes.csv` retains all 77 original-root writes into newly allocated objects, including intermediate base/derived address-point stores, array contents and counts, post-constructor parameter injection, and the graph's initial-node assignment. `output_writes.csv` retains the nine output-handle writes in source order.

All 22 address points referenced by the root's inlined polymorphic initializers currently equal the starts of unnamed map data rows. Their two zero ABI-prefix words occur in the preceding rows. `vtable_ownership.csv` records each address point, observed prefix address, current and preceding row, candidate slot sequence, and the boundary before the next observed prefix. Those end boundaries are proposals requiring independent ownership review, not edits or accepted table identities.

This is an identity blocker for a conventional C++ virtual-class reconstruction under the unchanged canonical checker: a compiler vtable relocation refers to the symbol's complete-object table base, then adds eight to form the address point. The current rows cannot simply be named with the compiler vtable symbol while leaving it eight bytes too high. As an ABI control, the existing mapped `PlayerActionMultiCondition` table has symbol base `0x003D10D4`, while its address point is `0x003D10DC`; its project-built constructor contains an `R_ARM_ABS32` relocation to `_ZTV26PlayerActionMultiCondition` with addend eight at section offset 40. That constructor is still nonmatching; this observation makes no byte-exact claim for `0x00252054`.

Main must review and separately repair the relevant data identities/ownership before accepting ordinary source virtual tables for this root. None of the root's original function bounds need to change. Do not invent helper function addresses to replace these inlined constructors or assign the existing stream-class name to the player condition at `0x002D3F74`.

## Current-main adaptation and preservation

The source/notes proposal is also materialized on `dot/largest-root-current`, directly based on main `5025a6cd5ec8531fb1bc40ff4b570a0c01ef201c`. Source commit `05b2527` preserves main's out-of-line `PlayerActionNode::getAction` declaration and `#pragma no_inline` definition verbatim, and adds only the two dependency constructors and their layout fields. This supersedes the e2-only compatibility scope above.

The current-main worktree verifies the original EU SHA, then clean `TMP=/tmp python make.py eu -ca` compiles, links, and exports. The unchanged canonical checker reports:

```
_ZN17PlayerActionGraphC1Ev:
M -> O: The complete source-generated function interval matches byte for byte.
_ZN29PlayerActionGraphBuildOutputsC1Ev:
M -> O: The complete source-generated function interval matches byte for byte.
_ZN17PlayerActionGraph4moveEv:
O -> O: The complete source-generated function interval matches byte for byte.
```

The getter has an eight-byte source-generated object section but no standalone mapped/accepted root. Its preservation is covered by the accepted `PlayerActionGraph::move` canonical closure check, not claimed as a separate new match. The current graph object contains the 28-byte move root, 24-byte constructor, and eight-byte getter helper. No other source includes the changed graph header. The complete proposal differs from main only in source/header and these notes; map, ledger, STATE and tools have no committed delta.

Final checkpoint status: two new dependency proposals / 64 bytes; the previously accepted 28-byte graph move preserved; the 45,284-byte builder remains U with zero compile-diff forms. Its remaining source work has a complete topology/argument inventory, and canonical acceptance requires separately reviewed ownership of the 22 inlined virtual tables plus correction of the unrelated stream-constructor identity at `0x002D3F74`.
