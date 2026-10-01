# Project state

Last updated: 2026-10-01, autonomous matching session.

## Final objective
M2: 100% byte-exact matching. The pilot has no review stop. Follow current project/BRIEF.md.

## Current milestone
M0: settle the game compiler. One of three required discriminating functions is proved.

## Done
- EU executable hash confirmed. The original .3ds and target bytes remain untouched.
- Clean sead API headers and declaration-only SDK entry points enable all 37 Game and 75 lib/al sources in the normal build.
- check.py accepts O only after isolating source-generated object bytes, linking their relocations at original map addresses and comparing complete function intervals including literal pools. Unknown addresses, wrong sizes and changed bytes are rejected.
- Exact mapped coverage: 26 functions, 860 / 2,756,024 function bytes, including literal pools. Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup matches791 and fails 894. Final paired evidence lives under build/compiler_probe. The other leaf/lookup matches reproduce both compiler builds and do not settle M0.
- The refreshed pilot manifest is project/pilot_functions.csv: 20 small, 20 medium and 10 large or branch-heavy functions. Every selected row is currently unmatched.

## In flight
- Wanwan initialization at 0x0030D024, compiler_candidates agent. Its two compiler builds differ naturally; strict matching remains in progress.
- FireBar constructor at 0x00310588, sead_headers agent. The previous parameter-controller probe compiled identically under both builds.
- Root runs the fixed pilot sample. Initial strict checks completed: 1 / 50. Both other lanes continue M0.

## Next tasks
1. Prove two more natural C++ functions under 791 and against894, without changing global flags or importing game instructions.
2. Run the balanced50-function pilot, cap each at8 iterations, write the report and continue directly into Phase2.
3. Scale matching and start the runtime lane after the pilot. Continue until M2 or a current brief stop condition.

## Blockers and limits
- One compiler discriminator is insufficient for M0; do not report it complete.
- The compact main build is a stub scaffold. Exact per-function links do not make a runnable port.
- Source remains under Game/backup/src and backup/include; configuration names these existing paths.
- Historical attempt counts are measured lower bounds where earlier broad sweeps could not be recovered; pilot iterations must be recorded as they occur.
