# Fixed-point processor 001F9F48

## Frozen independent result, released after grading

This source was frozen at `26ff3caa04849e6bc1c401b33b30c0d1ced27ba5` on **2026-10-01 22:22:45 UTC**, before this lane read the Pro grades. It was deliberately kept unpublished during the owner's trial. The grade in main `b16e2a0cc32e0cdacac5ba94feabe67432433391` was first read here at2026-10-01 23:17 UTC; the owner authorized publication of the dated result once that grade landed. This release changes notes only: all C++ and header bytes are identical to the frozen source. The measured base remains e87d556, not current main.

The frozen dot body remains564/572 bytes with490 raw unequal/missing bytes. Main grades Pro bodies at560/486 and544/480 (compiled bytes/raw differences). Both investigations identified the same accumulator recurrence issue; their worked examples use different parameters. The dot source retains its previously tested unsigned overflow handling and1,236-pair ARM/native evidence. Neither proposal is exact; no Pro source was incorporated.

[Main's separately graded response](https://github.com/exgota/super-mario-3d-land-browser/blob/b16e2a0cc32e0cdacac5ba94feabe67432433391/project/pro_responses/001F9F48.md). The response file on this old-base branch is the historical dot response, not a replacement for main's Pro grade. Any future intake must preserve the main grade and follow current family/preservation rules.


## Result

This is a corrected, bounded-replay-verified NonMatching proposal for the unchanged unnamed U interval `001F9F48..001FA184` (572 bytes). It adds **zero exact functions or bytes**. The final source checkpoint is `dc85325ec478671f3fe83404a676a26de5f669e5`, based on main `e87d556cd4d993606a03e205b4604d863d718bd4`; reservation was `f87b657b7c51508c6bb09b4feaf4f6f501995c53`. The working branch is `dot/fixed-point-processor`.

Source: `lib/al/src/Util/FixedPointBufferState.cpp`; neutral layout: `lib/al/include/Util/FixedPointBufferState.h`. The original class/API name remains unknown. The normal `NON_MATCHING` guard excludes this body from matching-only builds. No original table identity, guessed helper, imports, assembly implementation, or copied binary data was added.

A clean canonical `python make.py eu -ca` compiled 42 Game, 128 al and 1 SDK sources, linked and exported. The configured al compiler is ARMCC 4.1 build 791 (compiler SHA256 `d1f328ae28aa231877604b0f9ce73a8f00fc817fb08bb6f823cca21518f0d13d`). The final canonical source object is `build/eu/obj/lib/al/src/Util/FixedPointBufferState.o`, SHA256 `fe7e9df080c05384f1f93ffcd31320b59c6c3cedf046292d578331d33cfb704a`. Its sole executable section, with no relocations or helper sections, is 564 bytes, SHA256 `e79602d5bc24c07e365d7a1783476c1c98d6a9181c98637211fbda9b9b454653`.

The project checker on committed source reported:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The checker exited 1. The unchanged row received only a transient neutral symbol for this check; its original complete contents were restored afterward. The final map SHA256 is `a8cd649c372ae92c33ae26d4c4a90dc84f257e549e6465b292e419a2a2ce9b97`. No map, rank, ledger, STATE, config, tools or binary changes are committed. No separate full 651-root acceptance audit was run in this lane; the new header is used only by this new source and its U body is absent from the compact link.

## Corrections to the packet

The packet's proposed recurrence is wrong. Let `t` be the old terminal word, `d` the newly stored terminal word, `c` its coefficient, `a` the prior accumulator and `k` the accumulator coefficient. Define `Q(x,c)` as sign-preserving low-word multiply followed by arithmetic shift 7. The original computes:

```text
r = t - Q(d,c)
result = r - ASR7(low32((a + r) * k))
```

The packet instead adds `a` to `r` before the final subtraction and stores that enlarged value. At original 001FA0D0 only r8 receives the sum, while 001FA0EC subtracts the shifted product from r2. A replay regression with terminal 128, prior accumulator 64, terminal coefficient 0, damping 64, primary coefficient 0 and output coefficient 128 returns sample 32 in retail and this source; the packet formula predicts 96.

The packet also missed direct conditional branch callers. `001F6A0C` is a BLNE to the processor in root `001F6940`. It constructs four input pointers from its incoming buffer at offsets 0, 0x280, 0x500, 0x780. Thus the observed call uses distinct, adjacent 160-word slabs and processes the first two in place. `001F6D30` calls setup 001F9E20. `001DF1D8` calls constructor 001FA598 after an explicit 0x104 allocation at001DF1C8. Calls 001DF274/280/2A4 establish parameter setting, required-work-size query and attachment in the same allocating path. These are original bytes from the verified owner executable, not inferred symbols.

The constructor's literal at001FA6CC is `003D7F6C`, not the packet's `003D7ECC`. It remains an unnamed datum. Its actual public/class identity is not established.

## Integer and storage contract

Words and coefficients have 32-bit storage. MUL/MLA keep the low 32 bits; ADD/SUB/RSB wrap. Negating INT_MIN therefore retains 0x80000000. Each multiply is reduced before arithmetic right shift 7, even if a mathematically positive magnitude product overflows its sign bit. This is not a 64-bit fixed-point multiply, saturating arithmetic, or universally truncating division by 128.

All potentially overflowing C++ arithmetic uses unsigned words. The final source explicitly requires the tested implementation-defined unsigned-to-signed conversion and arithmetic signed shift, guarded by compile-time checks for 32-bit words, the INT_MIN conversion, and negative right shift. Negation after a shift cannot overflow because the shifted range is only [-16777216, 16777215]. There is no signed-overflow assumption. Earlier form 5 is a fully unsigned logical-sign-extension alternative in history, but its canonical section is 680 bytes. The final source is verified on ARMCC791 and GCC 14.2.0; untested integer implementations are not claimed portable.

The enabled field is a byte, not a C++ bool with invalid noncanonical representations. Constructor allocation and independent setup/reset observations agree with a 0x104 ARM receiver and the header offsets. Only channels 0/1 are processed. Five cursors are loaded again for channel 1, increment 160 times, and finally stored once, retaining the cursor only when the corresponding unsigned length is greater. There is no within-block ring wrap.

Original static initialization 385988 creates the default 3040/3680/2080 triplet at 42AE68. Whole original constructor 001FA598, work-size 001FA374, attach 001FA350 and setup 001F9E20 run without stubbed setup, power or memory-clear calls. Default setup yields work size 111392 bytes, ring lengths 1920/3200/3040/3680/2080, feedback coefficients 109/105, terminal 64, primary 76, output 51 and damping 51. Setup rounds the attached work pointer to 32-byte alignment and places the ten channel-specific rings consecutively. The observed allocation/caller path supplies separate work and input storage. Input/output aliasing within each slab is intentional and tested.

Successful parameter-setting alone does not prove a safe processing geometry: the optional three lengths are checked nonzero but need not be multiples of 160. The replay uses positive 160-multiple lengths, valid capacities and cursors. Arbitrary overlap with receiver fields, ring aliasing, oversized/wrapped pointers and non-block-sized rings remain unsupported; no reachability or equivalence claim is made for them.

## Verification scope

The complete, executable [replay recipe](fixed-point-processor-replay.md) contains all Python/native build steps and needs only this committed source, the approved compiler/tool dependencies and the owner's local executable. It contains no binary data.

Final ARM1176 Unicorn 2.1.4 replay passed 1,236 returning whole-root pairs: 45 independently initialized configurations each ran 26 consecutive blocks (1,170 pairs), 64 cases used synthetic full-width histories/coefficient boundaries, one disabled case supplied a null input-table pointer, and one focused accumulator regression distinguishes the packet bug. Each pair independently runs original static initialization/constructor/setter/attach/setup. For the final ARM suite this is 230 independent initialized machines including four fault pairs. Each successful root checks the full receiver, the 1 MiB work arena including untouched guards, all four input slabs, and the pointer table. It also checks return to the sentinel, r4–r11, SP and d8–d15 preservation. Stack scratch and caller-clobbered registers are not required equal.

The 45 sequences combine five legal setter/default configurations, three input patterns (signed 16-bit values, INT_MIN/INT_MAX and shift boundaries, full 32-bit words), and three work alignments (0/3/31). Twenty-six blocks cross every default ring wrap; the longest is 23 blocks. The 64 additional histories deliberately include coefficients outside the setter's normal range and are separately synthetic arithmetic evidence, not reachable gameplay states.

GCC 14.2.0 C++03 builds at -O0 and -O3 each passed the same 1,236 original-versus-native pairs with UBSan and no diagnostics (2,472 native pairs). Native receiver pointers are translated to a host-layout struct and compared back to the ARM layout; its initializer comes from a separately executed original setup. Four separate ARM probes (null input table, null primary, null feedback, huge primary index) produced unmapped-read faults in both bodies; faults are classified only and excluded from returning equivalence credit. Malformed states are not sent into native C++.

This is bounded routine replay using constructed inputs, not original hardware, recorded gameplay, full audio-system equivalence or exact reconstruction. Reports and temporary binaries stay under ignored `build/fixed-point/`.

## Attempt history

Four packet forms preceded this work. This pass added exactly four meaningful forms and stopped at eight cumulative forms:

| Form | Source checkpoint | Hypothesis | Section bytes | Raw unequal/missing bytes |
|---|---|---|---:|---:|
|5|a47a4ff|Correct recurrence; unsigned low-word arithmetic and explicit logical sign extension in inline helpers|680|604|
|6|b1daf4a|Factor sign selection around multiplication; scoped local stages without source helpers|620|541|
|7|ba8ba88|Explicit original-style branches and checked target signed-view shifts over unsigned products|564|490|
|8|7ac2b2c|Bounded signed local cursors permitted by valid setup geometry|564|490|

Forms 7/8 emit identical instruction bytes. The final dc85325 checkpoint retains form 7's unsigned cursors, which need fewer input assumptions. These raw diagnostics count unequal corresponding bytes plus unequal extents; they are not canonical exact credit. Final frame 0x5C still differs from original 0x24, the sample pointer and outer-channel value have different lifetimes, and branch/register scheduling remains different. Forms 5/6/7 received canonical size-mismatch checks. The form 8 checker invocation did not obtain a transient named row and rejected before comparison; its canonical built section was independently identical to form 7, and the restored final source received its own successful provenance check followed by the size rejection above. No compiler flag, oracle or boundary was changed and no further source search ran.
