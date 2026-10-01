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
- Accepted coverage: all 52 functions revalidated from canonical project ARMCC objects with committed source and header provenance. They cover 2892 / 2,756,024 function bytes, including literal pools (0.104933%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup matches791 and fails 894. Final paired evidence lives under build/compiler_probe. The other leaf/lookup matches reproduce both compiler builds and do not settle M0.
- The refreshed pilot manifest is project/pilot_functions.csv: 20 small, 20 medium and 10 large or branch-heavy functions. All selected rows were unmatched at selection time.

## In flight
- Wanwan source-only reconstruction at 0x0030D024 is ready for root integration. Its two compiler builds differ naturally but neither passes the target check.
- Compact main-link repair, sead_headers agent. The FireBar, controller and thread-shutdown probes compile identically under both builds.
- New hard rule 5 is enforced and all 52 prior O rows pass. Copied objects, uncommitted source and an incorrect recorded object hash reject without changing ranks. The compiler proof also requires its primary result from the verified project object. Pilot initial checks cover 50 / 50 functions. Nine unsupported large fragments are parked with ledger/blocked evidence; ByamlHashIter remains NonMatching after eight iterations. Setup matches diagnostically and awaits the normal build check.

## Next tasks
1. Prove two more natural C++ functions under 791 and against894, without changing global flags or importing game instructions.
2. Run the balanced50-function pilot, cap each at8 iterations, write the report and continue directly into Phase2.
3. Scale matching and start the runtime lane after the pilot. Continue until M2 or a current brief stop condition.

## Blockers and limits
- One compiler discriminator is insufficient for M0; do not report it complete.
- The compact main link currently fails on retained M-function dependencies. A lane is repairing retention and verified address aliases. Exact per-function links do not make a runnable port.
- Source remains under Game/backup/src and backup/include; configuration names these existing paths.
- Historical attempt counts are measured lower bounds where earlier broad sweeps could not be recovered; pilot iterations must be recorded as they occur.
