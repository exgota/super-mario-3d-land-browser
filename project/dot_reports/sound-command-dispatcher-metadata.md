# Sound command dispatcher: independent ABI and metadata evidence

This review uses frozen base `754f99a30a337756df5aa01c28b4ed6a977fecd5`, the owner's verified EU executable and exheader. Executable SHA256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. No removed library, leaked SDK, external source tree or derived declarations were used.

Independent caller `002C1170..002C11BC` dequeues a work item through `002298E4`, passes it to `002BECC8`, executes a memory barrier, and passes the item to `00392938`. The dispatcher reads next at +0 and opcode byte at +4; the common header spans 16 bytes. Payloads vary in size. Opcode42 reads through +0x1CD. No exact original class name or complete command hierarchy is claimed.

The 59-word inline switch table starts at002BECE8 and ends002BEDD4. Cases0,1,41 and opcodes>=59 advance to next. Cases57/58 share a final task-enqueue arm. Executable ranges are [002BECC8,002BECE8), [002BEDD4,002BF110), and [002BF114,002BF564), totaling491 instructions. Word002BF110 is a float-zero literal. The final16-byte pool [002BF564,002BF574) is also data. The complete unchanged map interval remains2220 bytes; no table or pool bytes are dropped from the matching requirement.

## Existing guard and absent BSS owner

Guard003F38B8 already has an unchanged four-byte U,d map row [003F38B8,003F38BC). Its original word is zero. `0028A998` independently reads a full word, sets it to1 only when initially zero, and returns whether it changed it. The dispatcher first tests bit0. Values2/4 therefore call the helper but skip construction, a distinction retained in the source and replay.

`00430DB0` has no map row. The dispatcher loads this address from002BF568; fourteen other literal references independently identify the same address, each paired with guard003F38B8. Constructor002585C4 initializes a final two-element array of eight-byte objects at+0x290 and writes through+0x29C. The proven initialized extent is at least[00430DB0,00431050), or0x2A0 bytes.

The exheader places RW at[003E1000,003F39D4) and BSS at[003F39D4,00431058). Startup literal00100044 independently contains00431058. There are eight further BSS bytes after00431050. No exact next-owner boundary or original sizeof has been established; those eight bytes are not silently assigned to this object. This evidence establishes an address and lower bound, not an accepted canonical data definition or partition.

The source uses `extern unsigned dat_003F38B8` and an incomplete `extern unsigned char dat_00430DB0[]`. It defines neither. The unchanged checker refuses the latter's nonbranch relocation. The separate replay link supplies its independently grounded original address without altering map.csv, checker logic, compiler object, target data, Type, or boundaries. That link provides no exact credit.

## Import ABI review

At frozen754, none of the48 direct fn_* identities has a pre-existing source/header declaration. Final sibling SoundArchiveStart.cpp at25b50ee2 also shares none of these identities. This review does not establish compatibility with unseen future declarations.

The following are neutral machine-compatible signatures. P=void*, CP=const void*, PP=const void* const*, U=unsigned word, I=signed word, F=float, B=unsigned char, S=signed char. Narrow types reflect explicit extensions at dispatch sites. Void preserves ignored-result behavior where original source return types are unproven; these are not original SDK API claims.

| Import | Signature |
| --- | --- |
| 0022A614 | void(P,P,U) |
| 0022A66C | void(P,U) |
| 0022A6CC | void(P,P) |
| 0022A6D4 | void(P,P) |
| 0022A6E4 | P() |
| 0022B410 | P() |
| 0022B488 | P() |
| 002585C4 | void(P), constructor result ignored |
| 0028A998 | I(U*) |
| 002C2A84 | void(P,U,short) |
| 002C34E8 | void(P,B,U) |
| 002C3554 | bool(P,B,P) |
| 002C35B8 | bool(P,B,P) |
| 002C361C | bool(P,B,P) |
| 002C410C | void(P,P) |
| 002C41A4 | bool(P,P,U,B,U,P,U,S) |
| 002C42FC | void(P,U,F) |
| 002C4308 | void(P,B,F) |
| 002C44E0 | bool(P,CP,CP,CP,CP,U,U,U,unsigned short) |
| 002C4888 | void(P,U,F) |
| 002C4BA0 | bool(P,U,U,U,S,S) |
| 002C4E28 | void(P,U,F) |
| 002C5348 | void(P,U,F) |
| 002C5850 | I(P,P,U,unsigned short) |
| 002C5C64 | bool(P,B,U,P,U,P,U) |
| 002C6224 | void(P,U,F) |
| 002C62B4 | void(P,U,B) |
| 002C6330 | void(P,U,F) |
| 002C6348 | P(P,I) |
| 002C635C | void(P,U,F) |
| 002C6378 | void(P,U,F) |
| 002C6390 | void(P,U,S,U) |
| 002C6414 | void(P,U,short) |
| 002C6420 | void(P,U,F) |
| 002C6438 | void(P,U,F) |
| 002C6450 | void(P,U,F) |
| 002C6468 | void(U,short) |
| 002C647C | bool(P,U,U) |
| 002C6510 | void(P,U,S) |
| 002C6590 | void(P,U,F) |
| 002C65A8 | void(P,U,U,F) |
| 002C6638 | void(P,U,F) |
| 002C6674 | void(P,P) |
| 002C6684 | void(P,Callback,P) |
| 002C6700 | void(P,B,I) |
| 002C6788 | void(P,P,U,P) |
| 002C68AC | void(P,CP,U,PP,I,S) |
| 002C6BE0 | void(P,CP,U) |

Callback is represented as void(*)(U,SequenceCallbackArgs*,P). In002C6690..002C66F8, it receives incoming r1 as its first word, a16-byte stack structure as its second argument, and object+0x84 as context. The structure contains short pointers at0/4/8 and a mutable byte at12; its pointers are object+0xC8, global00425C28, and track+0xA0 (the last calculated by00228C94). Original first-argument signedness or narrower typedef is unknown.

Important independent producer/consumer checks:

- 002C6788 arg4 is stored at+0x78, then dereferenced through vtable+8 at002C688C..002C68A8. Its r0=1 success residue does not establish a boolean source return type
- 002C5C64's buffer/size/archive/file arguments come from producer002BE80C: this+0x20EC,0x200,archive,file ID. The dispatcher reorders these into arg4..arg7
- 002C41A4 arg6 is a provider pointer flowing through object+0x60 to the virtual call002C3D44..002C3D54; arg7 is a file ID. Producer chain002BC8C0→0022AFEC establishes this distinction
- 002C6674/002C410C arg2 are subobject pointers constructed at002BFBE8 and0022B0A4
- 002C6BE0 takes range start and byte length. Producer002BDF9C calculates end−start, while002C6BF4 reconstructs end
- 002C44E0 reads56 bytes through arg2,5-byte records through arg3,38-byte records through arg4, and6-byte records through arg5. Args7/8 are unused words in its body; arg9 is the zero-extended comparison token
- 002C4BA0 args5/6 are sign-extended bytes. 002C68AC copies four pointer words from packet+0x1C. 002C5850 returns0/1/2, not a boolean

Getter0022A6E4 returns0042A4D0 (literal0022A740), initializing at least0x0C bytes. Getter0022B410 returns00425FF0 (literal0022B47C), initializing at least0x1C bytes. Getter0022B488 returns00425E50 (literal0022B520), initializing at least0x6C bytes. These are initialized lower bounds, not full object-size claims. The final source sequences every getter before reading command arguments, matching the original and supporting aliasing.
