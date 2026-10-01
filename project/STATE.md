# Project state

Last updated: 2026-10-01, first autonomous model session.

## Current milestone
M0: toolchain settled. Compiler discrimination remains unresolved.

## Done
- Original EU executable hash verified again.
- Strict byte comparison, including literal pools, now gates `O`; a generated-output mutation confirmed rejection. The differ and target remain unchanged.
- Incremental source selection enables Game/backup/src/Player/PlayerTrigger.cpp and lib/al/src/Math/alHashUtil.cpp with unchanged global flags.
- Six PlayerTrigger functions pass check.py. Total: 7 exact functions, 160 / 3,092,336 code bytes.
- ARMCC 4.1/791 and 4.1/894 both reproduce those six functions. Neither has been proved to be the game compiler.

## In flight
- Clean sead headers from local EU assembly, by the sead_headers agent.
- Isolated compiler probes under build/compiler_probe, by compiler_candidates.
- Incremental integration and missing global declarations, by the root agent.

## Next tasks
1. Finish clean header integration and compile all existing Game and lib/al source files.
2. Find at least three true Game functions that match under one compiler and fail under the alternative. Simple leaves match both, so they do not satisfy M0.
3. After M0, select the balanced 50-function pilot and stop for owner review after its report.

## Blockers
- Game source is under backup/src and backup/include; configuration points there explicitly.
- Missing original-address relocation in the compact stub link will prevent strict raw matches for external calls. A faithful link solution is needed before claiming those functions.
- Spend estimates are unavailable from this subscription session. Ledger token/time values are coarse estimates, not billing measurements; no zero-cost claim is made.
