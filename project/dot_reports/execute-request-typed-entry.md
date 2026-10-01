# ExecuteRequestKeeper request: typed-entry proposal

Prepared 2026-10-01 after reading the complete `project/pro_requests/00252B1C.md` packet. This is one evidence-led source proposal, not a fresh eight-candidate search. At the initial proposal freeze, no compilation, functional test or canonical check had been performed. The measured enrollment result is recorded below. The parent owns source enrollment, commits, builds, checking and publication.

## Scope and unchanged ABI

The proposed symbol is `_ZN2al20ExecuteRequestKeeper7requestEPNS_9LiveActorEi`, with unchanged retail interval `[0x00252B1C, 0x00252BF8)`, 220 bytes. The existing declaration `request(LiveActor*, int)` and constructor declaration `ExecuteRequestKeeper(size_t)` remain unchanged. The opaque 16-byte member becomes four queue pointers. No base class, virtual member or constructor implementation is added.

The sole new source is `lib/al/src/Execute/alExecuteRequestKeeper.cpp`; the sole changed header is `lib/al/include/Execute/alExecuteRequestKeeper.h`. No existing helper body or shared container header is replaced. The queue and entry names, and their helper names, are descriptive reconstruction names, not recovered original identities. Their compile-time extent checks require the target ABI: entry 4 bytes, queue 12 bytes, keeper 16 bytes.

## Independent evidence that justifies this probe

The owner's executable was rechecked as SHA256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`; it remains local and ignored. The request interval hash is `42eece83f6ccf438a7184bb66bc72a3d9953dd671a31c3fe43a6fbd260f72e0f`.

The independently named `ExecuteDirector::init` supplies a constructor link outside the capped request function:

```text
001CCC20: mov   r0, #0x10
001CCC24: bl    #0x2932b0
001CCC28: cmp   r0, #0
001CCC2C: ldrne r1, [r4]
001CCC30: blne  #0x1ddac8
001CCC34: str   r0, [r4, #0x10]
```

The existing source describes this final operation as allocating `ExecuteRequestKeeper(_0)` and storing it in the director's keeper member. The callee at `0x001DDAC8` loops four times. Each iteration allocates 12 bytes for a queue, writes the supplied capacity at offset 0 and zero count at offset 4, allocates capacity times four bytes, and writes that buffer at offset 8. This independently confirms the request's four-queue layout and proves the previously inferred capacity field for this constructor path.

Crucially, that callee constructs its allocated elements through a callback:

```text
001DDB14: ldr r1, [pc, #0x24]   ; pool word at 0x001DDB40 = 0x001D8D74
001DDB18: mov r3, r7           ; capacity
001DDB1C: mov r2, #4           ; element stride
001DDB20: bl  #0x28eabc
```

The callback at `[0x001D8D74, 0x001D8D80)` writes zero to `[r0]` and returns. The independently decoded helper at `[0x0028EABC, 0x0028EAFC)` invokes that callback once per element, stepping by the supplied stride. This supports a constructed, four-byte pointer-wrapper entry. It does not exclude compiler-generated scalar-pointer value-initialization. The wrapper is therefore an explicit, testable hypothesis rather than an asserted original class.

Evidence interval SHA256 values, all read directly from the same local executable:

- Constructor and literal pool `[0x001DDAC8, 0x001DDB44)`: `9797f8d2e13117c2391d244c53af1391502c0ddc336dfffb0d5d0a21bb956bff`
- Element callback `[0x001D8D74, 0x001D8D80)`: `4e04ee5e8b0262b528762ebe6bb10dc4fc3c9037cac69eff72906b7667e6ea14`
- Array-construction helper `[0x0028EABC, 0x0028EAFC)`: `290002253638463182bd3ec5fe8636aa6c7726aa0eacf9009e2a14bf7ba6639e`

No symbol aliases or mapped bodies are added for those evidence functions.

## One new type/API hypothesis

The eight historical candidates store raw `LiveActor**`. This proposal stores an array of four-byte entries, each containing one actor pointer. Index access returns an entry address. Erase uses ordinary entry copy-assignment. Append allocates a slot by incrementing the count and returning an entry address; the caller then assigns that entry's actor field.

This is a concrete change to element type and the append/access abstraction. The constructor's element-construction callback motivates the entry type. The request target's early slot-address materialization and count-store-before-buffer-load append motivate the slot-returning API. The opposing selection, flag/break membership loop and erase control flow otherwise retain historical form 5, so they are not arbitrary new control-flow variants.

The source expresses the observed scan-after-swap behavior: it increments the index after removal and does not recheck the replacement. It preserves the conditional upper-bound check before replacement, unconditional count decrement on a matched entry, absence of a capacity check, absence of request-type validation, duplicate handling, and absence of final-slot clearing. Invalid request types still leave the opposing pointer null. No safety behavior is invented.

The typed helpers may change inlining and address lifetimes. They do not force instruction order or registers. In particular, this review does not prove why the retail selected queue lives in r5, nor that ARMCC will preserve the target's reload schedule. Aliased or malformed queues, arithmetic overflow and runtime equivalence remain unverified. This proposal must succeed or fail on its measured output.

## Separate history: all eight prior structural forms

These are copied as historical outcomes from the original packet. The two columns are emitted size / differing-byte metric under ARMCC 4.1 builds 791 and 894. They are not results of this proposal. The metric is equal-offset byte differences plus absolute length difference, not the canonical acceptance result.

| Historical form | Essential change | 791 | 894 |
| --- | --- | --- | --- |
| 1 | Direct fields, early-return membership, postincrement subscript append | 288 / 157 | 288 / 157 |
| 2 | Explicit erase slot, inline find returning index or -1, increment-first append | 312 / 233 | 312 / 233 |
| 3 | Decremented last-index local, while-membership test followed by index-equals-size append | 216 / 117 | 216 / 117 |
| 4 | Inline erase with lower and upper bounds, flag/break contains, increment-first append | 220 / 115 | 220 / 127 |
| 5 | Drop lower bound, compute erase element before upper-bound test | 220 / 74 | 216 / 130 |
| 6 | Erase returns decremented size; outer loop carries/reloads mutable count | 216 / 120 | 216 / 120 |
| 7 | Move selected-queue load; erase receives slot and returns void; size-greater-than-index loop | 220 / 130 | 220 / 130 |
| 8 | Form 7 with separate helper source/header and source-generated two-object inline link | Link failed: 312 > 220 | Link failed: 312 > 220 |

All eight complete-interval match results were false. Form 8 produced no valid linked comparison image. Their original sources, exact flags, differences and caveats remain in `project/pro_requests/00252B1C.md`. This proposal does not erase, renumber or repeat that historical budget.

## Provenance and container-name limit

The permitted open-ead/sead README was checked first: <https://github.com/open-ead/sead#readme>. It describes reconstruction from recent games and warns that some template names are inferred. Only API shape was consulted afterward. Its Buffer interface provides element-reference and element-address access: <https://raw.githubusercontent.com/open-ead/sead/master/include/container/seadBuffer.h>. None of that implementation was imported, and it supplies neither a 3DS implementation nor an original name for this queue.

Retail evidence also warns against adopting the present clean `sead::PtrArray` spelling as this queue's identity. Named `sead::PtrArrayImpl::setBuffer` at `0x0027A81C` stores count at offset 0 and capacity at offset 4. Named `PtrArrayImpl::erase` at `0x00266FF8` uses count at offset 0. These differ from the request queue's independently established capacity/count order. No shared PtrArray declaration was changed as part of this proposal.

## Enrollment and verification

This staged proposal was prepared outside the live repository inputs while the parent owned an ongoing source-frozen build. It contains the header, request source and this report at their intended repository-relative paths. The parent must enroll and commit the chosen source, run the normal project build with unchanged settings, then check the entire mapped interval through the canonical gate. The measured enrollment result follows separately below.


## Measured canonical outcome: closer, not exact

The parent enrolled the header and source as commit `f41cb6229a64e3238c83b469a951444e3dd9f129`, adding the required `NON_MATCHING` guard around the request definition. The parent reports that the normal full project build linked and exported. Its recorded canonical object provenance confirms the project build step, ARMCC 4.1/791, stable committed inputs and the unchanged configured compilation settings.

The initial canonical check required the parent to register the proposed symbol on its existing local map row. After that, `/tmp/mario-main21b-request-check1-named.log` records:

```text
U -> m: The linked candidate differs from the unchanged original interval.
```

The current local row is rank `m`. This review did not edit the map, rerun the rank-changing checker or compile another variant. The isolated exact-check artifacts are at `build/exact_checks/eu/function_00252B1C_cyvd8bob/`. Their linker map gives the exact symbol at `0x00252B1C`, size 220, with a 220-byte `CANDIDATE_CODE` region. Reading that region confirms it is byte-for-byte identical to the canonical object's 220-byte request section. That section has no relocations.

Measured comparison to the complete unchanged target: **220 bytes emitted; 54 differing bytes; non-exact**. The metric is the same equal-offset byte differences plus length difference used in the historical packet; the length difference is zero. Historical form 5 under 791 emitted 220 bytes with 74 differing bytes. This evidence-backed type/API probe therefore improved that diagnostic by 20 bytes without satisfying the canonical match criterion.

The two structural predictions changed as intended:

- The opposing loop now computes the entry address before loading and comparing its actor, at the same instruction positions as retail. Historical form 5 computed the erase slot only after the comparison.
- Append now stores the incremented count before loading the buffer. Its final three instructions (`mvn`, `add`, and the actor `str`) match retail exactly, as do the subsequent `pop` and `bx` return instructions. Historical form 5 loaded the buffer before storing the count.

The remaining differences are substantive:

- The selected queue still lives in `ip`; retail uses `r5`. The opposing buffer uses `r5` instead of retail's `ip`.
- Retail conditionally reloads count on the nonmatch path before its branch. The candidate reloads count at the shared loop join, including an extra reload after decrement on the match path.
- The membership loop still uses different registers for count, buffer and loaded actor.

The result supports the usefulness of the constructed-entry and slot-returning abstraction as a code-generation hypothesis. It does not prove original class identity or uniquely attribute the improvement between the two changes, which were deliberately tested together as one proposal. No further source permutation is proposed on this evidence alone. Functional equivalence remains unverified.

Canonical provenance and hashes:

- Source: `7c851717093834d25e39a4164ce5955d90dc793898529ee6f78ba815e073a6b4`
- Header: `c7728443de39217216d7ccfc1c8dd8498a13beedf6cedfbccf9f67dfb02ff67f`
- Full canonical object: `4ed349623a165aa363dd73adb2028027f016a2dc037e45367e319c24bf163220`
- Request section and linked complete interval: `7b84c0488071ebfa9f49aa75e0793e29b267effdda48f20d41c62471ae445f7c`
- Historical form 5/791 section: `199e7a33edf3f897a609a917b5a4be175b3b1da000bee55f8df4f6e2aa63c3f9`

The normal read-only command was attempted after sourcing the environment:

```sh
python tools/diff.py --no_check _ZN2al20ExecuteRequestKeeper7requestEPNS_9LiveActorEi
```

It reported `Couldn't find in decomp` for this symbol. The diagnostic below therefore reads the canonical relocation-free object section and the target directly with Capstone; it is not a successful normal diff or an accepted exact check. The separately linked canonical checker candidate was also verified above.

### Complete read-only fixed-offset diagnostic

`=` means all four instruction bytes match. `!` means they differ.

```text
address  =/! retail                                  | typed-entry proposal, 791
00252B1C = push     {r4, r5}                    | push     {r4, r5}
00252B20 = cmp      r2, #0                      | cmp      r2, #0
00252B24 = mov      r3, #0                      | mov      r3, #0
00252B28 ! ldr      r5, [r0, r2, lsl #2]        | ldr      ip, [r0, r2, lsl #2]
00252B2C = ldreq    r3, [r0, #4]                | ldreq    r3, [r0, #4]
00252B30 = beq      #0x252b54                   | beq      #0x252b54
00252B34 = cmp      r2, #1                      | cmp      r2, #1
00252B38 = ldreq    r3, [r0]                    | ldreq    r3, [r0]
00252B3C = beq      #0x252b54                   | beq      #0x252b54
00252B40 = cmp      r2, #2                      | cmp      r2, #2
00252B44 = ldreq    r3, [r0, #0xc]              | ldreq    r3, [r0, #0xc]
00252B48 = beq      #0x252b54                   | beq      #0x252b54
00252B4C = cmp      r2, #3                      | cmp      r2, #3
00252B50 = ldreq    r3, [r0, #8]                | ldreq    r3, [r0, #8]
00252B54 = ldr      r0, [r3, #4]                | ldr      r0, [r3, #4]
00252B58 = cmp      r0, #0                      | cmp      r0, #0
00252B5C = mov      r0, #0                      | mov      r0, #0
00252B60 = ble      #0x252bac                   | ble      #0x252bac
00252B64 ! ldr      ip, [r3, #8]                | ldr      r5, [r3, #8]
00252B68 ! add      r2, ip, r0, lsl #2          | add      r2, r5, r0, lsl #2
00252B6C = ldr      r4, [r2]                    | ldr      r4, [r2]
00252B70 = cmp      r4, r1                      | cmp      r4, r1
00252B74 ! ldrne    r2, [r3, #4]                | bne      #0x252b9c
00252B78 ! bne      #0x252ba0                   | ldr      r4, [r3, #4]
00252B7C ! ldr      r4, [r3, #4]                | cmp      r4, r0
00252B80 ! cmp      r4, r0                      | ble      #0x252b90
00252B84 ! ble      #0x252b94                   | sub      r4, r4, #1
00252B88 ! sub      r4, r4, #1                  | ldr      r4, [r5, r4, lsl #2]
00252B8C ! ldr      ip, [ip, r4, lsl #2]        | str      r4, [r2]
00252B90 ! str      ip, [r2]                    | ldr      r2, [r3, #4]
00252B94 ! ldr      r2, [r3, #4]                | sub      r4, r2, #1
00252B98 ! sub      r2, r2, #1                  | str      r4, [r3, #4]
00252B9C ! str      r2, [r3, #4]                | ldr      r2, [r3, #4]
00252BA0 = add      r0, r0, #1                  | add      r0, r0, #1
00252BA4 = cmp      r2, r0                      | cmp      r2, r0
00252BA8 = bgt      #0x252b64                   | bgt      #0x252b64
00252BAC ! ldr      ip, [r5, #4]                | ldr      r2, [ip, #4]
00252BB0 = mov      r0, #0                      | mov      r0, #0
00252BB4 ! cmp      ip, #0                      | cmp      r2, #0
00252BB8 ! ldrgt    r3, [r5, #8]                | ldrgt    r4, [ip, #8]
00252BBC = ble      #0x252bd8                   | ble      #0x252bd8
00252BC0 ! ldr      r2, [r3, r0, lsl #2]        | ldr      r5, [r4, r0, lsl #2]
00252BC4 ! cmp      r2, r1                      | cmp      r5, r1
00252BC8 = beq      #0x252bf0                   | beq      #0x252bf0
00252BCC = add      r0, r0, #1                  | add      r0, r0, #1
00252BD0 ! cmp      ip, r0                      | cmp      r2, r0
00252BD4 = bgt      #0x252bc0                   | bgt      #0x252bc0
00252BD8 ! add      r0, ip, #1                  | add      r0, r2, #1
00252BDC ! str      r0, [r5, #4]                | str      r0, [ip, #4]
00252BE0 ! ldr      r3, [r5, #8]                | ldr      r3, [ip, #8]
00252BE4 = mvn      r2, #3                      | mvn      r2, #3
00252BE8 = add      r0, r2, r0, lsl #2          | add      r0, r2, r0, lsl #2
00252BEC = str      r1, [r3, r0]                | str      r1, [r3, r0]
00252BF0 = pop      {r4, r5}                    | pop      {r4, r5}
00252BF4 = bx       lr                          | bx       lr
```

32 of 55 instructions are byte-identical at their original offsets. No target boundary, compiler flag or comparison rule was changed.

## Bounded differential behavior check

A closed integer-ARM interpreter executed the unmodified220-byte retail body and the canonical relocation-free candidate over4,568 valid queue-state pairs (4,744 generated,176 excluded to avoid capacity overflow). All observable queue memory and preserved-register/stack checks agreed with each other and with an independent direct queue model. Tests cover all four request modes, null/non-null opaque actor identities, empty/full queues, membership and duplicate patterns, four distinct capacity8 queues, and at most149 instructions per body. They exclude invalid modes, overflow, malformed/aliased memory, concurrency and hardware faults. This is bounded behavioral evidence, not a proof or exact-match credit. The source remains guarded and canonical rank m.

Reproducible interpreter and exact hashes are in project/dot_reports/request-queue-differential.md.

## Latest packet-baseline verification

The retained source was applied to exact main8ca3fc0f2aa6c8fe0d870cf4992b9f6029286a13 and committed before the normal project build. Its checker/tool files are unchanged in the subsequently fetched af853bf3e12865dbbf1c2c467868fac318d0a7b8. All197 previously accepted functions passed before the proposal batch and again after the clean vector/matrix declarations, restored default vector assignment, published string header, and three guarded proposals were built. Evidence summary:197/197, no new exact credit from these three roots. The full compact build linked/exported after the ordinary provenance-dependent scaffold retry. Later main additions remain subject to main's own intake revalidation.

Latest source checkpoint e9df1e9: `python tools/check.py _ZN2al20ExecuteRequestKeeper7requestEPNS_9LiveActorEi --object build/eu/obj/lib/al/src/Execute/alExecuteRequestKeeper.o` returned `U -> m: The linked candidate differs from the unchanged original interval.` Main already names the existing target interval. No new map identity, relocation or boundary is needed.
