# Project state

Last updated: 2026-10-01, autonomous Phase2 and runtime session.

## Final objective
M2: 100% byte-exact matching. No pilot or review stop. Follow project/BRIEF.md.

## Current milestones
M1 passes: all50 pilot outcomes and the report are committed. M0 passes three of three committed-source compiler discriminators. M2 remains the final goal. M3 gameplay/replay is unverified.

## Verified
- Original dump and EU executable hashes remain unchanged. No game data is committed.
- Normal ARMCC build compiles41 Game and85 al sources plus the clean SDK priority unit. The compact main links/exports and unchanged progress.py runs. It remains a scaffold.
- Accepted coverage: 126 functions from canonical project ARMCC objects with committed source/header provenance, covering 8980 / 2,756,024 complete function bytes (0.325832%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup, fn_001C5A88 and Game fn_001BB19C pass791 and fail894. M0 passes; paired canonical-primary evidence is build/game_compiler_check/evidence.json.
- New accepted classes/helpers include SensorHitGroup, NerveExecutor, LayoutActor, typed Byaml child lookup, EffectKeeper and the724-byte course-selection update. Only check.py setsO.
- The pilot has40 matched,1 NonMatching and9 abandoned fragments. See pilot_report.md, pilot_results.csv and pilot_iterations.csv. Phase2 continues.
- Runtime tools verify all73,421 RomFS IVFC blocks and50 selected World1-1 files. Native C++ reader independently validates placement bits,3853 collision prisms and39 model/145 texture catalogs, with sanitizer and damaged-input checks. No level has run.

## Lanes and acceptance queue
- Root owns source intake, canonical acceptance, map, ledger, shared documentation and private-origin pushes. All111 existing accepted functions pass the full committed-source recheck after the shared header checkpoint and restored LiveActorFlag constructor. Evidence: build/execution_checkpoint_recheck.json.
- Dot branches are active. Four source proposals are next: hit-sensor-director (constructor364 bytes plus guarded execute), fugumannen-init (5 functions676 bytes), area-cube (180 bytes), togezo-init (8 functions1204 bytes). Reviewers check the minimal identities/layouts; root verifies every function locally. Execute-director has no additional commits. Reserve blocked functions and U functions >=0x400 bytes for dot.
- Runtime lane validates raw model attributes and skeleton fields. Native math now agrees on3422 direct float-bit comparisons. Resource-local mesh decoding covers39 models,137 shapes,50076 vertices and133872 indices; game rendering and replay remain unverified.
- Nerve lane probes small ExecuteRequestKeeper helpers in scratch. Ready local queues include34 execution/helper candidates,21 actor-group helpers, Byaml closures and placement readers,12 executor helpers,3 effect routines and independently evidenced NerveStateBase data ownership. Accept in small source-frozen batches after dot.
- Other matching lanes review dot proposals or preserve their ready evidence. Maximum8 lanes; xhigh small functions, ultra large functions/layouts. Canonical build inputs freeze across commit/build/check.

## Next three tasks
1. Integrate reviewed dot source/header/report proposals and necessary independent names/data registrations. Commit source, build normally, then accept each function with explicit dot credit.
2. Accept local execution/helper and actor-group packages in small batches, followed by Byaml/placement and executor packages. Keep main linking/exporting and STATE current.
3. Continue native World1-1 resource/runtime work alongside matching. Fetch dot branches every few hours and push main at least hourly.

## Blockers and measurement limits
- Independent ABI/caller evidence repaired NerveExecutor/LayoutActor and LiveActor data ownership in separate commits. The compact scaffold imports whole unaccepted mapped virtual tables as zero-filled weak data, keeping unknown/shared data conservative. This is scaffold behavior, not reconstructed game execution.
- Late-inlined Byaml constructors have no established standalone addresses. The accepted stage routine now uses provenance-checked source-generated helpers; no guessed helper import is allowed.
- Legacy progress counts all map rows and matching words. Its refreshed similarity fell to1112/3,092,336 bytes after compact relayout; exact coverage increased independently.
- Ledger times explicitly mix canonical-check-only rows and measured shared preparation windows. Windows overlap between functions and exclude earlier inspection where unmeasured. They are not isolated person-hours.
- Runtime still lacks actor overrides, full graphics decoding/rendering, game execution and replay. Three nested layout archives remain opaque to inspection.
- Stop only under current BRIEF conditions. Latest accepted functions keep the six-hour/100-attempt stall condition clear.
