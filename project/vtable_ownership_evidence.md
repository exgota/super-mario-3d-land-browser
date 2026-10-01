# NerveExecutor and LayoutActor data ownership

This is a narrow, independent ownership audit of the unchanged retail executable. It does not change the map, source, headers, target, hashes, ranks, tools, ledger, or shared project state. The root owns any metadata repair and canonical source acceptance.

The target SHA256 remains `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Full commands, source/header/object hashes, range hashes, table relocation identities, and complete-interval byte diagnostics are in `ownership_evidence.json`. `retail_ownership.txt`, `retail_adjacency.txt`, `retail_adjacent_endpoints.txt`, and `retail_constructor_calls.txt` contain the supporting retail reads and disassembly. All outputs are ignored by Git.

## Independent ownership findings

`al::NerveExecutor` owns the complete 20-byte table at `0x003D62FC..0x003D6310`. Its ABI header is two zero words: offset-to-top at `0x003D62FC`, and a zero RTTI pointer at `0x003D6300`. The address point is `0x003D6304`. Its entries are:

| Address | Retail value | Identity |
| --- | --- | --- |
| `0x003D6304` | `0x00331520` | Accepted `NerveExecutor::getNerveKeeper() const` |
| `0x003D6308` | `0x00268E3C` | Complete/base destructor, described below |
| `0x003D630C` | `0` | Generated deleting-destructor slot is zero in retail |

Retail constructor `0x0027B194..0x0027B1A8` writes address point `0x003D6304` at object offset zero and clears the keeper at offset four. The independently located destructor `0x00268E3C..0x00268E60` writes the same address point, scalar-deletes the keeper at offset four through established `_ZdlPv` at `0x002743AC`, and returns the original object. Independent derived destructors establish the distinction between base destruction and self-deallocation: `0x00318C44` tail-calls `0x00268E3C`; `0x00318C1C` calls it and then tail-calls scalar delete for the derived object. ARMCC emits D1/D2 aliases for this implementation. Its complete section is 36 bytes, including its literal pool; the ELF function-symbol size is 32 bytes.

The preceding table is a different complete object, `0x003D62EC..0x003D62FC`. Its two header words are zero, its address point is `0x003D62F4`, and both entries at `0x003D62F4` and `0x003D62F8` are zero. Constructor `0x001C7194..0x001C71DC` installs that address point and allocates a separate 12-byte polymorphic member, whose address point is `0x003D9484`. Its caller at `0x0024D754..0x0024D770` allocates eight bytes before calling that constructor. This is independent ownership, not an interpretation of the NerveExecutor candidate's relocation.

The following table is another complete object, `0x003D6310..0x003D6370`. Its header words are zero and its address point is `0x003D6318`. Constructor `0x0039A76C..0x0039A7B4` allocates 200 bytes, calls its base constructor, installs `0x003D6318`, and clears its own byte at offset `0xC4`. Destructor `0x001C7BB8..0x001C7BE8` installs the same point and then tail-calls its base destructor. The following header at `0x003D6370` and address point `0x003D6378` belong to a separately initialized object: static initializer `0x00387E54..0x00387E7C` stores `0x003D6378` into global object `0x003E2AE8` and registers its destructor `0x001C7BE8`.

The 22 entries at `0x003D6318..0x003D6370`, in address order, are `0x001C7BB8`, zero, `0x003315C0`, `0x0028CB70`, `0x002DABCC`, zero, zero, zero, zero, `0x002F2DCC`, `0x002F29D4`, `0x002F2DA8`, `0x002F2D8C`, `0x002F2A6C`, `0x002DAB24`, `0x002DABB0`, `0x00223A18`, `0x002DABB4`, zero, `0x002DABB8`, zero, and `0x001C7B94`. Their addresses establish slot identities; this audit does not invent source names for them.

`al::LayoutActor` owns the complete 80-byte table at `0x003D5FFC..0x003D604C`. Its primary ABI header is two zero words and its address point is `0x003D6004`. The audio secondary header at `0x003D602C` has offset-to-top -4 and a zero RTTI pointer; its address point is `0x003D6034`. The effect secondary header at `0x003D6040` has offset-to-top -8 and a zero RTTI pointer; its address point is `0x003D6048`.

| Address | Retail value | Identity |
| --- | --- | --- |
| `0x003D6004` | `0x0032FA78` | Accepted nerve getter |
| `0x003D6008` | `0x0027E82C` | Accepted appear |
| `0x003D600C` | `0x0027E85C` | Accepted kill |
| `0x003D6010` | `0x001BE04C` | Movement: independently observed alive gate and nerve/layout/effect/audio updates |
| `0x003D6014` | `0x001BE028` | Accepted calcAnim |
| `0x003D6018` | `0` | Generated primary audio-getter slot is zero in retail |
| `0x003D601C` | `0x0032FA80` | Accepted effect getter |
| `0x003D6020` | `0x001BE024` | Accepted control |
| `0x003D6024` | `0` | Unnamed virtual slot |
| `0x003D6028` | `0` | Unnamed virtual slot |
| `0x003D6034` | `0` | First audio-interface virtual slot |
| `0x003D6038` | `0` | Second audio-interface virtual slot |
| `0x003D603C` | `0x003765A0` | Accepted audio getter thunk for interface offset +4 |
| `0x003D6048` | `0x00376A3C` | Accepted effect getter thunk for interface offset +8 |

Retail constructor `0x0027E648..0x0027E694` installs these three points at offsets zero, four, and eight. Independent deleting destructor `0x0014C094..0x0014C0B4` installs the same points and tail-calls scalar delete. The complete-object allocation evidence is factory `0x002691F0..0x00269260`: it allocates 48 bytes, calls the LayoutActor constructor, then installs a derived LayoutActor table with the same two secondary offsets and no additional object storage.

The immediately preceding table is a different complete object, `0x003D5FEC..0x003D5FFC`. Its header words are zero, its address point is `0x003D5FF4`, and its two entries are `0x001BDB28` and `0x00330450`. `0x00330450` is an empty return. The first entry implements fog-area selection/interpolation, established from its own retail string literals and data accesses. Constructor `0x001BDD9C..0x001BDF40` installs `0x003D5FF4`. Its caller at `0x001C0BF4..0x001C0C08` loads allocation size `0x994` from literal `0x001C0CD0`, allocates, calls that constructor, and stores the result in a separate object field. Those two pointers are therefore not part of the 48-byte LayoutActor.

The following table is another complete object, `0x003D604C..0x003D6074`. Its header words are zero, and its address point is `0x003D6054`. Constructor `0x001BEDA0..0x001BEDD0` and deleting destructor `0x001BEDD0..0x001BEE30` independently install that point. Its eight entries are zero, `0x001BEDD0`, `0x001BECC0`, `0x001BE510`, `0x0032FCEC`, `0x001BED10`, `0x001BEC40`, and `0x001BECD0`. The next table begins at `0x003D6074`; its address point `0x003D607C` is independently installed by the already named `NerveAction` constructor at `0x00211BD8`.

## Ordinary C++ corroboration and byte diagnostics

Fresh scratch compilations use the configured ARMCC 4.1 build 791 flags without changing any flag. LayoutActor source and header are copied unchanged. The sole NerveExecutor header change is ordinary C++: replace the empty inline destructor with `virtual ~NerveExecutor() { delete mNerveKeeper; };`. No assembly or target-instruction arrays are used.

ARMCC emits an ordinary 20-byte NerveExecutor table with getter, D1, and D0 relocations at offsets 8, 12, and 16. It emits an ordinary 80-byte LayoutActor table with ten primary virtual entries, a three-entry audio secondary table, and a one-entry effect secondary table. The secondary offset-to-top values are -4 and -8, exactly as independently observed in retail. Each RTTI-pointer word is zero under the configured `--no_rtti_data` flags. Compiled function-pointer relocation identities are recorded separately from retail values, so zero retail slots are not silently replaced.

Standard relocations were evaluated only in temporary in-memory diagnostic bytes, using independently recovered table bases and the established scalar-delete and SafeString imports. The compiler objects remain unmodified, and the project check tool was not called. The full 20-byte NerveExecutor constructor, 36-byte NerveExecutor D1 section, and 76-byte LayoutActor constructor equal their unchanged retail intervals. This totals 132 diagnostic bytes and zero accepted functions.

There were two compiler invocations per class in this lane. The first invocation failed because the standalone process lacked ARMCC's standard-header environment. Adding the existing compiler's `ARMCC41INC` and `ARMCC41LIB` paths fixed it without changing source or compiler flags. The failed commands are preserved in `compile_environment_failure.json`; final commands, elapsed compiler times, and hashes are in `ownership_evidence.json`.

Rerun from the repository root:

```sh
. ./development_environment.sh
python build/phase_two_vtable_layouts/audit_vtable_ownership.py
```

## Precise minimal data-only repair proposal

The existing unnamed `dc` rows straddle genuine table boundaries. Do not name the old starts `0x003D62F4` or `0x003D5FF4` as the desired vtables. The minimum repair preserves exactly the original two union intervals, retains every byte, creates complete named tables, and leaves neighboring fragments unnamed. Every rank remains U. No function interval changes.

Replace the existing three `dc` rows spanning `0x003D5FDC..0x003D6054` with:

```csv
0x003D5FDC,          ,0x003D5FEC,          ,U,dc,,
0x003D5FEC,          ,0x003D5FFC,          ,U,dc,,
0x003D5FFC,          ,0x003D604C,          ,U,dc,_ZTVN2al11LayoutActorE,
0x003D604C,          ,0x003D6054,          ,U,dc,,
```

The first row is the preceding derived-NerveExecutor table's suffix. The last row is the following unrelated table's header. Keeping those fragments unnamed avoids claiming complete identities for unrelated classes.

Replace the existing three `dc` rows spanning `0x003D62E8..0x003D6318` with:

```csv
0x003D62E8,          ,0x003D62EC,          ,U,dc,,
0x003D62EC,          ,0x003D62FC,          ,U,dc,,
0x003D62FC,          ,0x003D6310,          ,U,dc,_ZTVN2al13NerveExecutorE,
0x003D6310,          ,0x003D6318,          ,U,dc,,
```

The first row is the earlier abstract-interface table's pure-virtual slot. The last row is the following unrelated table's header. The target never changes.

Per BRIEF hard rule 2, apply this metadata repair in a separate commit with this independent ownership evidence recorded in the decision log. Then, during the root's frozen apply window, the ordinary C++ destructor correction and constructor guards can be integrated and checked from canonical committed project ARMCC objects. Name the existing `0x00268E3C..0x00268E60` function row `_ZN2al13NerveExecutorD1Ev` if accepting that source. D2 is an alias, not another function interval. Only the project checker may set O.

## Uncertainty

The adjacent unnamed class names remain unrecovered; their concrete address points, entries, constructor/deallocation stores, and immediate bounds are established. The zero retail function entries align with slots emitted by ordinary C++, and virtual-function elimination is a plausible explanation. This audit does not prove the original linker mode or manufacture functions for those zeros. The zero RTTI pointers are directly observed. The repair does not depend on recovering their omitted type names or explaining why a particular unused slot is zero.

NerveExecutor's base storage at offsets zero and four is independently established, and ordinary C++ produces an eight-byte class. This lane found no standalone allocation that proves the complete retail base object has no extra unused storage. The table ownership and constructor interval do not depend on that additional claim.
