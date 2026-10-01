# Project state

Last updated: 2026-10-01, first autonomous model session.

## Current milestone
M0: toolchain settled. Compiler discrimination remains unresolved.

## Done
- Original EU executable hash verified again.
- Strict byte comparison, including literal pools, now gates `O`; a generated-output mutation confirmed rejection. The differ and target remain unchanged.
- All 35 Game and 75 lib/al sources compile, archive, link and export in the normal build with unchanged global flags.
- Six PlayerTrigger functions pass check.py. PlayerAnimFrameCtrl::getCurrentFrame also passes. PlayerActionCondition::setup also passes. al::calcHashCode also passes. Total: 10 exact functions, 204 / 3,092,336 code bytes.
- ARMCC 4.1/791 and 4.1/894 both reproduce those six functions. Neither has been proved to be the game compiler.

## In flight
- Original-address relocation proof, by the sead_headers agent.
- Isolated compiler probes under build/compiler_probe, by compiler_candidates.
- Incremental integration and missing global declarations, by the root agent.

## Next tasks
1. Integrate fixed-address checking for call and data relocations without importing game bytes.
2. Find at least three true Game functions that match under one compiler and fail under the alternative. Simple leaves match both, so they do not satisfy M0.
3. After M0, select the balanced 50-function pilot and stop for owner review after its report.

## Blockers
- Game source is under backup/src and backup/include; configuration points there explicitly.
- Missing original-address relocation in the compact stub link will prevent strict raw matches for external calls. A faithful link solution is needed before claiming those functions.
- Spend estimates are unavailable from this subscription session. Ledger token/time values are coarse estimates, not billing measurements; no zero-cost claim is made.
