# Project state

Last updated: 2026-10-01, autonomous matching session.

## Final objective
M2: 100% byte-exact matching. The pilot has no review stop. Follow current project/BRIEF.md.

## Current milestone
M1 passes: all50 pilot functions have recorded outcomes and the report is committed. Phase2 matching and the runtime lane proceed. M0 remains one of three required compiler discriminators; M2 remains the final objective.

## Done
- EU executable hash confirmed. The original .3ds and target bytes remain untouched.
- Clean sead API headers and declaration-only SDK entry points enable all 37 Game and 75 lib/al sources in the normal build.
- check.py accepts O only after isolating source-generated object bytes, linking their relocations at original map addresses and comparing complete function intervals including literal pools. Unknown addresses, wrong sizes and changed bytes are rejected.
- Accepted coverage: 71 functions from canonical project ARMCC objects with committed source/header provenance, covering 3952 / 2,756,024 complete function bytes (0.143395%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup matches791 and fails 894. Final paired evidence lives under build/compiler_probe. The other leaf/lookup matches reproduce both compiler builds and do not settle M0.
- The refreshed pilot manifest is project/pilot_functions.csv: 20 small, 20 medium and 10 large or branch-heavy functions. All selected rows were unmatched at selection time.

## In flight
- Root integrates the new724-byte CourseSelectMap function and compiler-generated switch marker handling. Both compiler versions match diagnostically. Wanwan remains a provisional nonmatching draft under build/compiler_probe.
- sead_headers reconstructs SensorHitGroup add/remove and its verified layout. The FireBar, controller and thread-shutdown probes compile identically under both builds.
- compiler_candidates starts the runtime asset-loading lane: clean RomFS indexing and World1-1 resource discovery from the owner's dump. No gameplay or replay claim.
- Committed-source provenance is enforced. All53 O rows pass after the iterator/header change. The pilot ends with40 matches,1 NonMatching and9 abandoned fragments; see pilot_report.md and pilot_results.csv.

## Next tasks
1. Prove two more natural C++ functions under 791 and against894, without changing global flags or importing game instructions.
2. Match shared helpers and classes with verified layouts, starting with SensorHitGroup and the course-map update.
3. Load/index the owner's RomFS for the runtime and locate World1-1 assets. Continue until M2 or a current brief stop condition.

## Blockers and limits
- One compiler discriminator is insufficient for M0; do not report it complete.
- The normal compact main build links/exports and progress.py runs. It remains a scaffold with mapped placeholders, not a runnable game.
- Legacy progress.py counts map rows and matching words. Exact coverage is the sum of complete O function intervals, including literal pools.
- Source remains under Game/backup/src and backup/include; configuration names these existing paths.
- Historical attempt counts are measured lower bounds where earlier broad sweeps could not be recovered; pilot iterations must be recorded as they occur.
