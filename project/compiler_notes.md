# Validated ARMCC matching notes

Curated for worker packets. Evidence baseline: `e2336037d4f709acc082b3322daedadf9500564a`. These are bounded observations, not universal compiler guarantees. Only the project's canonical checker awards rank O.

## Compiler and acceptance

- Game and al use ARMCC 4.1 build 791; CtrSDK uses 4.0 build 902. Preserve the configured module flags. The older unresolved-compiler paragraph in BRIEF is superseded by the recorded M0 result.
- M0 compared identical committed sources and headers under 791 and 894. Sensor lookup is 84 versus 80 bytes, effect-reference helper `fn_001C5A88` is 44 versus 40, and player helper `fn_001BB19C` is 40 versus 36. A compiler or linker error is not evidence that a version differs. Use the configured compiler; do not search compiler versions to rescue one function.
- Keep complete interval size separate from ELF instruction-symbol size. A source function can include a trailing literal pool. The StageSwitchKeeper example has 80 instruction bytes plus four pool bytes. Exact means the full unchanged mapped interval matches.

## Observed source patterns

- Dead initialization can change register allocation. For `al::calcHashCode`, removing a dead initial character load while retaining the loop assignment produced the target register allocation and all 44 bytes. Six equivalent forms were tried; only that form matched. A local left uninitialized is valid here only because every read follows the assignment. Never remove a required initialization to chase registers.
- Branch shape matters. The pilot's positive animation branch avoided unnecessary Boolean materialization. Ordinary C++ labels preserving a shared loop condition reproduced the 72-byte list setup. Preserve semantics, then change one grounded control-flow hypothesis at a time.
- Floating comparisons must preserve unordered behavior. The pilot's `isNearZero` needed the observed unordered comparison behavior; a seemingly equivalent comparison need not agree for NaN. Do not assume algebraic equivalence or finite-only inputs without evidence.
- Object and call ABI comes before expression tuning. The pilot corrected the pointer-list node value argument and the NerveKeeper transition order. A caller match does not establish a complete class layout or the original spelling of a reconstructed type.
- C1/C2 constructor names may be aliases for one physical section. CalendarTime and TreeNode examples select the compiler-emitted C1 section for a mapped C2 identity. Inspect the actual section and independently evidenced map metadata. Do not add duplicate source bodies or count aliases twice.
- A same-source helper may disappear during the configured linker inline phase. Acceptance requires provenance-checked source closure and elimination of the helper section. Unknown retail addresses and retained helper sections remain blockers. Do not synthesize an alias or provider to silence them.
- Check literals and existing row identities separately from instruction shape. Bubble's three remaining diagnostic differences were two scalar bytes and one singleton byte, not three scalar bytes. Correct a constant only from independent evidence. Keep game data and raw disassembly out of notes.

## How workers propose additions

Workers append a candidate pattern to their function's `.factory/facts/<ADDRESS>.md`: function/address, source revision, compiler/module flags, changed expression or declaration, observed full-interval result, canonical receipt if accepted, and limits or counterexamples. Keep proposals out of this curated file until the driver verifies the source and evidence. A failed form can document a local negative observation; it is not a validated positive pattern. New jobs receive this curated file in their packets. This file must remain read-only for workers.

## Sources

- [Configured compilers and flags](https://github.com/exgota/super-mario-3d-land-browser/blob/e2336037d4f709acc082b3322daedadf9500564a/data/config.json).
- [Three discriminator sources](https://github.com/exgota/super-mario-3d-land-browser/blob/e2336037d4f709acc082b3322daedadf9500564a/project/compiler_probe_functions.csv) and [decision record](https://github.com/exgota/super-mario-3d-land-browser/blob/e2336037d4f709acc082b3322daedadf9500564a/project/decisions.md): headings “Hash register allocation recovered in C++”, “M0 compiler milestone”, the StageSwitchKeeper literal-pool correction, the CalendarTime C1/C2 finding, and the TreeNode section correction.
- [Pilot observations and limits](https://github.com/exgota/super-mario-3d-land-browser/blob/e2336037d4f709acc082b3322daedadf9500564a/project/pilot_report.md). Its 40 of 50 result is a convenience sample from available source; it does not predict unnamed large-function matching.
- [Bubble identity and diagnostic corrections](https://github.com/exgota/super-mario-3d-land-browser/blob/e2336037d4f709acc082b3322daedadf9500564a/project/evidence/bubble_actor_family_identity.md).

The 41 percent register / 21 percent order failure breakdown from the advisory handoff is not adopted here: its underlying 47-run classification was not located in this review. No matching recipe or success rate is inferred from it.


## Owner-approved pinned source references, 2026-10-03

BRIEF rule 7 now permits leaked or unclear-provenance references and committed source. You may read the following local clones and their pinned public URLs. These are reference inputs, not acceptance evidence. Reuse existing shared headers and compare reference layouts with this target; never treat an upstream O label as a local match. Original data, assembly restrictions and the canonical checker remain unchanged. Full provenance: project/reference_sources.json and project/decisions.md.

- `/Users/exgota/super-mario-3d-land-factory/references/re_pepper_sead` at `c0f95fb9c697ffc5f82fcf0b4046459a82cb5fb6`: https://github.com/RE-Pepper/sead_ctr/tree/c0f95fb9c697ffc5f82fcf0b4046459a82cb5fb6
- `/Users/exgota/super-mario-3d-land-factory/references/re_pepper_ctrsdk` at `82b1d4e162d69b434afb6aa0c2638eab90d959e1`: https://github.com/RE-Pepper/ctrsdk/tree/82b1d4e162d69b434afb6aa0c2638eab90d959e1
- `/Users/exgota/super-mario-3d-land-factory/references/re_pepper_nintendo_ware` at `fafdae23115f077f61ba63f5f7f2b4f4f1539ca7`: https://github.com/RE-Pepper/NintendoWare_ctr/tree/fafdae23115f077f61ba63f5f7f2b4f4f1539ca7
- `/Users/exgota/super-mario-3d-land-factory/references/third_dimension_sead` at `67454e68a413dbc37911236322acbb3e517e5291`: https://github.com/3dsdecomp/sead/tree/67454e68a413dbc37911236322acbb3e517e5291
- `/Users/exgota/super-mario-3d-land-factory/references/third_dimension_nnsdk` at `d50a4b89d13c929d2438abf92e5a48f039efffbf`: https://github.com/3dsdecomp/nnsdk/tree/d50a4b89d13c929d2438abf92e5a48f039efffbf
- `/Users/exgota/super-mario-3d-land-factory/references/red_pepper_headers` at `f8329a61dc3b7e1f72127c2b77acac010a58b061`: https://github.com/3dsdecomp/RedPepper-Headers/tree/f8329a61dc3b7e1f72127c2b77acac010a58b061
- `/Users/exgota/super-mario-3d-land-factory/references/message_studio` at `8c4c16e9ab676713f1ddd4632172fa0dd231228e`: https://github.com/3dsdecomp/LibMessageStudio/tree/8c4c16e9ab676713f1ddd4632172fa0dd231228e
- `/Users/exgota/super-mario-3d-land-factory/references/red_pepper` at `6bd828b729f2442d18e7a586b6858a5fc6aeb6cd`: https://github.com/3dsdecomp/RedPepper/tree/6bd828b729f2442d18e7a586b6858a5fc6aeb6cd
