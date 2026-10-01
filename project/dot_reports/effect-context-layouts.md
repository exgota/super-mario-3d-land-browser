# Frozen initial layout evidence

This analysis was written before the parent's three canonical source builds. Its historical zero-compile statements refer to that initial analysis only. Current results and retained source are in effect-context-creation.md.

# Effect creation consumer, 0x002E54AC

2026-10-01. Analysis and first C++ proposal only. No compile, link, canonical check, production edit, map edit, or publication was performed by this worker. No byte-exact or functional-equivalence claim is made.

Target: existing unnamed U interval 0x002E54AC..0x002E5984, 1240 bytes, in main commit 48c97b5157698b6722a6d96330ad718f5351a854. The coordinator fetched main and confirmed eligibility before delegation. Local supplied code.bin SHA256 was verified as e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64. All binary observations below are from that local supplied binary. No removed library, leaked SDK, or external reference was used.

## Proposed deliverable and first hypothesis

- alEffectCreationFunction.cpp: one ordinary C++ semantic form for the complete routine
- alEffectCreationFields.h: partial views, offsets, 32-bit layout assertions, and declarations for independently identified imports
- retail-target.txt and retail-alternate-creation.txt: private local disassembly notes, not proposed production source

The source is intended for lib/al/src/Effect/ with its private header beside it. It is not currently in the active worktree. All names in effect_creation_detail describe observed roles; they do not assert the original library/class spelling. Unknown intervals are explicitly opaque and correspond to observed member offsets or independently established allocation strides, not to code-generation padding.

Hypothesis 1: the routine is a conventional set-pool allocator followed by matrix/default initialization and a descending, masked emitter allocation loop. A single probe counter is shared across both pool scans and across every selected emitter. The source preserves this unusual behavior. Initialization uses normal field assignments, two calls to the independently mapped matrix-copy function, the mapped sead::Random::getU32 when needed, and unsigned 32-bit LCG state advancement plus signed 64-bit range scaling. No invented helper address, assembly, instruction array, attribute-induced padding, or compiler option change is used.

The first form was proposed to the coordinator before any compilation. The form count is one; the compile count is zero. Further forms require concrete semantic/type hypotheses and remain bounded by the eight-form cap. The coordinator owns canonical build/check/integration.

## ABI

The routine saves 0x34 bytes of integer registers, 8 VFP bytes, then reserves 0x34 stack bytes: current SP is incoming SP minus 0x70. Its arguments are:

| Position | Observed role |
| --- | --- |
| r0 | Context pointer |
| r1 | Writable reference consisting of set pointer and identifier |
| r2 | Borrowed 3x4 matrix pointer |
| r3 | Signed resource-set index; target copies saved r3 from current SP+0x48 |
| incoming SP+0 | Resource-bank index |
| incoming SP+4 | Group/list ID, valid byte-sized group domain |
| incoming SP+8 | Emitter selection bit mask |

The result is exactly zero at failure paths and one at the successful finalization path. The draft chooses bool; binary evidence alone does not distinguish bool from a wider integer return type that only produces 0/1. The group argument is declared unsigned char because the only direct caller narrows it to a byte and the independent context constructor establishes 256 group heads. The target itself does not prove the original parameter spelling or signedness; this remains a legitimate type uncertainty rather than a claimed recovered name.

Only fn_0023F494 directly branches to this routine in the supplied image. It passes bank 0, its fourth argument narrowed to eight bits as group, and all emitter-selection bits set. Its void-pointer return import should be understood as boolean-valued. This correction does not itself justify another compile of the already capped caller packet.

## Control flow and edge cases

1. The routine reads the selected resource set's emitter count and compares it against context+0x868 using a signed comparison. This requires room for the entire resource count, even when the selection mask would choose fewer emitters. If insufficient, it reports emitter exhaustion and returns false. The reference is not cleared on this early path.
2. A set slot is free when its live-emitter count at slot+4 is zero. Before each probe, the context cursor at +0x834 is advanced and masked by context+0x18. The set array starts at context+0x10, slot stride 0x1D0. An unsuccessful probe increments the single shared probe counter and compares it against context+0x14. Exhaustion clears reference+0, reports set exhaustion, and returns false. A computed null slot pointer follows the same report/false path.
3. The selected slot is stored through reference+0 before either matrix copy. Two 48-byte transforms at slot+0xFC and +0x12C are copied from the borrowed input. Defaults and flags are initialized. The slot identifier and reference identifier both come from context+0x8AC. The bank and resource indices are stored at slot+0xF0/+0xF4. The live-emitter count is not explicitly reset here: free-slot selection already required it to be zero.
4. Emitter indices descend from resource count minus one to zero. Unselected bits are skipped. Emitter pool cursor +0x82C advances with mask +0x848. Slots have stride 0xB4 and are free when slot+0xA4 is null. Capacity is context+0x83C. Crucially, the r5 scan counter is never reset after set allocation or between selected emitters. The alternate creation routine 0x002E4E18 does reset its emitter scan count at 0x002E520C; that is different behavior and must not be copied here.
5. Emitter exhaustion reports an error, then ends the emitter loop. It does not return false or roll back emitters already allocated. Finalization still copies the live count to the stored emitter count, increments the context live-set count and next identifier, and returns true. A mask of zero likewise reaches success with zero emitters, after passing the full-count capacity precheck.
6. The emitter pointer is appended densely to the set's pointer array using live count, but its 24-byte parameter pointer selects by original resource index. Those are deliberately different indices when the mask skips members. The linked list is prepended through one of the 256 group heads; +0x9C is previous/newer and +0xA0 is next/older.
7. The resource seed at +8 is used unless zero; zero fetches one word from the global sead RNG captured at function entry. Seed low/high halves go to emitter+0x78/+0x7A, and the full word to +0x7C. Resource+0x84 equal to 0x7FFFFFFF sets set+0x1CF, corroborating the existing deletion filter interpretation.
8. Emitter+0xA4 is assigned from context+0x8C0 indexed by resource+0. The old random word is multiplied by signed resource+0x7C using a 64-bit product. Its high word is subtracted from signed resource+0x78 for emitter+0x80. Independently, random state advances as state * 0x41C64E6D + 0x3039, modulo 2^32. Emitter+4 becomes 0x7FFFFFFF.

The draft assumes structurally valid resource data: group 0..255, no more than eight resource members, valid pool/resource pointers, and sizes for which signed counters do not overflow. These domains are supported by fixed arrays and constructor sizing; no malformed-resource guards have been invented. Shift behavior outside index 0..31 and signed arithmetic overflow are not claimed equivalent. No differential execution has been performed.

## Independent layout evidence

### Context and allocation strides

Mapped constructor 0x002E5C94 allocates and initializes these fields independently of the target:

- +4: resource-bank pointer array, sized from +8 at 0x002E5CE8..0x002E5D00
- +0x14/+0x18: set capacity and mask, computed as power of two and capacity minus one at 0x002E5D34..0x002E5D48
- +0x1C: exactly 0x400 bytes cleared at 0x002E5D60..0x002E5D68, establishing 256 four-byte group heads
- +0x41C: emitter array, element size 0xB4, constructed via callback 0x002EBC08 at 0x002E5D7C..0x002E5DAC
- +0x10: set array, element size 0x1D0, constructed via callback 0x002E3C50 at 0x002E5E04..0x002E5E4C
- +0x83C/+0x848: emitter capacity/mask computed at 0x002E5D04..0x002E5D54
- +0x82C/+0x834: emitter/set cursors zeroed at 0x002E5FD0..0x002E5FFC
- +0x864: live-set count zeroed; +0x868 initialized from emitter capacity at 0x002E5FF4..0x002E6014
- +0x87C: persistent 48-byte matrix initialized by mapped matrix copy at 0x002E6190..0x002E6198
- +0x8AC: next identifier zeroed at 0x002E5FEC
- +0x8C0/+0x8C4/+0x8C8: three allocated implementation pointers at 0x002E5ED4..0x002E5F68

The set constructor 0x002E3C50 zeros +4/+8/+0xC. The context constructor writes each set's owner at +0 using the independently sized 0x1D0 array at 0x002E5F6C..0x002E5FCC.

The emitter constructor 0x002EBC08 calls the independent random-state constructor 0x002E4400 with emitter+0x78. The latter obtains three RNG words and stores two halfwords at +0/+2 and one word at +4, establishing the eight-byte subobject independently. This constructor and the creation-time single-seed assignment intentionally use different seeding procedures.

### Set members and source-level aggregate boundaries

The independently mapped alternate creation routine 0x002E4E18 performs a full set copy to stack and back while replacing emitters. It provides stronger aggregate-boundary evidence than target stores alone:

- Eight pointers at +0x10..+0x2C are copied with an eight-register group; the next region starts at +0x30
- +0x30..+0xEF is copied as exactly 0xC0 bytes at 0x002E4F1C..0x002E4F28; record propagation earlier in that function uses a 24-byte stride and fills up to eight records
- Words +0xF0/+0xF4/+0xF8 are copied individually
- +0xFC and +0x12C each use the known matrix-copy methods
- +0x15C and +0x180 copy three words each
- +0x168, +0x170, and +0x178 each copy two words at 0x002E4F70..0x002E4FA0
- +0x18C copies four words at 0x002E4FB0..0x002E4FBC
- +0x19C, +0x1A8, and +0x1B4 each occupy three words
- +0x1C0/+0x1C4/+0x1C8 are single words, followed by individual bytes +0x1CC..+0x1CF

The draft uses float vectors where the target's float stores and these copy boundaries corroborate them. It does not attach semantic labels such as scale, color, or translation to unknown blocks solely from default values. The parameters' six words are ordinary data fields, not encoded instructions; their exact original scalar types remain unknown.

### Resource structures and deletion/list fields

A resource-bank table pointer at +0x18 contains 40-byte records, independently accessed by 0x002E3CCC as well as target and alternate creation. +4 selects an array of eight-byte member entries; each entry's +4 is a resource pointer. Record+8 supplies the signed member count. The draft preserves all other resource-record bytes as opaque.

The accepted set-deletion routines 0x0024DD4C and 0x002E3BF8 corroborate set+8, +0xC, and pointer-array+0x10, emitter identifier+0x14, emitter resource+0xA8, and resource filter+0x84. Their committed source is lib/al/src/Effect/alEffectObjectDeletionFunction.cpp.

Mapped emitter deletion 0x002201C4 clears emitter+0xA4, decrements its owning set's +4, and increments context+0x868. It then removes the emitter from context[group]+0x1C using +0x9C/+0xA0 links at 0x002202C8..0x00220310. This independently establishes occupancy, remaining capacity, count, group, and list roles.

## Required imports and data evidence

| Import | Address | Evidence and integration requirement |
| --- | --- | --- |
| sead::Matrix34CalcCtr<float>::copy(nn::math::MTX34&, const nn::math::MTX34&) | 0x0027C18C | Existing mapped symbol `_ZN4sead15Matrix34CalcCtrIfE4copyERN2nn4math5MTX34ERKS4_`; body copies exactly twelve float registers and returns. Declared only. |
| sead::Random::getU32() | 0x0024B12C | Existing mapped symbol `_ZN4sead6Random6getU32Ev`; body updates four state words as an xorshift RNG and returns one word. Declared only. |
| fn_002200FC | 0x002200FC | Existing unnamed mapped routine. It dispatches the supplied message to installed callbacks; absent callbacks, it updates a context timestamp. The proposal does not invent its original class/name. |
| dat_003E26DC | 0x003E26DC..0x003E26E4 | Existing eight-byte U data row. Named sead::GlobalRandom::createInstance at 0x001049C0 independently initializes word 0 with the object pointer and word 1 with its disposer; target and independent RNG-state constructor read word 0. Retain whole-row identity; no invented four-byte split. |
| sead::Vector3<float>::ones | 0x004305EC..0x004305F8 | Independently proven by `__sti___14_seadVector_cpp`: 0x00383B38 loads the address from 0x00383D90 and 0x00383B3C/40/44 write 1.0f to the three elements. Already documented in lib/sead/README.md and project/dot_reports/area-cube.md. Current map has no row here; adding a data import needs coordinator review/evidence, not target-boundary adjustment. The supplied executable does not contain BSS bytes. |

No external function is assigned a guessed address. None of the imported routines is implemented in this proposal. The matrix and RNG names are existing clean map identities, not names copied from an excluded SDK. The partial declarations do not reconstruct unrelated library internals.

The target's literal pool starts at 0x002E583C but code resumes at 0x002E5884. The complete interval also includes the multiplier literal at 0x002E5980. A check must cover the entire 1240-byte interval, including pools and the post-pool instruction block.

Two local Shift-JIS diagnostic strings are reconstructed as ordinary C++ string literals. At 0x002E5840: "エミッタが枯渇しました\n" (emitter exhaustion). At 0x002E5858: "エミッタセットが枯渇しました\n" (emitter-set exhaustion). The other literals are the global holder address, ones-vector address, 1.0f, 0.0f, and LCG multiplier 0x41C64E6D. These are source data and constants; no string encodes executable code.

## Implications for packet 0023F494

This reconstruction independently upgrades the caller's context view from one isolated matrix offset to a well-supported context layout, confirms 256 groups and therefore the byte-narrowing role, and confirms the boolean-valued creation result. The independently visible allocator/creator still consumes a matrix. There is no separately recovered position-overload body or second direct call site showing how the caller's translation updates were originally factored. Accordingly, this work does not yet justify reopening the packet's eight-form cap merely with a renamed type, a member-function rewrite, or a speculative inline wrapper.

## Verification limits

The source has not been compiled or run. The header's ARM-only layout assertions have not yet been evaluated by ARMCC. Existing source is unchanged. The proposal has no target comparison, no matching claim, and no native/emulator differential claim. The two leading compile risks to inspect are aggregate-copy lowering for the two ones vectors and branch-local versus merged seed assignments; these are code-shape uncertainties, not reasons to change semantics or flags. The boolean and byte parameter source spellings remain tentative.

### Additional verification at 16:16 UTC

`import-and-arithmetic-evidence.txt` records the complete 0x00383AC8..0x00383B48 initializer slice: s1 is loaded from 0x00383D70 (word 0x3F800000) and is not changed before the three stores at 0x00383B3C/40/44. Their r0 address comes from 0x00383D90 (word 0x004305EC). It also records exact probe-counter and multiply instruction slices.

A Python algebra check tested 132 boundary pairs: 11 old-state values spanning 0..0xFFFFFFFF and 12 signed ranges spanning INT_MIN..INT_MAX. The unsigned 32x32 high product plus the range-sign correction used by retail agrees in every case with the low 32 bits of the signed 64-bit product shifted right 32 in the draft. The result is retained in `arithmetic-boundaries.json`. This validates the expression's integer algebra only; it is not C++ execution, original-machine-code execution, or a function-equivalence result.

Both pool probe comparisons are signed `bgt`, and both use the same r5 without intervening reset. The complete scan sections are retained in the additional evidence file. All intervening callees follow the ordinary preserved-r5 calling convention.
