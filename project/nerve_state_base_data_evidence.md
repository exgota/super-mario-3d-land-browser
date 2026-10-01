# NerveStateBase data ownership

The complete `_ZTVN2al14NerveStateBaseE` table is `0x003D72E4..0x003D730C`, 40 bytes. Its primary address point is `0x003D72EC`. The current data rows put the two ABI header words in the preceding row and append the following class's header to NerveStateBase's eight entries.

The unchanged production constructor at `0x002684E0..0x00268500` emits 32 bytes including its literal pool. Ordinary ARMCC C++ diagnostics agree with every retail byte when the table import uses the independently established base `0x003D72E4`. This is a diagnostic, not an accepted match. Only canonical committed project output and the project checker establish acceptance.

The unchanged target SHA256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. This lane edited only ignored scratch files. It did not edit production, the map, ranks, function intervals, ledger, target, tools, or shared state. Root's concurrent source and map changes are outside this lane.

## Independent retail identity

Named constructor `0x002684E0..0x00268500` calls independently named `NerveExecutor` constructor `0x0027B194`, loads the literal `0x003D72EC` at `0x002684FC`, writes that pointer to object offset zero, and sets the byte at object offset eight to one. Its accepted `appear` and `kill` helpers independently clear and set that same byte. Its accepted `update` helper calls NerveExecutor's update function, reads the byte at eight, and dispatches the function at primary-table offset `0x1C` when alive. That dispatch identifies the final control slot directly from retail behavior.

The header at `0x003D72E4` has offset-to-top zero. The RTTI-pointer word at `0x003D72E8` is zero. The eight primary entries are:

| Offset from address point | Retail entry address | Retail value | Slot identity |
| --- | --- | --- | --- |
| `0x00` | `0x003D72EC` | `0x00331520` | Inherited getNerveKeeper |
| `0x04` | `0x003D72F0` | zero | Complete/base destructor slot in generated C++ |
| `0x08` | `0x003D72F4` | zero | Deleting destructor slot in generated C++ |
| `0x0C` | `0x003D72F8` | `0x001E2DA0` | init |
| `0x10` | `0x003D72FC` | `0x001E2DB0` | appear |
| `0x14` | `0x003D7300` | `0x001E2DA4` | kill |
| `0x18` | `0x003D7304` | `0x001E2DBC` | update |
| `0x1C` | `0x003D7308` | `0x001E2DF0` | control |

No thunk, secondary header, or secondary address point occurs in this table. The inherited keeper getter returns complete-object offset four. The accepted named helpers establish the state byte at eight. Independently constructed derived state `0x00141630..0x0014168C` calls NerveStateBase at `0x00141644`, then begins its additional fields at offset `0x0C`, with the next field at `0x10`. This corroborates the current 12-byte base layout. No standalone allocation of the base itself was found in the narrow call audit.

## Adjacent owners

The preceding table is `0x003D72D4..0x003D72E4`, 16 bytes. Its zero header words are at `0x003D72D4` and `0x003D72D8`; its point is `0x003D72DC`. Its two entries are `0x001E2AD4` and `0x001C9E40`. Constructor `0x001E2BCC..0x001E2BEC` independently installs `0x003D72DC` and clears its own fields at offsets four, eight, and twelve. Caller `0x001CE85C..0x001CE89C` independently allocates 16 bytes, calls this constructor at `0x001CE870`, saves the object at its parent's offset `0x24`, then dispatches its first virtual entry. It therefore owns the two entries before NerveStateBase's header. No class or method name is invented for it.

The earlier table `0x003D72C4..0x003D72D4` has two zero header words and two null entries. Its point `0x003D72CC` is independently installed by constructor `0x001E2AB8..0x001E2AD4`, which also stores its object pointer in global `0x003EF5BC`. This establishes a separate predecessor without relying on the current shifted data rows.

The immediate successor is `0x003D730C..0x003D7324`, 24 bytes. Its two header words are zero and its address point is `0x003D7314`. Its four entries are `0x00335FD8`, `0x00335DD4`, `0x00335F7C`, and `0x00335AFC`. In independent retail context `0x001DA950..0x001DA980`, the instruction at `0x001DA95C` loads point `0x003D7314` from literal `0x001DAAD0`. The instruction at `0x001DA964` stores it into stack offset `0x30`; `0x001DA978` passes that stack object to `0x002443A8`. This is a separate polymorphic stack object, not a NerveStateBase virtual-table extension. The nearby retail text includes `StreetPass_BoxTitle`, but that alone does not establish a source-level class name.

The following independent class has its header at `0x003D7324` and point at `0x003D732C`. Routine `0x001C7CD8..0x001C7D80` allocates 152 bytes and installs `0x003D732C`, plus three secondary points derived from it. This separate installation establishes the four-entry successor's endpoint at `0x003D7324`. The narrow audit does not claim the following class's complete table extent.

## Ordinary C++ corroboration

The scratch probe copies `lib/al/src/Nerve/alNerveStateBase.cpp` and `lib/al/include/Nerve/alNerveStateBase.h` unchanged. ARMCC 4.1 build 791 emits one 40-byte table with two zero header words and relocations for the inherited getter, D1 destructor, D0 destructor, init, appear, kill, update, and control in that order. It emits the 32-byte constructor with a direct call relocation to `_ZN2al13NerveExecutorC1EPKc` and an absolute relocation to `_ZTVN2al14NerveStateBaseE`.

Two successful compilations were recorded. Root's NerveExecutor destructor-header correction was observed during this audit, prompting a second compilation. Hashes show that both invocations already used the corrected ancestor header. The initial compilation, object, and log are preserved under `initial_invocation/`. Both runs give the same 40-byte shape and 32-byte constructor equality. The current `ownership_evidence.json` records the corrected ancestor-header hash and the second invocation. Neither run modifies compiler flags, emits assembly or target-instruction arrays, edits objects, calls the checker, or changes ranks.

Applying standard relocations only to an in-memory diagnostic copy of the constructor section, using base constructor `0x0027B194` and table base `0x003D72E4`, gives identical generated and retail SHA256 `e0e9b7eb03b4654eab645925aa671ab3d6d0c8a165bcac5f08f6088a83e7bf59`. All 32 bytes agree. This lane reports zero accepted functions.

The complete table's retail SHA256 is `d9c39e1083999d25ba5c8e24aab5b6bf97149e797598dca014be6f6931d534a0`. Commands, compiler environment, source/header/object hashes, binary-range hashes, table words, point references, ordinary table relocations, and constructor diagnostics are in `ownership_evidence.json`. Focused retail instructions are in `retail_ownership.txt`. Reproduce from the repository root:

```sh
. ./development_environment.sh
python build/phase_two_vtable_layouts/audit_nerve_state_ownership.py
```

## Data-only repair proposal

The existing two unnamed `dc` rows span `0x003D72DC..0x003D7314`, with sizes 16 and 40 bytes. Replace them with the preceding table's last two entries, the full NerveStateBase table, and the following header fragment. Preserve the exact 56-byte union interval and every byte. Keep all ranks U and every function interval unchanged.

```csv
0x003D72DC,          ,0x003D72E4,          ,U,dc,,
0x003D72E4,          ,0x003D730C,          ,U,dc,_ZTVN2al14NerveStateBaseE,
0x003D730C,          ,0x003D7314,          ,U,dc,,
```

This minimal repair leaves neighboring fragments outside that union unchanged. Apply any repair in root's separate metadata evidence commit and decision log, as required by BRIEF hard rule 2. The corrected table start and name then resolve the constructor's genuine C++ import. No constructor source change is proposed.

## Uncertainty and separate finding

The two null destructor slots are observed values. Ordinary C++ establishes their slot identities, but the audit does not prove the original linker's elimination mode or treat zero as an executable destructor address. Neighboring classes remain unnamed; neither gap affects NerveStateBase ownership.

The existing map's `code_end` for `0x001DA7B4..0x001DAB04` is `0x001DA92C`, before the independently decoded instructions at `0x001DA95C..0x001DA97C`. This audit reads the narrow actual context to establish the successor's point. It proposes no function-boundary repair and leaves this separate finding to root.
