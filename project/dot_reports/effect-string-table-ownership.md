# Effect string virtual-table ownership proposal

Read-only followup, 2026-10-01, current main `48c97b5157698b6722a6d96330ad718f5351a854`. This supplies a concrete data-only repair proposal for the main lane. No map row, boundary, rank, tool, object, or original byte was changed. The action-update source remains unverified by the canonical checker because these imports are unresolved; this note grants no exact credit.

## Three complete 20-byte tables

The independently inspected constructor at `0027AD3C` initializes capacity 64, binds buffer=this+12, terminates it and calls formatV. Its literals `0027AD9C`, `0027ADA0`, `0027ADA4` are respectively `003DA510`, `003DA288`, `003D7AF8`. The final pointer is installed at `0027AD80`. The first two intermediate constructor stores were optimized away, while their literal loads survive. They must not be described as three runtime vptr stores.

The named StringTmp<32> constructor at `0027BEC8` has the same sequence with capacity 32. Literals `0027BF28/2C/30` contain `003DA510`, `003DA260`, `003D7AE4`; it installs the final point at `0027BF0C`. The shared first point and capacity-specific later points corroborate the class layering. Independent 64-byte buffered member initialization in `002E9C54` at `002E9CA0..002E9CB4` actually stores the shared buffered vptr through `stm r0,{r4,r6}` at `002E9CA8`, with r4 loaded from `002E9D40=003DA510`.

Existing canonical ARMCC object `alEffectSetAction.o` emits three ordinary `.constdata__ZTV...` sections, each 20 bytes. Each has two ABI header words and three slots: two destructor slots and assureTermination. The corresponding retail intervals have two zero header words, two null destructor slots and final function `0039E0E4`. That implementation loads buffer/capacity at +4/+8 and stores a zero byte at buffer+capacity-1. Null destructor slots are observed contents; they are not executable imports or proof of a specific linker elimination mode.

| Type proposed from constructor layering | ABI base | Point | Complete end |
|---|---|---|---|
| sead::BufferedSafeStringBase<char> | 003DA508 | 003DA510 | 003DA51C |
| sead::FixedSafeString<64> | 003DA280 | 003DA288 | 003DA294 |
| al::StringTmp<64> | 003D7AF0 | 003D7AF8 | 003D7B04 |

Each 20-byte retail interval has SHA-256 `8e0ec3978d0c26ab22e63bc128971bee641ebe0fb92eda64c43a9ea03b68549d`. Identical bytes do not collapse the three independently referenced addresses into one source identity. The ordinary generated references carry `_ZTV` addend +8, which requires the bases above rather than the current row starts at the address points.

## Adjacent ownership constraints

Buffered table: predecessor constructor `002E2488` loads `003DA4F0` from literal `002E24C0` and stores it into its object at `002E2494`. The preceding table's trailing slots run through `003DA504`; the two following zero words at `003DA508/50C` form the buffered header. The next point `003DA524` appears independently in wide-string constructor `00225B68` (literal `00225BC4`, load `00225B80`), whose capacity is 2 and termination uses halfword stores. Its assureTermination slot is `0039E0FC`, the independent halfword-termination implementation. This places its header at `003DA51C`, beyond the char table.

Fixed64 table: preceding point `003DA274` is loaded at `002E9FC0` from `002EA168` during a 512-byte buffered member initialization; that intermediate vptr store is optimized away. The three preceding slots end at `003DA27C`. Following point `003DA29C` is independently loaded from `0021F25C` during 96-byte buffered object construction `0021F21C`; its intermediate store also disappears before the final derived pointer. The repeated compiler-supported five-word shape and these independent capacity-specific references place the 64-byte table between headers `003DA280` and `003DA294`. These optimized-away loads are weaker than direct installations; main should review this limitation when accepting ownership.

StringTmp64 table: preceding 32-byte constructor directly installs point `003D7AE4`; its final termination slot is `003D7AEC`, followed by the 64-byte table header at `003D7AF0`. The next row starts at `003D7B04`, whose first word is a nonzero pointer `003D7B5C`, followed by other nonzero pointers. It is separately consumed as a construction-pointer table: `002E27FC` loads `003D7B04` from `002E28E0`, and `002E2814..002E2838` reads its entries for a different stack object. Thus the next row is not another null ABI header within StringTmp64. No original name or complete extent is asserted for this adjacent structure.

## Minimal repair for main review

Preserve every byte and the exact union of each pair of current rows, all ranks U and all function intervals. These are proposed metadata rows, not edits made by this lane. Main can record/revalidate them in its separate data-boundary decision commit, following BRIEF rule 2.

```csv
0x003DA4F0,          ,0x003DA508,          ,U,dc,,
0x003DA508,          ,0x003DA51C,          ,U,dc,_ZTVN4sead22BufferedSafeStringBaseIcEE,
0x003DA51C,          ,0x003DA524,          ,U,dc,,

0x003DA274,          ,0x003DA280,          ,U,dc,,
0x003DA280,          ,0x003DA294,          ,U,dc,_ZTVN4sead15FixedSafeStringILi64EEE,
0x003DA294,          ,0x003DA29C,          ,U,dc,,

0x003D7AE4,          ,0x003D7AF0,          ,U,dc,,
0x003D7AF0,          ,0x003D7B04,          ,U,dc,_ZTVN2al9StringTmpILi64EEE,
```

Current row unions are respectively `003DA4F0..003DA524` (52 bytes), `003DA274..003DA29C` (40), and `003D7AE4..003D7B04` (32). All outer endpoints remain fixed. This note follows the data-only ownership review pattern documented by main in `project/nerve_state_base_data_evidence.md`; it does not assume that example grants the dot lane permission to change metadata. Canonical source closure and byte matching must be rerun after any main-lane repair. The last action candidate already has instruction/stack differences, so fixing imports alone is not an exact-match claim.
