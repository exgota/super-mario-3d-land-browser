# Light-area introsort, 0x0039F3FC

This is a guarded **NonMatching proposal, with zero accepted roots/bytes and zero accepted throughput**. The canonical project checker rejects both complete emitted functions on size. The root is 2,636 compiled bytes against the unchanged 2,624-byte original; its generated heap helper is 1,808 against 1,240. The emitted C++ closure totals 4,444 bytes against 3,864 original bytes. The small root size delta is not a similarity claim: an original-address diagnostic compares 2,076 different bytes within the common 2,624-byte extent, plus 12 extra candidate bytes.

Branch `dot/root-39f3fc`; frozen base `45a4305466a7c4cbd87589169384102e88f8d1b5`; final source commit `40eaa76`. Assignment began 2026-10-02 01:11:26 UTC. No canonical acceptance has occurred. Parent owns publication and the authoritative intake gate.

## Scope, ownership and provenance

Only `lib/al/src/Light/LightAreaIntrosort39F3FC.cpp` and these notes are submitted. No existing source/header is edited. The type names in private namespace `dot39f3fc` are descriptive, not recovered Nintendo symbols. This belongs to the same broader lighting family as completed `dot/root-1c6410`, whose director calls this root, but that lane is complete and its header/source are unchanged. The private type spelling differs and no definition of the earlier C-linkage `fn_0039F3FC` declaration is introduced. Future shared-type consolidation requires explicit ownership review.

The algorithm is instantiated through `<algorithm>` from the already approved ARMCC 4.1/791 distribution, specifically `data/compilers/4.1/791/include/algorithm` and `algorithm.cc`. No removed SDK/library source or vendor implementation text is copied into this patch. Source uses the existing clean-room `sead::Vector3f` header. The configured al compiler and all flags are unchanged. Caller/lighting ownership supports choosing the al module; M0's game discriminators do not independently prove the original compiler build for this exact template specialization. No alternate compiler or flag-rescue test is claimed.

## Independent identification and ABI

The sole nonrecursive direct caller is the lighting-data director at `0x001C6410`: its call at `0x001C69E4` supplies a 0x128-stride range, count as signed int, and the comparator at `0x002D5918`. The caller and the separate parser `0x001C4F64` establish a name pointer, interpolation count and three 0x60-byte light records. The nested parser `0x0024E0B8` identifies Ambient, Diffuse, Specular0, Specular1, ConstantColor5, Direction and IsCameraFollow. Original copy `0x0024E054` and assignment `0x0024DAFC` independently transfer 23 four-byte words and the boolean at +0x5C while leaving +0x5D..+0x5F untouched.

The root has the distinctive median-of-three partition, 16-element cutoff, recursive right partition, halved signed depth budget on the remaining left partition, and heap fallback. Its mapped code ends at `0x0039FE34`, followed by two literal words through `0x0039FE3C`. The constant 0x13A7 implements the byte-distance comparison for more than sixteen 296-byte records; the other literal is the signed division reciprocal. Full unchanged target disassembly and literal words are in `root-39f3fc-target.md`.

The existing helper row `0x00204950..0x00204E28` is independently identified as this area's heap-adjust specialization, not assigned an invented boundary. Original root call sites `0x0039F524`, `0x0039F778` and `0x0039F9D0` pass base in R0, hole index in R1, element count in R2, a pointer to the caller-created by-value Area temporary in R3, and comparator as the fifth stack argument. The original helper reads those arguments, selects between children at `2*hole+1/2`, transfers 0x128-byte records, follows the hole downward, then bubbles the saved value upward. Its calls to original copy/assignment independently agree with the light layout. The emitted `std::__adjust_heap` uses this ABI and is an actual root dependency, not unused emission. It remains separately NonMatching; closure replay below executes its compiled body, rather than inferring its behavior from the retail helper.

The real comparator calls original strcmp `0x0028AA60` and returns `strcmp >= 0`. Equal names therefore violate the strict-order assumption. This is retained and explicitly tested, not corrected. The root alone performs partitioning/heap fallback; the caller's later insertion-sort finish is outside this proposal.

## Four frozen forms and canonical results

| Form / source commit | Root bytes | Helper bytes | Change |
| --- | ---: | ---: | --- |
| 1 / 657a8f5 | 2388 | 1616 | Standard algorithm with aggregate colors/direction |
| 2 / d7e1903 | 3992 | 2952 | Explicit four-scalar color assignment |
| 3 / d598882 | 2388 | 1616 | Restore aggregate colors; model non-POD constructor declarations |
| 4 / 40eaa76 | 2636 | 1808 | Existing scalar Vector3::set for direction assignment |

All forms were committed before ordinary `make.py eu` and the unchanged `tools/check.py --object`. Those initial diagnostics also temporarily changed the two template rows from f to ft; they are classification-dependent diagnostics, not unchanged-classification canonical eligibility evidence. A subsequent final-source rerun retained the original Type f on both rows and changed existing-row names only. That names-only check independently reaches the same size failures for both roots. The source and canonical object were unchanged for this correction. Form 3 tested a real type-property hypothesis but produced the same function bodies as form 1. Four unsuccessful source forms end this pass; no further matching edits were made. The final source remains guarded with `NON_MATCHING`.

Exact final output for each of the two symbols is:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Root symbol: `_ZSt16__introsort_loopIPN9dot39f3fc4AreaEiPFbRKS1_S4_EEvT_S7_T0_T1_`

Helper symbol: `_ZSt13__adjust_heapIPN9dot39f3fc4AreaEiS1_PFbRKS1_S4_EEvT_T0_S8_T1_T2_`

The corrected final check temporarily names three existing rows only: root, heap helper and Light copy constructor. Both existing root/helper rows keep their original Type f throughout this check. The older ft-classification result is separately labeled diagnostic in `root-39f3fc-metadata.md`, with the classification evidence reserved for main review. Every boundary, type, rank and name is restored byte-for-byte afterwards. The original rows remain U. The original map SHA is `d517e7aa3eefdbb859e781d7d974f8daf4fbcb33b964edb7d0a5ef6a11db8bd9`. No checker output is inferred from a handmade object: the two checks consume the canonical committed-source ARMCC object produced by the project build.

## Two separate whole-root replay results

1. **Compiled root with original heap helper:** 265 paired fixtures pass, comprising 261 returns and 4 faults. The full root runs with original heap adjustment, Light copy, Light assignment, comparator and strcmp. It makes 9,024 original heap-helper calls, 84,066 Light-copy calls and 28,956 assignment calls. This establishes the root under these original callees only.
2. **Compiled root plus compiled heap helper:** a separately linked 4,444-byte closure independently passes the same 265 paired fixtures, 261 returns and 4 faults. This run uses compiled heap adjustment and its inlined assignments, while original Light copy, comparator and strcmp still execute. It makes no calls to the original heap helper or original standalone assignment. Results, complete observed data-memory hashes and ordered comparator-argument trace hashes match the first run for every fixture.

Both runs execute on Unicorn's ARM1176 model with FPEXC explicitly enabled. They contain **no modeled function endpoints**. There are 39,567 independent record/padding/heap-order/no-op assertions in each suite; whole observed data memory, comparator argument order, FPSCR, SP, R4-R11 and D8-D15 are checked. R0 is not compared because the function returns void. An initial discarded harness omitted VFP enablement; the retained harness requires successful return for every normal fixture and treats unexpected faults as failures.

The cases cover lengths 0..18, 31,32,33,63,64,65,127,128,129,257; forward/reverse/shuffled/zigzag/rotated unique names; zero, positive and negative depth budgets including INT_MIN; two all-equal heap cases; and three memory sentinel patterns. Payloads include arbitrary 32-bit words, infinities, signed zero, signaling/quiet NaN encodings and subnormals. Seven FPSCR modes cover rounding, FZ, DN and preexisting exception flags. These are bit-preserving copies; no floating arithmetic accuracy is claimed.

Data comparison covers `[0x00600000,0x00640000)`, including the complete record array, name strings, and guard bytes. It excludes stack scratch bytes, because legal stack layout/lifetimes differ. Callee-saved registers and SP are checked on returns; fault cases do not assert a successful ABI return. Bounds and pointers are controlled, not a model of arbitrary host memory or production assets.

Both all-equal positive-depth cases (17 and 33 records) fault after scanning beyond the logical array and reading an invalid sentinel name pointer 0xA5A5A5A5. Their comparator-call counts are 20 and 36. Null names and null nonempty ranges produce the same two additional read faults. Fault type/address/width, ordered comparator trace and compared data memory agree. No general memory-safety or recovery guarantee follows. Every run has a one-million-instruction budget per case; no final fixture reaches that budget. This is bounded evidence, not a proof of termination for arbitrary comparators/inputs.

The suite visits 508 of 654 original root instruction addresses. All 146 unvisited addresses are exactly `0x0039F544..0x0039F788`: the generated partial-sort scan retained after `middle == last`, which the original unconditional comparison skips. This is instruction-address coverage, including predicated instructions, not all paths or predicate outcomes. The root and source-helper reproduction is in `root-39f3fc-replay.md`; there is no game initialization, rendering, or gameplay credit.

## Clean build and previous-root preservation

A final normal clean `make.py eu -ca` compiles 45 Game, 130 al and one SDK translation units, links and exports. The candidate then repeats both size failures with names-only diagnostic bindings and unchanged original Type f. No shared objects, copied build output, compiler-flag changes or map edits are used in this build.

The pristine frozen-base report has a successful clean build and all **707 previously accepted roots / 726 canonical definition checks** passing. Its SHA-256 is `42dd622085ca5d98023e5a329bc5d966576fe6c487576bbbc479c60539013dd9`. The candidate's independent clean build preserves all **175 preexisting objects / 365 input files**, compiler hashes and normalized compile commands. ELF comparison checks every allocated/non-debug section byte, symbol attribute and resolved relocation. Only absolute STT_FILE paths, the checkout prefix in nonallocated .comment and resulting symbol/string-table indices are normalized. This is **baseline canonical checking plus exhaustive object/input/command equivalence**, not a claim that all 726 canonical checks were rerun on this candidate. Main's acceptance gate remains authoritative, including any roots added after frozen45a.

## Frozen hashes

| Item | SHA-256 |
| --- | --- |
| `lib/al/src/Light/LightAreaIntrosort39F3FC.cpp` | `e8d26f39de21af91e72f5387e6533dc3b2e918a0fce2a1630a451336a4e82ec4` |
| `data/ver/eu/map.csv` | `d517e7aa3eefdbb859e781d7d974f8daf4fbcb33b964edb7d0a5ef6a11db8bd9` |
| `data/ver/eu/code.bin` | `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64` |
| `data/compilers/4.1/791/bin/armcc.exe` | `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d` |
| `data/compilers/4.1/791/include/algorithm` | `4350c398a5c12cfb57d401c152747c1357ffbdc7783a57cf8ee23da6df372eaa` |
| `data/compilers/4.1/791/include/algorithm.cc` | `d2f6a1ba8d45c1c9593bb748982b208d757f236e7584d89b3c97d6791d95ac23` |
| `build/eu/obj/lib/al/src/Light/LightAreaIntrosort39F3FC.o` | `8e329d332fe5b1a054157219edde52ae9fb3ab90f10170ac823655bfee41f5d8` |
| `build/root39f3fc/behavior.axf` | `2c88a34c16552c1b455ece0b21c081721d58aaccc5b02920ec59865dff10a4e9` |
| `build/root39f3fc/closure.axf` | `1fbd6aed57cba851030cce739d1b10341add7e8c751277175e20480a72b00534` |
| `build/root39f3fc/preservation.json` | `561ef0d7e356b511eb631ea2cdc63ca25f9302b332fa90eb6d882a93df52503c` |
| Original full root interval | `791a1e76e202bc3ca9c251ead74acdbf56e20f74bf3d6b4b622b6daf87c63b81` |
| Root-only replay section | `5e6a2bb25c0a93356581e342393540a62a55040db9a835f9a598dd3c624403c7` |
| Root+helper replay section | `91a393c03295fde9edf9b7468dbd84197972ad93e473b3683a5ac6b1c48301a1` |

Generated binaries and target-backed traces remain ignored local evidence. The report and reproduction scripts are notes, not replacements for the authoritative checker.
