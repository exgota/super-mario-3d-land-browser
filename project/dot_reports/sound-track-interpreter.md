# Sound track command interpreter: nonmatching proposal

The complete raw root `003409FC..0034129C` is reconstructed as ordinary C++ in `lib/al/src/Audio/SoundTrackInterpreter.cpp`. It remains guarded by `NON_MATCHING`. The final checker rejects its complete 2236-byte section against the unchanged 2208-byte original interval. **Zero exact roots and zero exact bytes.**

The final source passes 9,616 original-versus-compiled whole-root cases and four bounded fault/cycle controls. The final clean project build links. Preservation is equivalence-backed against the coordinator's previously passing 752-root/772-definition baseline; this lane did not run another 772-check audit.

## Frozen scope

- Base: `bc5b236802b0300bc6b00f0ac55cf5ae5749c55c`
- Branch: `dot/sound-track-interpreter`
- Final source commit: `ee81bda172172b51508f825b0923a2c0abb13087`
- Source SHA-256: `f428d3e9d6da80da4a44b5f220d429fe40d15123f6dfbc55ceb45f35f1edd69a`
- Final ARMCC object SHA-256: `fab485d30b29bacc7671857c2f63bb8bed4031eb4a3503c03ae8e4f880826cde`
- Diagnostic relocated section SHA-256: `6293a37da463556aea461353423b0c7e0a2d53d6affef969f6b36a4f027ced4b`
- No shared source/header changes. No data definitions, copied tables, invented helper addresses, assembly, padding adjustments, flags changes, or map-boundary/Type changes
- No binary data is committed. The complete private EU input still hashes to `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`

The coordinator's queue screen reported 105 worktrees/967 distinct relevant contents with no prior implementation. Active source families were checked before assignment. This is the first work on this raw root, distinct from command dispatcher `002BECC8` and sound-start root `0022B6E8`. Their clean proposal declarations were read for compatibility. Only `fn_002C6348(void*, int)` overlaps; its declaration agrees exactly.

`lib/al/src/Audio` follows the existing neutral audio proposal storage convention. The sound-sequence role follows the original callers and track/owner state, not an assumed original library or class name. The configured module uses ARMCC 4.1 build 791. The original compiler build is not established for this root. No 894 tool was installed, and neither another compiler nor special flags were used to rescue it.

## Independent ABI and ownership evidence

Constructor `002BD4D4` stores `003D9090` at its larger object's `+0x98`, and stores the address of that member at `+0x78`. The existing, unchanged 16-byte data row `003D9090..003D90A0` begins with addresses `003409FC`, `0034129C`, then two zero words. This lane defines none of that table.

Wrapper `003412C4` loads a handler from track `+0xc8` and passes it to parser `003412D8`. Calls at `00341530`, `00341614`, `003416C8`, and `003417F8` load handler table slot zero. They pass `(handler, track, unsigned command, argument, extra)`, with `extra` on the stack. `003417D0` sign-extends the extra operand to 16 bits before the extended-command dispatch. Branch/call offsets use decoded 24-bit values. The root ignores the handler argument. Parser `00341464` uses table slot one for a separate note operation; it is not part of this implementation.

The root takes its state at track `+0x1c` and owner pointer at `+0xc0`. State construction at `002C3150` and initialization at `002C3000` independently confirm the four six-byte ramp records at track `+0x6c/+0x72/+0x78/+0x7e`, the script positions, and the parameter bytes. The local `Track` type is only a `0xc4`-byte prefix used by this root, not a complete class: actual instances also have the voice chain at `+0xc4` and decoder pointer at `+0xc8`.

Variable accessor `00214928` selects owner values at `+0xc8` for indices below 16, or the original global array at `00425C28` for indices 16..31. Accessor `00228C94` selects track values at `+0xa0` for indices below 16. Root indices 32..47 subtract 32 before the latter call. Debug command `0xd6` is gated by the existing single-byte row `003F152D..003F152E`; the declaration imports that exact byte and supplies no replacement data.

`002C6690` independently builds the callback descriptor with owner/global/track value pointers and a byte at offset 12; after the callback it copies that byte back into state `+8`. `002C2810` writes base/current script pointers; `002C2BE4` starts the selected secondary track. `002C6348` is the original signed-index owner track lookup. `002C2F20` is a separate voice-control helper, not a fake extension of this root. All these functions remain external.

## Complete interval and source forms

The blank Pool column does not mean this interval is all instructions. Four jump-table intervals are `340AF0..340B1C`, `340B40..340B70`, `340B98..340BB0`, and `3410D0..3410F4`, totaling 152 bytes. Literal islands `340E04..340E14` and `34116C..341174` total 24 bytes. All 176 data bytes remain inside the unchanged 2208-byte function interval. There are 508 four-byte executable instruction words.

1. Commit `1a354ef`: full typed-state switch reconstruction, including ramps, stack commands, variable arithmetic/comparisons, callback, track start, and original imports. The normal project build produces 2160 bytes. The unchanged checker rejects complete section size. 5,987 replay cases and the four controls agree; shift tests were restricted to a smaller count range
2. Commit `ee81bda`: a semantic correction makes variable shifts defined across the caller's signed-16 operand domain. Original `3411C0..3411D4` uses ARM register shifts, whose count is the low eight bits; counts at least 32 need explicit zero/sign-fill handling in C++. Unsigned left-shift intermediates also avoid signed overflow. The normal project build produces 2236 bytes, 28 longer than the original. The checker again rejects complete section size. The widened final replay passes 9,616 cases

No cosmetic/register-only forms were tried. There were two meaningful unsuccessful source forms, below the four-form cap. The final form is retained for its explicit operand semantics. No further grounded source hypothesis is currently established; the root remains parked and nonmatching.

Every source form was committed before its normal `python make.py eu` build and unchanged `tools/check.py fn_003409FC --object build/eu/obj/lib/al/src/Audio/SoundTrackInterpreter.o` check. For these checks only, the existing root row's blank Symbol field was temporarily spelled `fn_003409FC`. A `finally` block restored the exact original map bytes, including its U rank; final map SHA-256 is `350611b6caf9bdba44895628edcf7955b53964976f621613198589760d9217e4`. Neither boundaries nor Pool nor Type were altered.

Final check output, return code 1:

```text
U -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

The canonical check reached size validation after resolving the source imports. It did not approve a canonical relocated match. A separate diagnostic link relocates the unchanged full compiler object at `003409FC` with the genuine map imports; it is not an acceptance bypass and earns no credit. Its larger output occupies 28 bytes beyond the original interval in the separate replay image. No original input file is edited, and replay does not call that overwritten adjacent wrapper.

## Replay and explicit limits

Final replay takes 206.158128 seconds. It compares 9,616 ordinary complete-root invocations, all of which return, preserve SP and callee-saved integer/VFP registers, and agree on complete heap/global memory digests, callback/import arguments, and FPSCR. Both maximum observed stack depths are 120 bytes. R0 is void and caller-saved register residue is deliberately not compared.

Coverage includes every byte opcode, every extended low-byte operation, variable scopes and invalid high indices, signed arithmetic extrema, all three stack depths plus the full-stack guard, loop counts 0/1/2/255, nested return/loop frame kinds, signed and unsigned ramp interpolation, tempo/program boundaries, debug gating, secondary track self/null/other cases, all voice-control modes, and four FPSCR controls for conversions. Shift tests include every possible low-eight-bit register count in both directions, several variable values, and signed-16 extremes. RNG tests execute the original generator with boundary seeds and positive/negative bounds.

All 12 direct imported callees execute the actual original code, including the original division implementation and RNG. Voice helpers traverse real two-node synthetic lists; original downstream `00229300`, `0022937C`, `00229434`, and `00229744` execute on bounded inactive/no-device voice states. The sole model is an external callback at the harness address `00700100`. It reads the real descriptor, changes its three pointed-to arrays, changes the descriptor flag, and records its argument. Callback return-to-state copying therefore executes in the original `002C6690`.

The original fetch coverage is 505/508 instruction words, including condition-failed ARM instructions. Unfetched returns `340AEC`, `340B3C`, and `340B94` are statically unreachable switch-table defaults: enclosing comparisons already restrict indices to 0..10, 0..11, and 0..5 respectively. No inline table/literal bytes count as executable coverage.

Three invalid-memory controls agree on fault class/address, state, and global changes: an invalid track, an invalid owner in a parameter store, and an invalid owner variable accessor. A cyclic two-node voice-list control reaches a 3000-instruction budget on both images with equal observed heap/globals and no fault. This is a bounded nontermination observation, not a general cycle/liveness proof; traces and instruction counts are not equated for a budget-limited cycle.

This is compiled ARM differential evidence, not a portable C++ sanitizer proof or arbitrary-memory proof. Track/owner/secondary-track identities are distinct; arbitrary aliasing is untested. Inputs retain caller-derived operand widths except explicitly labeled boundary controls. Valid script pointer provenance and normal ramp object invariants remain caller obligations. Actual audio-device paths, concurrent callbacks, full script playback, sound rendering, and gameplay are unverified.

## Clean build and preservation

Final `python make.py eu -ca` returns zero and links in 42.609022 seconds. Because the root remains U, the compact linked image retains its generated stub; that normal image does not link this source implementation. Actual source execution above uses the diagnostic object link.

The baseline is the coordinator's clean `mario-mainbc5/build/dot-baseline-bc5/report.json`, SHA-256 `faf671be7c53bc783dd4a09301a1b9eae3f38034f900e357d2357d91f92bed4f`, which passed 752 roots and all 772 actual definitions in 609.803395 seconds.

This lane verifies all 179 old canonical C++ objects, their allocated sections, every symbol, relocation, ARM attributes, normalized build provenance, and all 372 prior input hashes against that baseline. Only workspace prefixes in 179 STT_FILE names/build command arguments are normalized. There are zero object/input differences. This comparison includes eight objects with no definitions in the baseline checker set: PlayerFunction, SceneObjFactory, ProductStateStage, StageProgressAccessors, Application, alActorExecutionHooks, alStageSwitchTypeCount, and alByamlIteratorConstructors. It also compares all 470 old tracked Game/lib/tools/config/map blobs and the unchanged asm-differ gitlink. Zero old tracked inputs differ. The final build has 180 C++ objects: the 179 originals plus this new TU.

Preservation is therefore equivalence-backed. It is explicitly not a new 772-check run. All new exact inventory remains zero.

## Frozen evidence and reproduction

Structured evidence and every source/tool/compiler/check/replay/snapshot hash are in `sound-track-interpreter-evidence.json`. The committed `sound-track-interpreter-reproduction.md` contains all five exact tested scripts, verified extraction instructions, and complete build/check/replay/preservation commands. Extracted script bytes were verified identical to the frozen tested files; this notes-only publication fix leaves source/object unchanged and does not claim another test run. Local diagnostic files remain under `build/dot-sound-track/`: `run-check.py`, `prepare.py`, `replay.py`, `snapshot.py`, `preservation.py`, full check/build logs, both attempt results, complete original text disassembly, final replay controls, and object snapshots. Private binary/object artifacts remain ignored and local.

Read the scripts before rerunning. Use `. ./development_environment.sh` first; the cloud environment then sets `DEVKITARM=/usr`. Run the project's normal build. `run-check.py final` performs the single temporary spelling and restores the exact map. `prepare.py final` verifies normal-build provenance and creates the explicitly noncanonical diagnostic link. `replay.py` runs the original/compiled comparison without replacing original callees. `snapshot.py` and `preservation.py` compare full old object and input sets against the immutable coordinator baseline. No test tool or oracle was edited.

The session window from assignment at 04:33:07 UTC to final source/replay/preservation completion at 04:51:19 UTC on 2026-10-02 is 1092 seconds (18.2 minutes). It includes ABI investigation, two forms, builds, checker failures, replay, and preservation. Report sealing follows that window. Accepted complete bytes/hour for the window is zero.
