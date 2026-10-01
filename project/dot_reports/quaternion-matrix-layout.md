# Quaternion threshold and Matrix22 packet follow-up

Zero exact matches are claimed. The final tree retains one guarded Matrix22 multiply implementation with bounded emulator validation. Quaternion makeVectorRotation remains a packet: its earlier scalar form has observable NaN differences, and the three grounded follow-ups below did not produce an acceptable exact candidate. The quaternion diagnostic source and helper were removed from the final tree.

Base: `e2cbf8db72566da78fc16b77a5b90026bfd9ea0a`. Branch: `dot/quaternion-matrix-layout`. Work was isolated on the dot cloud computer. The EU executable SHA256 was rechecked as `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. All compiles used committed repository C++ through unmodified `python make.py eu`, ARMCC 4.1/791, and the project's unchanged flags. No software was installed. No copied SDK implementation or public implementation source was used.

## Matrix22 result

Target: `_ZN4sead15Matrix22CalcCtrIfE8multiplyERN2nn4math5MTX22ERKS4_S7_`, `0x0027C208..0x0027C23C`, 52 bytes. The source is `lib/al/src/Math/seadMatrix22Multiply.cpp`; it is the scalar-snapshot form already in packet 0027C208, now enrolled as committed guarded source. It is not a fourth distinct source-form attempt. The target's row-major layout and multiply-then-accumulate order are preserved. Inputs are fully consumed before output writes, including legal same-object aliases.

The unchanged checker reported:

```text
python tools/check.py _ZN4sead15Matrix22CalcCtrIfE8multiplyERN2nn4math5MTX22ERKS4_S7_ --object build/eu/obj/lib/al/src/Math/seadMatrix22Multiply.o
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The complete candidate is 80 bytes, versus 52 original bytes. The four separate left loads and four result stores remain the key structural gap; the original groups each into VFP multiple transfers and allocates the results to S8..S11 without input-register reuse. No byte-distance figure is reported because the full-size gate fails first. The accepted Matrix33 copy/transpose and Matrix34 copy sources on this base use ordinary scalar snapshots, but they provide no evidence that would justify artificial register liveness or an instruction stand-in for this arithmetic routine. No new Matrix22 form was spent on cosmetic rearrangement.

The canonical object was replayed against the owner's original interval under installed Unicorn 2.1.4 ARM1176. All 31,675 pairs agreed: 905 synthetic operand pairs, seven FPSCR modes, and five alias patterns. Comparisons covered all 1,024 bytes of the input/output sentinel region, all four result words, FPSCR excluding call-clobbered NZCV, and preservation of R4..R11/S16..S31. Full FPSCR NZCV also agreed in every Matrix22 pair. Cases included 257 finite pairs, 136 exceptional/special pairs, and 512 arbitrary binary32 pairs. Modes were nearest, positive, negative, zero, nearest with flush-to-zero, nearest with default-NaN, and nearest with both. Alias patterns were distinct objects, equal inputs, output=left, output=right, and all equal. For equal-input cases the second written operand supplies the shared record.

This is synthetic emulator evidence. It does not establish hardware behavior, all possible float values, partial record overlaps, gameplay, or exact matching. The 16-byte MTX22 layout is recovered; the C++ `void` declaration is an effect-only model consistent with the packet, not an independently recovered public return-type contract.

Source SHA256: `481c093609073ed6fec4aadfc3c1e54008ae3705c3954c4f7420fa55ea5ee2cb`. Initial canonical object SHA256: `dc13a72afdd9139ed9a3e44c275e5d337f01eadad0a34ac703d56093838059a9`. Function-section SHA256: `6770e250f309624a091b96780fa218a11d4b97fe77083ca349b625c169802469`. Original interval SHA256: `bd0d11c3c8de738586b385737f17eb6034a2bebe37b5702660b32f2e805d2230`.

## Quaternion behavior counterexample

Target: `_ZN4sead11QuatCalcCtrIfE18makeVectorRotationERN2nn4math4QUATERKNS3_4VEC3ES8_`, `0x00260554..0x00260600`, 172 bytes. Reproduction commit `0f757fe` builds packet form 2 unchanged in substance. The canonical object has 180 bytes and the unchanged checker reports the same full-section size failure as Matrix22. No diagnostic map names were needed for either native symbol.

The target compares FLT_EPSILON against the accumulated sum with VCMPE. Under current fast-math flags the ordinary C++ threshold becomes a signed integer comparison of the float's bits against `0x34000000`. These agree for the finite cases tested but differ for negative NaNs. With nearest rounding, from words `(0xffc12345, 0, 0)` and to words `(0x3f800000, 0, 0)`:

- Original returns true and writes `(0xffc12345, 0x7fc12345, 0xffc12345, 0xffc12345)`; FPSCR is `0x30000001`
- Candidate returns false and writes identity `(0, 0, 0, 0x3f800000)`; FPSCR is `0`

Even positive quiet NaNs that take the same branch can differ in the accumulated invalid-operation flag, because the original VCMPE is signaling. This invalid-operation difference is separate from FPSCR's call-clobbered comparison flags.

No caller-established input contract excluding these values was recovered in this lane. These are observed edge-case differences, not evidence of a gameplay failure.

The baseline replay ran 876 inputs in seven modes, 6,132 pairs. Data/return comparisons failed in 100 pairs; arithmetic-status comparisons failed in 119. These counts overlap and must not be added. All 1,792 normalized-vector pairs, all 35 threshold-boundary pairs, and the seven simple finite pairs agreed in memory, return, and non-NZCV FPSCR. Special cases had 60 data/return and 84 status differences; arbitrary binary32 cases had 40 and 35 respectively. Full FPSCR NZCV differed in every pair because only the original updates its VFP comparison flags. No claim of a generally behavior-equivalent quaternion implementation follows.

Baseline source SHA256: `6bdaa89b8bcb92f1e5e201636e58aaaea8c2d4a4f64d595efa599eedde50916d`; canonical object SHA256: `54681fdf5d0b5ba8dfca53e4a6c1c902f5e72aad015061b365500e4d24febc49`; section SHA256: `aaa5a6d39bb28d7679f67f09b5043dd56e475097c40b8bdd9e390b865e679495`; target SHA256: `f9100c2b258309f504dc6f9e7ac8bfb4f0f41229fdcae268d1f7e9f305bcd262`.

## Grounded follow-ups and stop condition

The quaternion packet previously tried two forms. This lane tested three more hypotheses, counted conservatively as five total attempts including the rejected pragma. Matrix22 remains at its original three. No eight-form cap was cycled.

1. Commit `00a6e9b`: standard `isgreaterequal(FLT_EPSILON, sum)` requests an ordered comparison independent of NaN sign. The installed compiler's own math.h documents quiet-NaN handling and expands this to `__ARM_fcmp4`. The compiler emits a 212-byte root with a call. Unchanged check output: `Source closure rejected: A source helper has no unique canonical C++ definition: __ARM_fcmp4`. No original address was invented for the library helper. This form was not functionally replayed.
2. Commit `781bc96`: a separate ordinary C++ translation unit receives both operands as runtime floats and evaluates `limit >= value`. This tests whether late linking can retain the signaling comparison without constant folding. The helper is 20 bytes and contains VCMPE/VMRS plus boolean selection; the root is 212 bytes. The checker was run with its supported `--inline-object` argument. Output: `Source closure rejected: Residual helper code or another allocated extent remains after inlining.` The unchanged link map has a 212-byte root at 0x00260554 and a retained 20-byte helper at 0x00260628, total 232 bytes. No exact or functional replay credit was taken, and no standalone original identity is asserted.
3. Commit `dc39e3a`: `#pragma STDC FENV_ACCESS ON` tests whether a source-level environment declaration preserves signaling semantics under this compiler mode. ARMCC reports `Warning: #161-D: unrecognized #pragma`. The root remains the same 180-byte section and the checker reports full-size mismatch. This source/compiler-mode hypothesis is closed; the unsupported pragma is not retained.

These failures supply a concrete blocker: the present ordinary C++ path does not reproduce the target's ordered signaling comparison together with its transfer/register allocation and shared identity pool. Further work needs new compiler-stage or clean API evidence, rather than another aggregate/field-order spelling. Adding guessed pool objects, arbitrary addresses, volatile reads, artificial liveness, or assembly substitutes would not resolve the permitted problem.

## Reproduction and hygiene

The replay harness and exact commands are in `project/dot_reports/quaternion-matrix-layout-replay.md`. It reads game bytes locally, validates original/object/input hashes, and does not embed any game bytes. Replay summaries are recorded as text in this report. Local detailed JSON, compiler objects, closure maps, and logs remain under ignored build storage or /tmp.

Only source and notes are retained. Local checker rank updates were restored; no map, ledger, STATE, tools, compiler flags, function boundaries, target bytes, or accepted shared headers are included. The first two checks were accidentally launched together; the Matrix22 reader observed the quaternion checker mid-write and failed before checking. The map writer completed, and a sequential Matrix22 rerun produced the size result above. That transient invocation contributes no measurement or attempt.

Final verification at source checkpoint `ccdcd34`: `python make.py eu -ca` exits 0, compiling 42 Game, 103 Actor, and one SDK source, then linking and exporting the compact scaffold. The sequential Matrix22 `--object` rerun still reports the 80/52-byte size failure. A fresh canonical-object replay again passes all 31,675 pairs, with the identical object and function-section hashes recorded above. `git diff --check` passes. The original executable hash remains unchanged. These are build/replay checks, not preservation claims for every accepted root. No push was made; the parent owns publication.
