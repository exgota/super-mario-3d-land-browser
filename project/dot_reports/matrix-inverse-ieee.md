# Matrix33 inverse IEEE and aliasing follow-up

Zero exact matches are claimed. This branch retains a guarded, ordinary C++ inverse with bounded behavior evidence: all 75,392 original/candidate pairs agree. Its complete canonical compiler section is 404 bytes against the immutable 260-byte retail interval, so it remains NonMatching. The existing packet's scalar source has concrete signed-zero, directed-rounding, and exceptional-value differences; those are now reproducible rather than assumed away.

Base: `a4f1579f9f0f13dacfe91d92501cf74154c131ad`. Branch: `dot/matrix-inverse-ieee`. Source checkpoint: `f288d8b`. Work used the dot cloud computer and the already installed ARMCC 4.1/791, wibo, Python environment, and Unicorn 2.1.4. No software was installed, no compiler or optimization flags changed, and no external implementation source was used. Original EU executable SHA256 was verified as `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64` before replay.

## Source and exact result

Target: `_ZN4sead15Matrix33CalcCtrIfE7inverseERN2nn4math5MTX33ERKS4_`, `0x0027C23C..0x0027C340`, 260 bytes including its eight-byte pool. Existing row type `fa`, rank `U`, enclosing section, and all boundaries are unchanged in this proposal. Source: `lib/al/src/Math/seadMatrixCopy.cpp`. The existing `float m[3][3]` MTX33 record remains 36 bytes. Accepted copy and transpose method bodies are unchanged.

The packet had already tried three forms. This lane reproduced form 1 at commit `2cd498e` and tested one new form, at `da506ec`, for four total distinct forms. The final source checkpoint changes only its diagnostic comment. Reproduction of the existing form is not counted as a fifth attempt.

The new form keeps the packet's nine input snapshots, determinant grouping, adjugate grouping, and output stores. It adds two justified IEEE representation operations:

- Copy the determinant's representation into an unsigned word and compare against both zero encodings, matching the target's integer zero test
- Compute each of the four alternating cofactor products first, then invert its sign bit through ordinary `memcpy` object-representation copies and XOR

The second operation addresses a measured compiler transformation: under the unchanged fast floating-point flags, `-(a*b-c*d)*reciprocal` became `(c*d-a*b)*reciprocal`. That changes zero signs and can change rounded finite results. The bit operation preserves the target's sign inversion after the rounded product. `matrix33NegateProduct` is only a source helper; no retail helper identity or address is asserted. It disappears completely from allocated compiler sections. There are no assembly substitutes, volatile fields, artificial register-liveness devices, or external data relocations in either replayed root.

Both candidates came from committed source built by unmodified `python make.py eu`. The unchanged checker was run with the canonical object:

```text
python tools/check.py _ZN4sead15Matrix33CalcCtrIfE7inverseERN2nn4math5MTX33ERKS4_ --object build/eu/obj/lib/al/src/Math/seadMatrixCopy.o
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Form 1 is 308 bytes. Form 4 is 404 bytes. The final check returns exit 1 at the full-size gate; no byte-distance or matching claim follows. No isolated original-address link is proposed for the oversized body. The size increase is the cost of explicit representation operations in this compiler, not progress toward exact matching.

## Behavioral counterexamples in the packet source

With round-to-nearest and a normal identity input, the original returns diagonal ones, positive zeros at indices 2 and 6, and negative zeros at indices 1, 3, 5, and 7. Packet form 1 returns positive zero at all six off-diagonal positions. Both have clear arithmetic exception flags; only form 1 changes FPSCR comparison flags.

A finite counterexample uses rows `(1,2,3)`, `(0,4,5)`, `(0,0,6)` and rounding toward positive infinity. Original output indices 1 and 5 are respectively `0xbf000001` and `0xbe555556`; form 1 gives `0xbf000000` and `0xbe555555`. The sign-operation ordering matters even when NaNs and signed zero are excluded from those result slots. No caller-established restriction to a particular rounding mode was recovered here.

Across the final 75,392-pair corpus, form 1 differs in memory in 57,108 pairs and in non-NZCV arithmetic status in 152 pairs. The status failures are contained within the memory failures and must not be added. Full FPSCR NZCV differs in every pair, because the source uses a floating comparison where retail uses an integer comparison. Its 256 ordinary finite random matrices do agree in round-to-nearest and round-toward-zero, but 250/256 differ under rounding toward positive infinity and 252/256 under rounding toward negative infinity, for each of the four FZ/DN combinations and each alias mode.

| Synthetic group | Matrices | Pairs | Form 1 memory failures | Form 1 status failures | Form 4 failures |
| --- | ---: | ---: | ---: | ---: | ---: |
| Named examples | 9 | 288 | 112 | 0 | 0 |
| One special value in identity | 171 | 5,472 | 5,088 | 0 | 0 |
| Diagonal and zero sign combinations | 512 | 16,384 | 14,336 | 0 | 0 |
| Ordinary finite random | 256 | 8,192 | 4,016 | 0 | 0 |
| Duplicate first two rows | 256 | 8,192 | 5,424 | 0 | 0 |
| Arbitrary binary32 words | 512 | 16,384 | 9,188 | 152 | 0 |
| Two NaNs with differing signs/payloads/classes | 576 | 18,432 | 16,896 | 0 | 0 |
| All entries NaN | 64 | 2,048 | 2,048 | 0 | 0 |
| Total | 2,356 | 75,392 | 57,108 | 152 | 0 |

The initially completed 1,716-matrix corpus excluded the last two groups: 54,912 pairs, with 38,164 memory and 152 status failures in form 1, and none in form 4. The larger corpus is the final evidence and supersedes that preliminary count.

## Replay coverage and limits

Each of 2,356 synthetic matrices runs in all sixteen combinations of four rounding modes, flush-to-zero on/off, and default-NaN on/off, with exception traps disabled. Every combination runs once with distinct records and once with output equal to input. Inputs include both zero signs, subnormals, minimum normals, extreme finite values, infinities, quiet and signaling NaNs, multiple NaN payloads, and numerical cancellation cases. Duplicate rows are mathematical singularities; this report does not assume finite-precision determinant evaluation always gives exact zero for them.

The harness executes the original local EU interval and the canonical ARMCC section independently in Unicorn 2.1.4's ARM1176 model. It validates the binary, object, and all recorded project input hashes and rejects unexpected relocations. It compares all 1,024 bytes of the sentinel/input/output region, all nine output words, R0, and FPSCR. It checks preservation of R4..R11, S16..S31 and SP, and guards stack memory outside the permitted local save area. Every form-4 pair agrees in full FPSCR, including NZCV. Both engines separately produce the same output and FPSCR for distinct and same-object aliases in all 37,696 mode/input combinations.

This establishes a bounded emulator result, not exhaustive binary32 equivalence, physical ARM11 behavior, untested FPSCR exception-trap configurations, a public return-value API, native-host compiler behavior, partial-record overlaps, game integration, or gameplay. The `void` signature remains the packet's effect-only API model; preservation of R0 does not establish an official return contract. No direct caller contract was added or guessed.

## Remaining exact-match blocker and source plan

The fixed register batches in retail remain unexplained by the accepted ordinary C++ API/layout evidence. Retail loads nine VFP registers together, performs the zero branch before opening the callee-save frame, computes the cofactors in a coordinated register block, and stores nine output registers together. The four grounded source forms do not recover that structure. Inherited `fa` metadata and the scheduling pattern are clues, not proof of original source language.

Do not restart with field order, fabricated liveness, volatile accesses, a guessed helper address, new literal ownership, or changed compiler flags. A further matching attempt needs independently supported translation-unit/compiler-stage or clean API evidence that can explain those transfer and frame choices. The retained representation-preserving source is useful for bounded behavior work meanwhile; it adds zero exact bytes. Source search stops at four forms.

## Verification and reproduction

Final `python make.py eu -ca` at source checkpoint `f288d8b` exits 0: 42 Game, 120 Actor, and one SDK source compile, then the compact scaffold links and exports. The sequential final canonical checks preserve the accepted Matrix33 copy and transpose siblings (`O -> O`) and again reject inverse for full-section size. The fresh final object repeats the complete 75,392-pair replay. This is not an audit of all 618 accepted functions.

The complete executable diagnostic harness and commands are in `project/dot_reports/matrix-inverse-ieee-replay.md`. It embeds no game bytes and reads the original executable only from local ignored storage. Local JSON summaries, objects, provenance records and logs remain under ignored `build/matrix_inverse_replay/`; they are not committed.

Final canonical object SHA256: `3e43a0b604d4571af6a58bd9ca7f8e62bb042d7f01f3f2707550db5a45878d8a`. Baseline object SHA256: `0c7d58720ae31fb910c283fa939b1b855afd567b4c41cf7f5e383e1137c2f724`. Object fingerprints include local path-dependent build material and are evidence identifiers, not portable acceptance checks.

Final source SHA256: `cab00f50ef06824da5432ae6a30c4f03fed62b57f1b072c2d9ee95728f5c2876`. Candidate function-section SHA256: `dd12b31545feaa4a9da60285e34d4123bfd36f355993570b6729a420dd9cd486`. Original interval SHA256: `46d74868c97c5d608e8beeb0843950d4f2158a69e90c70f640fa3d9738e53add`. Baseline form-1 source SHA256: `2f62b24a2fdfa86dc8d998369bff40746ae9e484992a5f0674a5e874edf1cfc7`; baseline section SHA256: `9f513cbf51efd69fff7347882094a7d97867c077ed959c02478ae58151b9e98c`.

Only source and notes are proposed. Checker rank changes were restored. No map, bounds, type, ledger, STATE, tools, config, accepted header, target data, or game-data changes are included. Publication remains with the parent.
