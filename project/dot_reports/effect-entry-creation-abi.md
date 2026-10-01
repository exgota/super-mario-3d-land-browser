# 0023F494 independent ABI review

2026-10-01. Analysis only. No C++ compile, link, check, production edit, map edit, or publication was performed. No new match or functional-equivalence result is claimed.

## Result

No justified new source probe found beyond the packet's eight exhausted forms. The binary supports useful ABI corrections and an independently corroborated context layout. It does not establish the identity of a separate position overload or wrapper whose inlining could explain the retail scheduling.

The coordinator confirmed latest-main commit `48c97b5157698b6722a6d96330ad718f5351a854` still has the unnamed U row `0x0023F494..0x0023F538`. I fetched the packet and current DOT_BRIEF with the GitHub connector. The map content endpoint returned empty content for this large file; the coordinator separately used its blob to confirm the current row. The local baseline has the same U row. The supplied local binary SHA256 is `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`.

## Independently observed argument flow

The consumer `0x002E54AC` saves 52 bytes of integer registers, 8 bytes of VFP registers, and reserves 52 bytes, so its current SP is incoming SP minus 0x70. This allows its stack-argument loads to be interpreted without relying on the proposed source:

- Incoming r0: creation context; retained in r4.
- Incoming r1: writable reference, retained in r7. The allocated object is stored through it at 0x002E55AC; an identifier is stored at reference+4 at 0x002E56A4.
- Incoming r2: matrix input. It is copied by genuine 0x0027C18C to object+0xFC and object+0x12C at 0x002E55C4/0x002E55D0.
- Incoming r3: resource-record selector, recovered from saved r3 at [sp+0x48]. It is multiplied by 40 and combined with resource-table fields at 0x002E54D0..0x002E5504.
- Incoming stack+0: resource-bank selector, recovered at [sp+0x70]. At 0x002E54E8..0x002E54F0 it selects a pointer from context+4. The helper passes zero.
- Incoming stack+4: per-context list/group selector, recovered at [sp+0x74]. At 0x002E56C4..0x002E56D0 it produces context + selector*4; 0x002E579C onward accesses a linked-list head at that result+0x1C. At 0x002E58DC..0x002E58E0 the same selector is stored in the created member object+8. The helper masks its incoming fourth argument to eight bits before passing it. Calling this value a group/list ID is better grounded than calling it flags.
- Incoming stack+8: bit-selection mask, loaded into sl at 0x002E54D8. At 0x002E56D4 it is tested against 1 shifted by the emitter/member index. The helper passes all bits set, not a signed resource sentinel in this slot.

The callee returns zero at failure exits 0x002E551C and 0x002E5590, and one at the success exit 0x002E5978. Thus its result is boolean-valued, rather than an object pointer. The binary does not distinguish an original bool declaration from an int/word declaration returning only 0/1. The helper ignores the return value, so replacing the packet's unused void-pointer return declaration with a boolean/word declaration is a semantic cleanup, not evidence that it will change this caller's machine code. Packet form 6 already tested an unconsumed void return.

The argument names above describe observed roles, not recovered original NintendoWare or game symbols. Exact signedness and source spellings remain unknown.

## Independent temporary-matrix evidence

Mapped context constructor 0x002E5C94 independently initializes context+0x87C at 0x002E618C..0x002E6198 by passing that address to genuine 0x0027C18C. The latter is exactly a 12-float load followed by a 12-float store and return. This corroborates a persistent 48-byte matrix object at the helper's borrowed scratch-matrix location.

The source pointer is loaded from the literal at 0x002E620C. It points beyond the supplied executable image into runtime data, so this review does not claim the initial matrix contents are identity or otherwise reconstruct its initializer. It establishes initialization by a 48-byte matrix copy only.

## Inlining and wrapper assessment

A full aligned ARM branch-word scan of the supplied image finds only three direct branches to 0x0023F494, all inside known caller 0x0023F02C (0x0023F080, 0x0023F09C, 0x0023F0DC), and only one direct branch to 0x002E54AC, inside this helper at 0x0023F52C. An exact little-endian address-constant scan finds no address-taken reference to either function in the supplied image. These negative observations do not rule out source-only inline functions or dynamically constructed function pointers; they do rule out claiming a second independently visible direct callsite here.

The null-matrix branch could plausibly have come from a source-level position overload that updates context+0x87C and then invokes the matrix creation routine. However, the only observed translation-write sequence belongs to the target itself. No second genuine callsite, standalone body, or independent inlined instance establishes this hypothetical overload. Assigning it an address or treating it as a recovered import would be unjustified. Packet form 7 already exhausted an invented separate five-argument forwarding helper and observed that its standalone body remained allocated.

Likewise, the entry constructor independently establishes an object layout but does not tell whether this target was originally a member function or a free function with a typed pointer. Recasting the same layout as a member and hoping ARMCC schedules it differently is currently a cosmetic experiment, not a new evidence-led ABI hypothesis. A raw-word group ID narrowed to a byte is already reflected by packet forms 1/2; declaring the incoming parameter as a byte is contradicted by the retail masking and was already rejected by form 4.

The corrected consumer roles and context-constructor observation should be retained for future effect-system reconstruction. A new capped probe should wait for an independently established callable/source boundary, concrete alias/type evidence, or another genuine inlined implementation pattern that explains why stack constants and owner/reference loads remain branch-local. No additional compile is recommended from this review alone.

## Sources

- Owner's supplied, SHA-verified EU executable, inspected locally on 2026-10-01. Never copied into this report or uploaded.
- [Packet 0023F494](https://github.com/exgota/super-mario-3d-land-browser/blob/48c97b5157698b6722a6d96330ad718f5351a854/project/pro_requests/0023F494.md), fetched 2026-10-01.
- [Current DOT_BRIEF](https://github.com/exgota/super-mario-3d-land-browser/blob/main/project/DOT_BRIEF.md), fetched 2026-10-01.
- Local repository AGENTS.md, complete project/BRIEF.md, Guide.md, STATE.md, newest daily report, and baseline map read before analysis.
