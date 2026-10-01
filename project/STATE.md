# Project state

Last updated: 2026-10-01, first autonomous model session.

## Current milestone
M0: toolchain settled. Compiler discrimination remains unresolved.

## Done
- Original EU executable hash verified again.
- Strict byte comparison, including literal pools, now gates `O`; a generated-output mutation confirmed rejection. The differ and target remain unchanged.
- All 37 Game and 75 lib/al sources compile, archive, link and export in the normal build with unchanged global flags.
- Six PlayerTrigger functions pass check.py. PlayerAnimFrameCtrl::getCurrentFrame also passes. PlayerActionCondition::setup also passes. al::calcHashCode also passes. The sensor-name lookup also passes. The photo scenario lookup also passes. Exact coverage: 12 functions, 364 / 2,756,024 mapped function bytes, including literal pools.
- One pure C++ lookup matches under791 and fails894. M0 requires three; the final production paired evidence is being recorded.

## In flight
- Original-address strict checking is integrated and validated.
- Isolated compiler probes under build/compiler_probe, by compiler_candidates.
- Placement-category lookup integration, by root; thread shutdown candidate0x00255E78 and parameter-controller candidate0x00199CA4 in parallel.

## Next tasks
1. Integrate fixed-address checking for call and data relocations without importing game bytes.
2. Find at least three true Game functions that match under one compiler and fail under the alternative. Simple leaves match both, so they do not satisfy M0.
3. After M0, select the balanced 50-function pilot and stop for owner review after its report.

## Blockers
- Game source is under backup/src and backup/include; configuration points there explicitly.
- The compact build remains a stub scaffold. Strict per-function links now verify relocations faithfully; this is not a runnable port.
- Spend estimates are unavailable from this subscription session. Ledger token/time values are coarse estimates, not billing measurements; no zero-cost claim is made.
