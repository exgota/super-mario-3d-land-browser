# Actor scale reader: bounded NonMatching result

## Frozen independent result, released after grading

This source was frozen at `428145213f778499afd2ccb337812e6d06087b49` on **2026-10-01 22:15:14 UTC**, before this lane read the Pro grades. It was deliberately kept unpublished during the owner's trial. The grade in main `b16e2a0cc32e0cdacac5ba94feabe67432433391` was first read here at2026-10-01 23:17 UTC; the owner authorized publication of the dated result once that grade landed. This release changes notes only: all C++ and header bytes are identical to the frozen source. The measured base remains e87d556, not current main.

**Historical comparison only: main now has a canonical172-byte exact implementation from Pro's void-setter fallback. Do not replace that accepted source with this older partial.** The frozen dot body remains172 bytes/4 differences on its original e87 baseline. It was not rebuilt with the new Vector3 setter and adds no exact credit. The accepted main result supersedes this proposal, so no further matching work on this root is requested.

[Main's separately graded response](https://github.com/exgota/super-mario-3d-land-browser/blob/b16e2a0cc32e0cdacac5ba94feabe67432433391/project/pro_responses/0024EC80.md). The response file on this old-base branch is the historical dot response, not a replacement for main's Pro grade. Any future intake must preserve the main grade and follow current family/preservation rules.


Base: `e87d556cd4d993606a03e205b4604d863d718bd4`. Branch: `dot/actor-scale-reader`.
Reservation: `ce07c3f`. Final source checkpoint: `40e54ee`.

The actor wrapper at `0x0024EC80..0x0024ED2C` remains **NonMatching**, with zero
exact credit. Its complete canonical 172-byte interval differs in 4 bytes, down
from the packet's 22. Only the allocation of R6 and R7 differs. The proposal is
ordinary C++ under the existing `NON_MATCHING` convention, with no changed
headers, flags, artificial helpers, assembly, volatile operations or altered
function boundaries. The accepted direct reader 156 is untouched.

## Source and canonical result

The wrapper calls the accepted `fn_00240FA8` on `info->mPlacementInfo`, retains
its Boolean result, returns true on success, and returns the retained false
result on failure. ARMCC fully inlines that accepted reader. All original
validity and three lookup calls, short-circuit branches, scalar copies, return
paths, frame size and literal-pool bytes agree. Four register fields differ:

| Address | Retail | Canonical source |
| --- | --- | --- |
| 0x0024EC9C | mov r7, #0 | mov r6, #0 |
| 0x0024ECC0 | mov r6, sp | mov r7, sp |
| 0x0024ECDC | add r1, r6, #8 | add r1, r7, #8 |
| 0x0024ED18 | mov r0, r7 | mov r0, r6 |

The committed source precedes its canonical `python make.py eu -ca` clean
build. That build compiles, archives, links and exports successfully with the
unchanged ARMCC 4.1/791 module configuration. The original target remains U in
the delivered map and is not claimed as part of the compact linked scaffold.
Only the checker set its temporary diagnostic rank:

```
python tools/check.py fn_0024EC80 --object build/eu/obj/lib/al/src/Placement/alPlacementFunction.o
U -> m: The linked candidate differs from the unchanged original interval.
```

All **56/56 previously accepted roots defined in the affected placement object**
pass the same canonical object checker. That includes the accepted direct scale
reader, which remains 156 bytes exact. This is a focused source-file preservation
check, not a fresh audit of all 651 accepted roots. The map is fully restored.

## Search cap and immutable attempts

Four earlier forms are preserved in the original packet. This pass adds four
committed forms; no further structural or cosmetic search follows:

| Total form | Source commit | Structure | Full section result |
| --- | --- | --- | --- |
| 5 | e0177f1 | Call accepted reader, explicit true/false outer returns |168 bytes; size mismatch |
| 6 | bf37b75 | Stage recovered vector, preserve false result, then scalar output assignment |156 bytes; compiler groups the copy |
| 7 | daf4245 | Call reader, preserve false return through failure guard, then return true |172 bytes;4 different bytes |
| 8 | 7a2a887 | Explicit success branch, then propagate retained false result |172 bytes; identical canonical object to form7 |

Final guard/comment commit 40e54ee does not change compiled bytes. Forms 7 and 8
produce the same object, so reversing the success guard does not resolve the
remaining allocation. All four forms preserve the accepted direct reader under
canonical checking. The original four packet source forms were not recompiled.
The next matching attempt needs new evidence about source/API context; this
result does not justify dummy variables, register forcing or guessed ABI flags.

## Real-callee and alias evidence

The complete reproducible [Markdown recipe](actor-scale-reader/replay_recipe.md)
passes **296 cases, 592 ARM executions and 173098 executed instructions** using
the owner's verified executable and the canonical source-generated root. Every
callee executes its real retail body. The harness does not intercept calls or
fabricate returns. It observes actual isValid276AA0, float lookup278C30, Byaml
data lookup28CAB4 and string comparison28AA60 execution.

The 296 cases are 216 combinations of absent/float/integer/Boolean/string/null
components, 13 IEEE bit patterns, 7 iterator/root state cases, 30 successful raw
alias cases and 30 failed raw alias cases. Float cases include signed zeros,
subnormals, finite extremes, infinities and both NaN classes. Alias destinations
overlap the iterator, ActorInitInfo, BYAML header/key table/root records and a
shifted output, with three incoming register/stack patterns.

Return values, the entire 128 KiB fixture arena, output-only write traces,
callee-saved registers and restored SP agree in every pair. Success copies all
three raw component words; failure preserves all output bytes. The test also
checks the exact number of attempted component lookups, including early failure.
These are constructed BYAML buffers and raw ARM alias observations, not an
arbitrary-buffer safety proof or a native-C++ strict-aliasing guarantee.

## Stack-observer counterexample

The register difference has an observable machine-state consequence even though
the bounded functional cases pass. Across 747 call-entry observations per side,
ABI arguments, SP and the active 32-byte caller frame agree. A wider 256-byte
snapshot also sees memory below that frame, where already-returned callees left
R6/R7 spill words. **158 such observations in 109 cases differ.**

The first counterexample has only scale_x present as float 1.0 and fails the
scale_y lookup. At the second float-lookup entry, bytes at entry-SP offsets
-92..-89 and-56..-53 differ. Those locations lie below the active caller frame.
The initial strict all-stack comparison correctly failed; the final recipe
retains and reports that counterexample, separately asserting caller-frame and
functional-memory equality. It does not erase the difference or claim complete
machine-state equivalence. The harness merely observes the unchanged callees;
no stack-reading substitute callee was added.

No exact credit, original asset-loading, hardware, gameplay or replay milestone
follows from this bounded result.

## Hashes and reproducibility

- Final source: `fdf51189bc10b657363b49d40dff4055c16a686f17493fe2bb5ab1fddc07487c`
- Canonical object: `f301b8248631543dc67c32a3abc8c1a2b47267523c40963512bf655c75bece6a`
- Linked complete 172-byte root: `1c660fde2f52abbd4b0876904d5c56d0cafa43afdbc9e4f1d9137c845021af64`
- Replay aggregate: `f7021f6d856c8117604680d9a175869bf6d7dbcd3ae7cec137af37c5dca233d4`
- Original EU executable: `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`
- Restored baseline map: `a8cd649c372ae92c33ae26d4c4a90dc84f257e549e6465b292e419a2a2ce9b97`
- Unchanged Vector header: `08547526705c7b2ee17679aaa95911d04a21a5fb85d886bfbfa8655b3a382a6e`
- Unchanged Byaml header: `3ceeca89571abeb56431268928af2c13cf412debf7bc751a0b46ffeeabf6a4c4`

The recipe includes canonical build/check instructions, every generated test
fixture, original-address candidate extraction through the unmodified checker,
all 56 preservation checks, replay logic and unconditional map restoration. Its
Python blocks were extracted and executed from the Markdown. All game bytes
remain local and ignored; no binaries are included in the branch.

Only the target's existing unnamed row was temporarily named fn_0024EC80.
Bounds 0024EC80..0024ED2C, pool 0024ED20, class f and every other symbol identity
were unchanged. The source, map/tool/configuration hashes and compiler
provenance are retained in ignored `build/actor_scale/final/report.json`.
Per-form diagnostics remain under `build/scale_form5` through `build/scale_form8`;
the complete final replay result is `build/actor_scale/replay_result.json`.

Work began 2026-10-01 at 21:59 UTC and shares a session with the tree packet.
These wall-clock windows are not separate per-root labor estimates.
