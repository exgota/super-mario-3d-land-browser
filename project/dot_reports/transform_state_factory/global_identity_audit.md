# Independent transform factory global and declaration audit

Observed 2026-10-02 UTC. Read-only inspection of `mario-main861` at `86104d96a7f570383bbfefc5fffad998e134496c`, `mario-root-33ba3c` at `16f73469a7f5cd32943aa9647c7a8663a175ae0e`, and `mario-skeletal-animation-construction` at `84483e6252e02d125d88bacb154d2160eca95ab7`. No repository source, types, maps, bounds, ranks or other files were edited. The only audit write is this file. No external write or communication was attempted.

AGENTS.md, full project/BRIEF.md, project/STATE.md, latest daily report, DOT_BRIEF.md, and Guide.md were read. Owner `data/ver/eu/code.bin` was independently SHA-256 checked: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Disassembly below was independently decoded with Capstone from that file with executable base 0x00100000; proposal prose was not used as a replacement for the binary.

## Results

1. 0x00430C68 has an independently repeated 48-byte identity-matrix initialization and a corresponding four-byte guard at 0x003F389C. This is an observed initialized/consumed span [0x00430C68,0x00430C98), not recovered original linker symbol size or complete original allocation provenance.
2. 0x00430CF0 has an independently repeated 64-byte transform initialization under the separate four-byte guard 0x003F38A8: a 48-byte identity matrix, three float ones, and flags 0x7E1. The consumed/initialized span is [0x00430CF0,0x00430D30).
3. 0x00430D20 is 0x00430CF0+0x30. The factory's literal at 0x002A3170 supplies the address of the three float scale words of that transform. It is not independent evidence for a third allocation, a separately initialized object, or a separate original symbol. Defining an unrelated 12-byte global there would hide an observed overlap.
4. Main has no source declarations of the audited imports. The root-33BA3C proposal has one real declaration compatibility constraint: its fn_00291470 parameters use the nominal `transform_blend::Matrix34` type. Equal-sized independently named matrices do not create a compatible C++ function declaration merely because both declarations have C language linkage.
5. Canonical metadata does not have BSS rows at any of the three requested addresses. The matrix and transform spans cannot be accepted as original allocation boundaries merely by turning these access observations into map rows. No map change is proposed by this audit.

## 0x00430C68: independent identity initializer and consumers

`fn_00254890`, mapped interval [0x00254890,0x00254910), supplies a compact independent initializer/accessor:

- 0x00254890 loads literal 0x00254900 = guard 0x003F389C; 0x00254898 loads the guard word, and 0x0025489C tests bit 0.
- 0x002548A8 calls 0x0028A998 with the guard address if the low bit is clear.
- 0x002548B4 loads literal 0x0025490C = 0x00430C68. Literals 0x00254904 and 0x00254908 are float 1.0 and float 0.0 respectively.
- Stores 0x002548C4..0x002548EC initialize all twelve words: ones at +0x00,+0x14,+0x28 and zeros at the other nine offsets through +0x2C.
- 0x002548F8 reloads and returns address 0x00430C68 in r0. No incoming argument is used.

An independent constructor `0x001E4788` uses the same guard at 0x001E48B0 and 0x001E4920, performs the complete twelve-word identity stores at 0x001E48E0..0x001E4908 and 0x001E494C..0x001E4974, and calls the 48-byte copier 0x00291470 at 0x001E491C and 0x001E4988. The same constructor separately initializes the neighboring 64-byte matrix at 0x00430C98 under guard 0x003F38A0. These are separate initialization identities, but they do not identify the original aggregate/allocation or linker symbol extents.

`fn_00291470`, [0x00291470,0x00291490), compares destination r0 and source r1, returns immediately when equal, otherwise reads twelve single-precision words at source +0..+0x2C and writes exactly twelve words to destination +0..+0x2C. This confirms the matrix consumption size independently of any C++ struct declaration. Preserving r0 does not prove a source-language pointer return rather than void; pointers and references also have the same call-register representation here.

The factory has its own inline 0x003F389C-guarded initialization at 0x002A2E94..0x002A2F20 and again at 0x002A2F44..0x002A2FCC, followed by copies from 0x00430C68 to its matrix arrays.

An aligned literal-word inventory found 23 code-pool occurrences of 0x00430C68, at function starts:
`001E4788 001F3E58 001F4A0C 002164F8 0024F7A0 00254890 00261594 002840F4 0029A73C 0029C5C4 0029D854 0029FFFC 002A2C9C 002A5958 002A63D8 002A819C 002AFBA8 002B5608 002B6FD0 00335704 0033BA3C 003429BC 00392B00`.
Every occurrence has a corresponding 0x003F389C literal in that function. This inventory is not an exhaustive proof against address materialization by arithmetic, alternate encodings, or pointers obtained from other data.

## 0x00430CF0 and interior 0x00430D20

The factory's relevant literal pool is:

- 0x002A3170 = 0x00430D20
- 0x002A3174 = 0x3F800000 (float 1.0)
- 0x002A3178 = 0 (float 0.0)
- 0x002A317C = 0x003F38A8
- 0x002A3180 = 0x00430CF0
- 0x002A3184 = 0x000007E1
- 0x002A3188 = 0x003F389C
- 0x002A318C = 0x00430C68

Factory initialization:

- 0x002A2DE0..0x002A2E04 tests guard 0x003F38A8 and calls 0x0028A998.
- 0x002A2E08 loads destination 0x00430CF0 into sb; 0x002A2E0C obtains the identity matrix from fn_00254890; 0x002A2E18 copies 48 bytes there with fn_00291470.
- 0x002A2E2C,+0x30; 0x002A2E30,+0x34; and 0x002A2E34,+0x38 store float one at 0x00430D20/24/28.
- 0x002A2E38 writes 0x7E1 at +0x3C = 0x00430D2C. An earlier quick chat update initially called this value 0x801; that was explicitly corrected before this report. 0x7E1 is the independently decoded literal value.
- 0x002A2DCC loads the literal address 0x00430D20 once, and 0x002A2DDC preserves it at sp+0xE0.
- 0x002A2E50 copies the matrix from 0x00430CF0; 0x002A2E54..0x002A2E5C loads/stores the three scale words using the saved 0x00430D20 pointer; 0x002A2E64 reads the flags from 0x00430CF0+0x3C.
- 0x002A2E7C and 0x002A2E80..0x002A2E90 copy these matrix, scale and flag components into a 0x40-byte-stride output element. The source range is one coherent transform record as consumed here.

Independent accessor `0x002164F8`, interval ending 0x002165D8, establishes the same storage without the factory:

- Literal 0x002165BC = outer guard 0x003F38A8; literal 0x002165C0 = 0x00430CF0.
- Guard check/call at 0x00216504..0x0021651C. Nested matrix guard 0x003F389C and matrix address 0x00430C68 are literals 0x002165CC and 0x002165D4.
- Complete identity matrix stores at 0x00216554..0x00216580 if needed; a 48-byte matrix copy to 0x00430CF0 at 0x00216594.
- Three float-one stores at 0x00216598/9C/A0 to +0x30/+0x34/+0x38. Flags store at 0x002165A8 to +0x3C using literal 0x002165C4 = 0x7E1.
- Returns 0x00430CF0 at 0x002165B4.

Independent consumer `0x00261594` repeats that initialization twice, with guard literals 0x00261844 = 0x003F38A8 and 0x0026184C = 0x003F389C, matrix literal 0x0026185C = 0x00430C68, destination literal 0x00261858 = 0x00430CF0 and flags literal 0x00261848 = 0x7E1. Initializer copy calls are 0x0026165C and 0x00261704; tails are 0x00261660..0x00261670 and 0x00261708..0x00261718. It then passes this storage twice to 0x0025C3E4 at 0x0026173C.

Aligned literal inventory: 0x00430CF0 occurs exactly in the three examined functions (literal addresses 0x002165C0,0x00261858,0x002A3180). 0x00430D20 occurs only at 0x002A3170. Every 0x00430CF0 occurrence has a 0x003F38A8 guard literal. No separate initialization/guard for 0x00430D20 was found.

## Guard semantics and neighboring storage

The original fn_0028A998 [0x0028A998,0x0028A9B4) reads one 32-bit word. If it is zero, it writes one to that same word and returns one; otherwise it returns zero. It performs no second-word access. Therefore unsigned-word guard declarations are consistent with the observed implementation. The original language's guard class/typedef/signedness and generic thread-safety semantics are not proved by this implementation.

Existing main map rows already delimit guard 0x003F389C..0x003F38A0 and guard 0x003F38A8..0x003F38AC as four-byte data entries. This audit does not change those rows or their symbol names.

Nearby initialized/consumed storage supplies useful separation evidence:

- 0x00430C38 is a three-float zero vector, not a 48-byte matrix in the examined function 0x0038B36C. Guard 0x003F3894; stores at 0x0038B3AC..0x0038B3B4.
- 0x00430C44 is a nine-float identity matrix, guard 0x003F3898; stores at 0x0038B414..0x0038B438, ending at 0x00430C68.
- 0x00430C98 is the separate sixteen-float identity matrix, guard 0x003F38A0, initialized in constructor 0x001E4788 through +0x3C, ending at 0x00430CD8.
- 0x00430CD8 is a separate command-output buffer in 0x002F41B8; this audit observed a store at +0x10 (0x002F434C) and passes its base to 0x002542BC. This establishes a separate usage pattern, not its original full extent.
- 0x00430D30 has a separate guard 0x003F38AC in constructor 0x002B9A90. Stores at 0x002B9B00/04/08 initialize a zero word and two links pointing to 0x00430D34, then its address is passed to 0x002323E8. This begins at the end of the 64-byte transform's consumed span.

These adjoining uses strengthen the interpretation of the matrix/transform spans but remain insufficient to recover original linker symbols, complete object padding, arrays, aliases, or original enclosing allocation boundaries. In particular, the literal interior pointer 0x00430D20 must not independently justify a split.

## External declaration compatibility inventory

The inventory searched .h/.cpp source under Game and lib in all three checkouts, case-insensitively, for the exact neutral symbols and the relevant addresses. Main's historical Pro packet and response are separately noted below. Source was not compiled or changed during this audit.

| Import | Main861 actual source | root33BA3C proposal | Skeletal construction proposal | Compatibility consequence |
|---|---|---|---|---|
| fn_00254890 | None | None | None | Original returns pointer value 0x00430C68 without consuming arguments. A fresh pointer-return declaration is a reconstruction, not recovered source type identity. |
| fn_0028A998 | None | `extern C int fn_0028A998(unsigned*)` | None | Reuse exactly the same parameter/return types for source compatibility; ABI-compatible bool/int or signed/unsigned alternatives are not proof of declaration identity. |
| fn_00291470 | No actual declaration; map names it | `extern C void fn_00291470(transform_blend::Matrix34*, const transform_blend::Matrix34*)` | None | A new private Matrix34, float*, void*, or reference signature is a different function type despite the same two ARM pointer registers. Share the exact nominal type/declaration, or keep this as a clearly held compatibility issue. |
| fn_002B349C | None | None | None | Observed input r0 object, r1 allocator, r2 count, r3 zero/nonzero mode; writes object+0x10/+0x14 and invokes allocator slots+8/+0xC. This does not prove original class/enum/bool types or return declaration. |
| dat_003F389C | None | `extern C unsigned dat_003F389C` | None | Exact unsigned declaration can be shared. Independently named guard wrappers are not the same C++ object type. |
| dat_003F38A8 | None | None | None | Four-byte mutable guard access supports the same unsigned-word representation; exact original C++ identity remains unknown. |
| dat_003D82D0 | None | None | None | Existing whole main map interval is [003D82D0,003D830C), 60 bytes. Neutral unsized const unsigned array is consistent with the skeletal proposal's table declaration convention, but no shared declaration presently exists. |
| dat_003D86BC | None | None | None | Existing whole main map interval is [003D86BC,003D86F8), 60 bytes. Same caveat: typed-vtable object, unsigned array, and pointer object declarations are not interchangeable C++ identities. |

root33BA3C additionally declares `extern C transform_blend::Matrix34 dat_00430C68`; a new reconstruction of the same symbol as a different class or a float array would conflict at C++ type level even if both accesses were byte-identical. Its `transform_blend::Transform` has Matrix34, float scale[3], unsigned flags and sizeof 0x40. Those layout observations agree with the factory's transform data, but are not an automatic ODR equivalence to an independently declared Transform type.

Main `project/pro_requests/001EA220.md:138` and `project/pro_responses/001EA220.md:90` contain a historical `extern C void fn_00291470(EffectTransformationFields&, const EffectTransformationFields&)` proposal. That is not an active source declaration at the inspected base. It is a real incompatibility to resolve before reviving that proposal together with the root33BA3C pointer declaration. The packet itself says the original C++ API/return type is unknown.

The skeletal proposal's `skeletal_construction::Allocator`, `Evaluator`, `EvaluatorVtable`, and `Blend` and root33BA3C's private `transform_blend::Evaluator`, `EvaluatorVtable`, and `Root` are nominally distinct source types. Some records describe overlapping runtime objects, but no audited common external declaration currently uses those two private type families interchangeably. Matching field offsets or pointer-call ABI alone cannot authorize silently replacing one declaration with the other.

## Vtable address identities

The factory initially installs 0x003D86BC at 0x002A30E8..0x002A30EC, then calls helper 0x002B349C, then installs 0x003D82D0 at 0x002A3110..0x002A3114. Destructor-shaped routines independently install the corresponding addresses: 0x002A34D4 loads table 0x003D82D0 and stores it at object+0 at 0x002A34E0; 0x002B36C4 loads table 0x003D86BC and stores it at object+0 at 0x002B36D8.

The existing 60-byte table rows contain fifteen words each. Their first words are 0x002A34D4 and 0x002B36C4 respectively, agreeing with those independently observed routines. This establishes address/whole-row import evidence, not original C++ class names, vtable member types, or source declaration compatibility. No table words were reconstructed as source or changed.

## Scope limits

This audit supplies original-binary observations, a source declaration inventory, and separation of consumed spans from unknown original symbol/allocation boundaries. It does not supply new canonical data rows, source/API/type changes, matching claims, checker results, or acceptance credit. No compile was attempted because the task was read-only. The historical root proposal's report is appropriately cautious about missing canonical BSS; its passing mention of nearby 0x00430C38 as matrix storage should be read in light of the independently decoded zero-vector use above.
