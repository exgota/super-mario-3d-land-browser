# Project state

Last updated: 2026-10-01, autonomous Phase 2 and runtime session.

## Final objective
M2: 100% byte-exact matching. No pilot or review stop. Follow project/BRIEF.md.

## Current milestones
M0 passes three committed-source compiler discriminators. M1 passes all 50 pilot outcomes and the report. M2 remains the final goal. M3 gameplay/replay is unverified.

## Verified
- Original dump and EU executable hashes remain unchanged. No game data is committed.
- Normal ARMCC build compiles 42 Game and 85 al sources plus the clean SDK unit. The compact main links/exports; it remains a scaffold.
- Accepted coverage: 197 functions from canonical project ARMCC objects with committed source/header provenance, covering 14784 / 2,756,024 complete function bytes (0.536425%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup, fn_001C5A88 and Game fn_001BB19C pass791 and fail894. M0 evidence: build/game_compiler_check/evidence.json.
- Pilot: 40 matched, 1 NonMatching, 9 abandoned. See pilot_report.md, pilot_results.csv and pilot_iterations.csv. Phase 2 continues.
- Runtime verifies 73,421 RomFS IVFC blocks and 50 selected World1-1 files. Native reader validates 298 placements, 3853 collision prisms, 39 models, 137 mesh/material bindings, 127 materials, raw skeleton/vertex/index fields and serialized topology. No level has run.
- Native math matches 3422 direct retail float bits in debug/O3. Shape uniform transfer and command-copy gate interfaces pass 822 native words and 548 original gate cases. Shader meaning, rendering and gameplay remain unverified.

## Lanes and acceptance queue
- Root alone owns dot intake, canonical acceptance, map, ledger, shared docs, Git and origin pushes. Other matching lanes work disjoint eligible small/medium U families; runtime continues. Maximum 8 lanes.
- All 24 exact claims from eight dot proposals pass locally, 3700 bytes total. Latest proposals add placement144, CourseList120 and ExecuteDirector1012. Heap adds zero exact bytes. All source/header/report intake is reviewed; no dot map/rank/ledger/tool changes are imported.
- All 33 execution/helper candidates and 21 actor-group candidates pass locally. The prior full shared-header recheck passed126/126 at8980 bytes. New source-specific acceptance covers later roots; repeat the full check after the queued local executor header changes.
- Guarded sensor execute is rank m, with810 bounded ARM11 pairs recorded separately as NonMatching. Placement initializer passes76 contract-bounded pairs; heap validation reports410 pairs, including30 fault pairs. Root still needs to review/freeze those latter reports and record bounded outcomes.
- Ready queues: local executor8 remaining functions/564 bytes plus seven CPP/header corrections; Byaml2/228 and placement6/420; effect deletion3/316; effect lookup/deletion4/388; pose access12/184; independent NerveStateBase data repair/ctor32; updateCollider168.
- Fresh matching lanes: actor clipping/collider helpers; ActorInitInfo/link readers; actor pose mutations; request-queue drains; effect emission; KeyPoseKeeper helpers. Preserve their minimal patches and independently evidenced identities. Do not edit production during root's source-frozen checks.
- Active dot branches reserve blocked.md entries, unanswered packets and U functions >=0x400 bytes. Fetch every few hours. Seven hard-function packets are committed; Pro relay is paused.

## Next three tasks
1. Review/accept the local executor follow-on source/header package, verify affected constructors and all189 accepted roots, preserving the six already credited dot functions without duplicate ledger rows.
2. Accept queued Byaml/placement, effect and pose families in small source-frozen checkpoints. Review bounded placement/heap reports without broad gameplay claims. Keep main linking/exporting, STATE and private-origin pushes current.
3. Continue native World1-1 resource/runtime work alongside matching. Next runtime slice is resource-local texture reference identity after the verified draw-transfer checkpoint.

## Blockers and measurement limits
- Independently repaired virtual-table data ownership is separate from source/rank acceptance. Compact main uses verified weak zero-filled imports for whole U tables and named BSS. These placeholders do not reconstruct game behavior.
- Unmapped source helpers require provenance-checked inline closure and must disappear fully. No guessed standalone address is allowed.
- Legacy progress last reports168 matches,7 Non-matching and12504/3,092,336 word-similarity bytes. Strict function coverage is measured independently. Run it after the next source checkpoint.
- Ledger times include check-only rows and shared measured script windows. Related windows overlap and exclude unmeasured preparation; they are not independent person-hours. No cost tracking.
- Runtime lacks actor overrides, complete graphics/material decoding, rendering, game execution and replay. Three nested layout archives remain opaque.
- Stop only under current BRIEF conditions. Latest accepted functions keep the six-hour/100-attempt stall condition clear.
