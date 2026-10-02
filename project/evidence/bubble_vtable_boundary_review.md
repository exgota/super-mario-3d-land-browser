# Bubble virtual-table boundary review

The original EU executable supports a complete Bubble table at 003D27DC..003D2874, 152 bytes, with the primary address point at 003D27E4. This review compiled no candidate source and read no matching object. The constructor and neighboring constructors establish the table boundary independently of desired compiler output.

## Original identity chain

The original actor factory entry 003B9A40 stores key pointer 003DFF7B, whose null-terminated text is `Bubble`, and creator 0039818C. The creator loads 0x94 into the first argument before calling the existing `_ZnwjRKSt9nothrow_t` at 002932B0. On success it constructs a SafeString on its stack and directly calls 0030350C. Allocation 0x94 is 148 bytes.

The constructor calls the existing MapObjActor constructor 00280428. It reads 003D27E4 from its literal at 00303594 and stores that value to object offset 0 at 00303520. Its adds at 00303530/34/38 form address points 003D284C, 003D2858 and 003D286C, then the store at 00303540 writes them to object offsets 4, 8 and C. These are primary-address-point offsets 68, 74 and 88. The constructor initializes its final observed pointer field at 90 and returns before the original pool 0030358C..003035A0. No source/header repair is part of this review.

The committed source header at 88d6105 already spells the class `Bubble`. The existing named original initializer `__sti___10_Bubble_cpp` independently identifies its translation unit. `_ZTV6Bubble` is a proposed C++ ABI label that uses that class spelling and the original runtime/constructor identity. The stripped executable does not supply this public symbol string. No table name is inferred from matching compiled bytes.

## Complete original Bubble table

The original table contains 38 words. The primary null prefix has 2 words. The primary address-point range 003D27E4..003D2844 has 24 entries. Three secondary subtable ranges follow: effect 003D2844..003D2850, audio 003D2850..003D2864 and stage 003D2864..003D2874. Their offset-to-top words are -4, -8 and -12, respectively. Their address points exactly equal the constructor stores. Each nonzero method address equals an existing function start; no interior pointer or new helper address is introduced.

| Address | Table offset | Observed word | Existing function identity or role |
|---|---|---|---|
| 0x003D27DC | 0x00 | 0x00000000 | Primary header word 0 |
| 0x003D27E0 | 0x04 | 0x00000000 | Primary header word 1 |
| 0x003D27E4 | 0x08 | 0x003375F4 | Primary virtual slot 0: _ZNK2al9LiveActor14getNerveKeeperEv |
| 0x003D27E8 | 0x0C | 0x0030324C | Primary virtual slot 1: unnamed function 0x0030324C |
| 0x003D27EC | 0x10 | 0x001EBE64 | Primary virtual slot 2: _ZN2al9LiveActor18initAfterPlacementEv |
| 0x003D27F0 | 0x14 | 0x0027FF70 | Primary virtual slot 3: _ZN2al9LiveActor6appearEv |
| 0x003D27F4 | 0x18 | 0x0027EA40 | Primary virtual slot 4: _ZN2al9LiveActor17makeActorAppearedEv |
| 0x003D27F8 | 0x1C | 0x0027EBF0 | Primary virtual slot 5: _ZN2al9LiveActor4killEv |
| 0x003D27FC | 0x20 | 0x0027D250 | Primary virtual slot 6: _ZN2al9LiveActor13makeActorDeadEv |
| 0x003D2800 | 0x24 | 0x0026DF44 | Primary virtual slot 7: _ZN2al9LiveActor8movementEv |
| 0x003D2804 | 0x28 | 0x0025AA34 | Primary virtual slot 8: _ZN2al9LiveActor8calcAnimEv |
| 0x003D2808 | 0x2C | 0x00000000 | Primary virtual slot 9: null |
| 0x003D280C | 0x30 | 0x002795C0 | Primary virtual slot 10: _ZN2al9LiveActor12startClippedEv |
| 0x003D2810 | 0x34 | 0x002797F4 | Primary virtual slot 11: _ZN2al9LiveActor10endClippedEv |
| 0x003D2814 | 0x38 | 0x003031D0 | Primary virtual slot 12: unnamed function 0x003031D0 |
| 0x003D2818 | 0x3C | 0x00303130 | Primary virtual slot 13: unnamed function 0x00303130 |
| 0x003D281C | 0x40 | 0x003375DC | Primary virtual slot 14: _ZNK2al9LiveActor10getBaseMtxEv |
| 0x003D2820 | 0x44 | 0x00337604 | Primary virtual slot 15: _ZNK2al9LiveActor15getEffectKeeperEv |
| 0x003D2824 | 0x48 | 0x003375FC | Primary virtual slot 16: _ZNK2al9LiveActor14getAudioKeeperEv |
| 0x003D2828 | 0x4C | 0x00000000 | Primary virtual slot 17: null |
| 0x003D282C | 0x50 | 0x00000000 | Primary virtual slot 18: null |
| 0x003D2830 | 0x54 | 0x001EBF2C | Primary virtual slot 19: _ZN2al9LiveActor7controlEv |
| 0x003D2834 | 0x58 | 0x00000000 | Primary virtual slot 20: null |
| 0x003D2838 | 0x5C | 0x001EBCEC | Primary virtual slot 21: _ZN2al9LiveActor14updateColliderEv |
| 0x003D283C | 0x60 | 0x00000000 | Primary virtual slot 22: null |
| 0x003D2840 | 0x64 | 0x00000000 | Primary virtual slot 23: null |
| 0x003D2844 | 0x68 | 0xFFFFFFFC | Effect offset-to-top -4 |
| 0x003D2848 | 0x6C | 0x00000000 | Effect null header word |
| 0x003D284C | 0x70 | 0x003765A8 | Effect address point: _ZThn4_NK2al9LiveActor15getEffectKeeperEv |
| 0x003D2850 | 0x74 | 0xFFFFFFF8 | Audio offset-to-top -8 |
| 0x003D2854 | 0x78 | 0x00000000 | Audio null header word |
| 0x003D2858 | 0x7C | 0x00000000 | Audio address point, null slot |
| 0x003D285C | 0x80 | 0x00000000 | Audio null slot |
| 0x003D2860 | 0x84 | 0x00376A44 | Audio getter slot: _ZThn8_NK2al9LiveActor14getAudioKeeperEv |
| 0x003D2864 | 0x88 | 0xFFFFFFF4 | Stage offset-to-top -12 |
| 0x003D2868 | 0x8C | 0x00000000 | Stage null header word |
| 0x003D286C | 0x90 | 0x00375F28 | Stage address point: _ZThn12_NK2al9LiveActor20getStageSwitchKeeperEv |
| 0x003D2870 | 0x94 | 0x00375E4C | Stage initializer slot: _ZThn12_N2al9LiveActor21initStageSwitchKeeperEv |

The original secondary getter thunks 003765A8, 00376A44 and 00375F28 each execute `ldr r0, [r0, #0x30]; bx lr`. They use the supplied subobject receiver directly, rather than subtracting its offset first. With the independently stored receiver points at object offsets 4, 8 and C, they access complete-object fields 34, 38 and 3C. The stage initializer thunk 00375E4C explicitly subtracts C from its receiver at 00375E50 before storing the keeper at complete-object offset 3C. This corroborates the three secondary table roles. Null words remain observed nulls; this report makes no runtime call-safety claim for those slots.

## Independent complete neighboring tables

The immediately previous complete table is 003D2744..003D27DC/152, address point 003D274C. Original registry 003B9F80 contains runtime key `TreeA` at 003DFEBA and creator 00398154. That creator allocates 0xB8 and directly calls constructor 002F9EDC. The constructor loads 003D274C from its literal 002F9F8C and writes the primary and secondary points at object offsets 0/4/8/C. Its secondary offsets are 68/74/88. The table ends with the stage initializer pointer at 003D27D8. Its next two words 003D27DC/003D27E0 are zero and precede Bubble's independently stored primary point. `TreeA` is a runtime key, not a proposed public C++ class name.

| Address | Table offset | Observed word | Existing function identity or role |
|---|---|---|---|
| 0x003D2744 | 0x00 | 0x00000000 | Primary header word 0 |
| 0x003D2748 | 0x04 | 0x00000000 | Primary header word 1 |
| 0x003D274C | 0x08 | 0x003375F4 | Primary virtual slot 0: _ZNK2al9LiveActor14getNerveKeeperEv |
| 0x003D2750 | 0x0C | 0x002F9C04 | Primary virtual slot 1: unnamed function 0x002F9C04 |
| 0x003D2754 | 0x10 | 0x002F9B18 | Primary virtual slot 2: unnamed function 0x002F9B18 |
| 0x003D2758 | 0x14 | 0x0027FF70 | Primary virtual slot 3: _ZN2al9LiveActor6appearEv |
| 0x003D275C | 0x18 | 0x0027EA40 | Primary virtual slot 4: _ZN2al9LiveActor17makeActorAppearedEv |
| 0x003D2760 | 0x1C | 0x0027EBF0 | Primary virtual slot 5: _ZN2al9LiveActor4killEv |
| 0x003D2764 | 0x20 | 0x0027D250 | Primary virtual slot 6: _ZN2al9LiveActor13makeActorDeadEv |
| 0x003D2768 | 0x24 | 0x0026DF44 | Primary virtual slot 7: _ZN2al9LiveActor8movementEv |
| 0x003D276C | 0x28 | 0x00177B60 | Primary virtual slot 8: unnamed function 0x00177B60 |
| 0x003D2770 | 0x2C | 0x00000000 | Primary virtual slot 9: null |
| 0x003D2774 | 0x30 | 0x002795C0 | Primary virtual slot 10: _ZN2al9LiveActor12startClippedEv |
| 0x003D2778 | 0x34 | 0x002797F4 | Primary virtual slot 11: _ZN2al9LiveActor10endClippedEv |
| 0x003D277C | 0x38 | 0x002F96D8 | Primary virtual slot 12: unnamed function 0x002F96D8 |
| 0x003D2780 | 0x3C | 0x002F9538 | Primary virtual slot 13: unnamed function 0x002F9538 |
| 0x003D2784 | 0x40 | 0x003375DC | Primary virtual slot 14: _ZNK2al9LiveActor10getBaseMtxEv |
| 0x003D2788 | 0x44 | 0x00337604 | Primary virtual slot 15: _ZNK2al9LiveActor15getEffectKeeperEv |
| 0x003D278C | 0x48 | 0x003375FC | Primary virtual slot 16: _ZNK2al9LiveActor14getAudioKeeperEv |
| 0x003D2790 | 0x4C | 0x00000000 | Primary virtual slot 17: null |
| 0x003D2794 | 0x50 | 0x00000000 | Primary virtual slot 18: null |
| 0x003D2798 | 0x54 | 0x002F9DEC | Primary virtual slot 19: unnamed function 0x002F9DEC |
| 0x003D279C | 0x58 | 0x00000000 | Primary virtual slot 20: null |
| 0x003D27A0 | 0x5C | 0x001EBCEC | Primary virtual slot 21: _ZN2al9LiveActor14updateColliderEv |
| 0x003D27A4 | 0x60 | 0x00000000 | Primary virtual slot 22: null |
| 0x003D27A8 | 0x64 | 0x00000000 | Primary virtual slot 23: null |
| 0x003D27AC | 0x68 | 0xFFFFFFFC | Effect offset-to-top -4 |
| 0x003D27B0 | 0x6C | 0x00000000 | Effect null header word |
| 0x003D27B4 | 0x70 | 0x003765A8 | Effect address point: _ZThn4_NK2al9LiveActor15getEffectKeeperEv |
| 0x003D27B8 | 0x74 | 0xFFFFFFF8 | Audio offset-to-top -8 |
| 0x003D27BC | 0x78 | 0x00000000 | Audio null header word |
| 0x003D27C0 | 0x7C | 0x00000000 | Audio address point, null slot |
| 0x003D27C4 | 0x80 | 0x00000000 | Audio null slot |
| 0x003D27C8 | 0x84 | 0x00376A44 | Audio getter slot: _ZThn8_NK2al9LiveActor14getAudioKeeperEv |
| 0x003D27CC | 0x88 | 0xFFFFFFF4 | Stage offset-to-top -12 |
| 0x003D27D0 | 0x8C | 0x00000000 | Stage null header word |
| 0x003D27D4 | 0x90 | 0x00375F28 | Stage address point: _ZThn12_NK2al9LiveActor20getStageSwitchKeeperEv |
| 0x003D27D8 | 0x94 | 0x00375E4C | Stage initializer slot: _ZThn12_N2al9LiveActor21initStageSwitchKeeperEv |

The immediately next complete table is 003D2874..003D290C/152, address point 003D287C. Original registry 003B9BC0 contains runtime key `Bunbun` at 003E02E2 and creator 003981C4. That creator allocates 0xF0 and directly calls constructor 00303E74. The constructor loads 003D287C from literal 00303EF4 and stores it at object offset 0, with secondary points 003D28E4/003D28F0/003D2904 stored at offsets 4/8/C. It derives the latter two from the effect point by +C/+20, again equivalent to primary offsets 68/74/88. The table's last word 003D2908 is the stage initializer pointer.

| Address | Table offset | Observed word | Existing function identity or role |
|---|---|---|---|
| 0x003D2874 | 0x00 | 0x00000000 | Primary header word 0 |
| 0x003D2878 | 0x04 | 0x00000000 | Primary header word 1 |
| 0x003D287C | 0x08 | 0x003375F4 | Primary virtual slot 0: _ZNK2al9LiveActor14getNerveKeeperEv |
| 0x003D2880 | 0x0C | 0x00303824 | Primary virtual slot 1: unnamed function 0x00303824 |
| 0x003D2884 | 0x10 | 0x001EBE64 | Primary virtual slot 2: _ZN2al9LiveActor18initAfterPlacementEv |
| 0x003D2888 | 0x14 | 0x0027FF70 | Primary virtual slot 3: _ZN2al9LiveActor6appearEv |
| 0x003D288C | 0x18 | 0x0027EA40 | Primary virtual slot 4: _ZN2al9LiveActor17makeActorAppearedEv |
| 0x003D2890 | 0x1C | 0x0027EBF0 | Primary virtual slot 5: _ZN2al9LiveActor4killEv |
| 0x003D2894 | 0x20 | 0x0027D250 | Primary virtual slot 6: _ZN2al9LiveActor13makeActorDeadEv |
| 0x003D2898 | 0x24 | 0x0026DF44 | Primary virtual slot 7: _ZN2al9LiveActor8movementEv |
| 0x003D289C | 0x28 | 0x0025AA34 | Primary virtual slot 8: _ZN2al9LiveActor8calcAnimEv |
| 0x003D28A0 | 0x2C | 0x00000000 | Primary virtual slot 9: null |
| 0x003D28A4 | 0x30 | 0x002795C0 | Primary virtual slot 10: _ZN2al9LiveActor12startClippedEv |
| 0x003D28A8 | 0x34 | 0x002797F4 | Primary virtual slot 11: _ZN2al9LiveActor10endClippedEv |
| 0x003D28AC | 0x38 | 0x003036EC | Primary virtual slot 12: unnamed function 0x003036EC |
| 0x003D28B0 | 0x3C | 0x00303628 | Primary virtual slot 13: unnamed function 0x00303628 |
| 0x003D28B4 | 0x40 | 0x003375DC | Primary virtual slot 14: _ZNK2al9LiveActor10getBaseMtxEv |
| 0x003D28B8 | 0x44 | 0x00337604 | Primary virtual slot 15: _ZNK2al9LiveActor15getEffectKeeperEv |
| 0x003D28BC | 0x48 | 0x003375FC | Primary virtual slot 16: _ZNK2al9LiveActor14getAudioKeeperEv |
| 0x003D28C0 | 0x4C | 0x00000000 | Primary virtual slot 17: null |
| 0x003D28C4 | 0x50 | 0x00000000 | Primary virtual slot 18: null |
| 0x003D28C8 | 0x54 | 0x00303E1C | Primary virtual slot 19: unnamed function 0x00303E1C |
| 0x003D28CC | 0x58 | 0x00000000 | Primary virtual slot 20: null |
| 0x003D28D0 | 0x5C | 0x001EBCEC | Primary virtual slot 21: _ZN2al9LiveActor14updateColliderEv |
| 0x003D28D4 | 0x60 | 0x00000000 | Primary virtual slot 22: null |
| 0x003D28D8 | 0x64 | 0x00000000 | Primary virtual slot 23: null |
| 0x003D28DC | 0x68 | 0xFFFFFFFC | Effect offset-to-top -4 |
| 0x003D28E0 | 0x6C | 0x00000000 | Effect null header word |
| 0x003D28E4 | 0x70 | 0x003765A8 | Effect address point: _ZThn4_NK2al9LiveActor15getEffectKeeperEv |
| 0x003D28E8 | 0x74 | 0xFFFFFFF8 | Audio offset-to-top -8 |
| 0x003D28EC | 0x78 | 0x00000000 | Audio null header word |
| 0x003D28F0 | 0x7C | 0x00000000 | Audio address point, null slot |
| 0x003D28F4 | 0x80 | 0x00000000 | Audio null slot |
| 0x003D28F8 | 0x84 | 0x00376A44 | Audio getter slot: _ZThn8_NK2al9LiveActor14getAudioKeeperEv |
| 0x003D28FC | 0x88 | 0xFFFFFFF4 | Stage offset-to-top -12 |
| 0x003D2900 | 0x8C | 0x00000000 | Stage null header word |
| 0x003D2904 | 0x90 | 0x00375F28 | Stage address point: _ZThn12_NK2al9LiveActor20getStageSwitchKeeperEv |
| 0x003D2908 | 0x94 | 0x00375E4C | Stage initializer slot: _ZThn12_N2al9LiveActor21initStageSwitchKeeperEv |

Outer witnesses close those neighbors. Plant's factory entry 003B9F10 selects creator 0039811C, which allocates 0x80 and calls constructor 002F92C4; its independently stored address point is 003D26B4. Burner registry 003B9D78 selects creator 003981FC, which allocates 0x84 and calls constructor 00304368; its independently stored address point is 003D2914. The two words immediately before the TreeA, Bubble, Bunbun and Burner primary points are zero. No public class or table symbol is proposed for these neighbors.

## Minimal equal-union decision

The two existing anonymous rows are 003D274C..003D27E4 and 003D27E4..003D287C. Each is U/dc. The first wrongly owns Bubble's 8-byte prefix; the second wrongly owns Bunbun's 8-byte prefix. Replace those two rows with exactly:

| Start | End | Size | Rank/type | Symbol |
|---|---|---|---|---|
| 003D274C | 003D27DC | 144 | U/dc | anonymous predecessor tail |
| 003D27DC | 003D2874 | 152 | U/dc | `_ZTV6Bubble` |
| 003D2874 | 003D287C | 8 | U/dc | anonymous next-table header |

The union is unchanged: 003D274C..003D287C, 304 bytes, with no gap or overlap. The independently proven complete neighboring tables remain descriptive evidence; this minimal patch does not rename or wholly repartition them. It changes no function row, function boundary, pool, compiler flag, rank or accepted total. The next existing anonymous row 003D287C..003D2914 remains unchanged.

Keep all three proposed rows U/dc. Naming the genuine table supports address resolution for source-generated constructor stores, but adds no matched function or data credit. No source array, copied original table bytes, modified object or original data provider is proposed. A normal committed-source project/check grade and all prior-definition preservation still precede any constructor acceptance.

## Inputs and limits

The original executable is 3096576 bytes, SHA256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Metadata is frozen from ownership commit b00d011; Bubble source ownership uses 88d6105. Full original constructor/creator/thunk disassemblies, all three complete table records and the local raw snapshot remain ignored under build/bubble_table_review_88d6105. This report records bounded table/identity evidence, not full class recovery, functional source correctness or a byte-exact claim.
