# BombHei control: bounded reconstruction

## Result

`dot/bomb-hei-control` starts at main `3de056e0dcb33619c32f8b46ef64f74c90d28934`. It adds the ordinary C++ body `_ZN7BombHei7controlEv` and refines only its class header. The body stays under `NON_MATCHING`. It is **not byte-exact**, adds **zero exact-function credit**, and is not an in-game equivalence claim.

The final 704-byte canonical ARMCC 4.1/791 interval differs in 49 bytes. The first differing byte is `0x0030E6A9`; the last is `0x0030E706`. All differences lie in the quaternion arithmetic. The other 556 bytes outside the packet's 148-byte quaternion region, including its complete 96-byte literal pool, remain equal. The last 52 bytes inside that region now also equal retail. Agreeing partial intervals add no exact credit.

`python tools/check.py _ZN7BombHei7controlEv --object build/eu/obj/Game/backup/src/Enemy/BombHei.o` returns exit 1 and exactly:

```text
U -> m: The linked candidate differs from the unchanged original interval.
```

The target's existing anonymous U row is temporarily given its independently proven method name for this check. The entire map is then restored byte-for-byte. No map, rank, ledger, STATE, tool, compiler flag, or binary change belongs to this branch.

## Bounded attempts

The incoming packet records four meaningful forms and eight paired physical compiles. This pass uses the remaining four-form budget, all under the unchanged canonical 4.1/791 build. No 4.1/894 comparison was run here.

| Cumulative form | Source commit | New expression/context hypothesis | Size | Result |
| --- | --- | --- | ---: | --- |
| 5 | `7faf41e` | A separate inline quaternion-vector product returns through the existing `Quatf` constructor | 704 | 49 different bytes |
| 6 | `7bebeaf` | Scalar product lifetimes cross that constructor return boundary | 704 | 80 different bytes |
| 7 | `ed9a638` | Three staged scalar accumulations express the Hamilton product before construction | 704 | 60 different bytes |
| 8 | `d81f1c1` | A vector cross-product record is combined with the scalar quaternion component | 708 | Full-size rejection; no byte-distance claim |

All four forms compiled and linked/exported successfully. Form 8's checker returns exit 1 with `U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.` Forms 5–7 return exit 1 with the mismatch text above. Commit `02858fe` restores form 5, and its recompilation is a replay, not a ninth form. No further source hypotheses were tried after the eight-form cap.

The first build began with no build output, compiled 45 Game, 128 al, and one SDK source, and linked/exported. The final restored tree also passed `make.py eu -ca`, followed by the committed-source check. The final checker verifies 20 source/config/header inputs for this object. The build uses the repository's commands and configured flags; no rescue flags or handcrafted objects are used.

## Floating semantics and aliasing

The original threshold at `0x0030E8A8` loads `_88`, moves its raw bits to an integer register, and performs signed `CMP` against `0x40600000`. Signed-less values store positive zero; other values multiply by the float whose bits are `0x3F75C28F` (0.96f). Every sign-bit-set word, including negative signaling and quiet NaNs, therefore takes the zero path. Positive signaling NaNs instead enter the multiply and can set the invalid flag. This is not an ordinary IEEE less-than comparison on all bit patterns.

The proposed source makes the signed-word policy explicit with a local float/int union, using the established ARMCC GNU-mode union interpretation. The packet's plain float comparison already compiled to the same retail tail under fast ARMCC; this change makes the binary policy explicit for future native work rather than fixing a demonstrated old canonical-tail mismatch. Any future compiler port must preserve the word interpretation and target floating environment deliberately.

The quaternion product keeps the exact scalar grouping `q * (v, 0) * conjugate(q)` without assuming unit length. Original output stores at `0x0030E704` and `0x0030E720` are followed by fresh quaternion loads. The helper retains reference-based aliasing and those reloads; it does not snapshot the rotation across output writes. The constructor-return expression changes register allocation and scheduling while preserving the observed arithmetic grouping.

## Replay scope and results

The complete runnable recipe is [bomb-hei-control-replay.md](bomb-hei-control-replay.md). It executes the unchanged retail bytes and the unchanged checker-linked canonical candidate under Unicorn 2.1.4. No replay modifies any oracle file, object, checker, or source input.

- 4,608 cases execute only the quaternion region `0x0030E6A8..0x0030E73C`. Cases cover nine output-pointer offsets relative to the rotation (`-12,-8,-4,0,4,8,12,16,64`), random raw words, signed zeros, subnormals, infinities, and signaling/quiet NaNs. Memory outside the 12-byte output is checked unchanged
- 8,292 cases execute only the threshold region `0x0030E8A8..0x0030E8C8`. Explicit threshold neighbors and random words check the signed comparison, NaN handling, outputs, and FPSCR. Negative-NaN examples `FFC00000`, `FF800001`, and `FFFFFFFF` all produce positive zero
- Both arithmetic suites run FPSCR modes `0`, `0x01000000`, `0x02000000`, and `0x03000000`, covering the flush-to-zero/default-NaN control combinations. They compare output memory and the complete FPSCR; there are zero mismatches
- 1,792 additional cases execute the complete 608-byte instruction body plus its original/candidate pools. **All nine external call targets are deterministic models**, including the collision gate, quaternion provider, front-pointer getter, audio/action calls, vector-float helper, nerve query/set, and final helper. No real callee runs in this suite
- Whole-root inputs cross gate true/false, fuse true/false, `_70` values `-1,0,1,2`, `_74` values `-1,0,1,2,89,90,91`, all seven relevant nerve addresses plus an unrelated state, and FPSCR 0/default-NaN-plus-flush modes. Actor/front memory, ordered call traces, modeled nerve changes, and FPSCR agree in all 1,792 cases. The assertions independently check decrement, fuse expiry, and the early set-nerve return. All 152 instruction addresses are visited across the corpus; that address coverage is not a claim of exhaustive path coverage
- Every arithmetic and whole-root case preserves SP, r4–r11, and s16–s31. Whole-root runs also check stack guards outside the expected 32-byte frame

There are 14,692 bounded comparisons and zero observed mismatches. The arithmetic suites use real instructions without external-call models; the whole-root suite validates only the stated callee models. Actual callee side effects, full lifecycle/gameplay, all possible timer inputs, and a native compiler's floating semantics remain unverified. In particular, the timer corpus does not establish behavior for signed decrement overflow.

## Independently checked identities and preservation

The retail factory pair at `0x003B9A18/1C` contains the complete BombHei name pointer `0x003E00C8` and creator `0x0039850C`. The creator requests `0x8C` bytes and calls constructor `0x0025C140`. Its vtable pointer literal is `0x003D3278`; the word at `0x003D32C4` is `0x0030E678`, consistent with the existing LiveActor control slot 19. These facts establish the method without deriving an import address from candidate relocations.

Independent constructor instructions establish counters 70/74, the 180 word at 78, floats at 7C/80, byte 84, and the float zero written by VSTR at 88. The header's old pointer type at 88 becomes float and sizeof remains 0x8C. Bytes 60..6F stay opaque, including the supplementary dispatch pointer. This does not reconstruct the constructor or supplementary interface.

Original `0x00337474` follows actor collider offset 20, then collider+38, and forwards the caller's output pointer to `0x00331F68`. That helper obtains two 16-byte values from offsets D4/44 and forwards them with the destination to `0x0026A958`; no invented quaternion-provider method name is used. Original `getFrontPtr` at `0x0027C04C` obtains pose offset 14 and dispatches through virtual offset 24. Audio helper `0x0024E9F8` uses its interface receiver and passes the SafeString/integer onward; the root supplies actor+8. All imports retain existing named or anonymous map identities. No standalone helper address was fabricated, and the new inline product helper leaves no emitted standalone function.

The new object emits zero existing O function definitions. Its other emitted definitions are preexisting header-only unaccepted interface/string methods. `rg` finds no old source consumer of `BombHei.h`; only the new translation unit includes it. No accepted source or shared header changes, and no prior accepted-root recheck is claimed. The local full map is restored with SHA256 `514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa`.

## Frozen identities

- Original code SHA256: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Final C++ SHA256: `916533c6d9bfd7046f3eb417d5d795555c41b58837df7c5ac639641ac4935ff3`
- Header SHA256: `1f1185d8a879d64b18e265154dd24c469aa7659b6f2a4e5cec12a4245b4d7fc1`
- Canonical object SHA256: `05d4c6e8b98ec37b133005f8c249daecf2414b49e6e0e1392eec4fe964b1060c`
- Original-address candidate interval SHA256: `1adfba5f10cc022d50346ff99a4d5acfe8844795110d0d854f56985b2ed634d1`

The next matching pass needs a new grounded expression/compiler-context hypothesis for the first 96 bytes of quaternion arithmetic. Do not repeat the exhausted scalar/record orders, assume IEEE comparison semantics for the threshold, or remove the required alias reloads merely to shorten code.
