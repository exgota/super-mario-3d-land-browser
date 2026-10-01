# Project state

Last updated: 2026-10-01, autonomous Phase 2 and runtime session.

## Final objective
M2: 100% byte-exact matching. No pilot or review stop. Follow project/BRIEF.md.

## Current milestones
M0 passes three committed-source compiler discriminators. M1 passes all 50 pilot outcomes and the report. M2 remains the final goal. M3 gameplay/replay is unverified.

## Verified
- Original dump and EU executable hashes remain unchanged. No game data is committed.
- Normal ARMCC build compiles 42 Game and 85 al sources plus the clean SDK unit. The compact main links/exports; it remains a scaffold.
- Accepted coverage: 273 functions from canonical project ARMCC objects with committed source/header provenance, covering 17556 / 2,756,024 complete function bytes (0.637005%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup, fn_001C5A88 and Game fn_001BB19C pass791 and fail894. M0 evidence: build/game_compiler_check/evidence.json.
- Pilot: 40 matched, 1 NonMatching, 9 abandoned. See pilot_report.md, pilot_results.csv and pilot_iterations.csv. Phase 2 continues.
- Runtime verifies 73,421 RomFS IVFC blocks and 50 selected World1-1 files. Native reader validates 298 placements, 3853 collision prisms, 39 models, 137 mesh/material bindings, 127 materials,217 direct texture references, raw skeleton/vertex/index fields and serialized topology. No level has run.
- Native math matches 3422 direct retail float bits in debug/O3. Shape uniform transfer and command-copy gate interfaces pass 822 native words and 548 original gate cases. Shader meaning, rendering and gameplay remain unverified.

## Lanes and acceptance queue
- Root alone owns dot intake, canonical acceptance, map, ledger, shared docs, Git and origin pushes. Other matching lanes work disjoint eligible small/medium U families; runtime continues. Maximum 8 lanes.
- All 24 exact claims from eight dot proposals pass locally, 3700 bytes total. Latest proposals add placement144, CourseList120 and ExecuteDirector1012. Heap adds zero exact bytes. All source/header/report intake is reviewed; no dot map/rank/ledger/tool changes are imported.
- All33 execution/helper candidates,21 actor-group candidates and eight executor follow-ons pass locally. The latest full shared-header recheck passes197/197 at14784 bytes; frozen evidence is build/executor_header_recheck_frozen.json.
- Guarded sensor execute is rank m with810 bounded ARM11 pairs. Placement initialization agrees on76 contract-bounded pairs. Heap creation agrees on380 returning pairs and30 fault pairs. All three bounded NonMatching reports and ledger outcomes are frozen separately from exact coverage.
- Ready queues: Byaml2/228, placement6/420, initialization/link6/404 and typed readers2/228; actor state7/264; effect deletion3/316, lookup4/388 and emission1/120; request drains3/316; KeyPose5/136; independent NerveStateBase data repair/ctor32; updateCollider168.
- Fresh matching lanes: actor clipping/collider helpers; ActorInitInfo/link readers; actor pose mutations; request-queue drains; effect emission; KeyPoseKeeper helpers. Preserve their minimal patches and independently evidenced identities. Do not edit production during root's source-frozen checks.
- Active dot branches reserve blocked.md entries, unanswered packets and U functions >=0x400 bytes. Fetch every few hours. Seven hard-function packets are committed; Pro relay is paused.

## Next three tasks
1. Build/check the composed Byaml/placement16-family checkpoint and recheck affected existing accepted source roots. Keep other lanes matching fresh eligible small/medium functions.
2. Accept queued Byaml/placement, effect and pose families in small source-frozen checkpoints. Keep main linking/exporting, STATE and private-origin pushes current.
3. Continue native World1-1 resource/runtime work alongside matching. Texture identity is committed. Continue bounded texture payload/header decoding with explicit unsupported layouts.

## Blockers and measurement limits
- Independently repaired virtual-table data ownership is separate from source/rank acceptance. Compact main uses verified weak zero-filled imports for whole U tables and named BSS. These placeholders do not reconstruct game behavior.
- Unmapped source helpers require provenance-checked inline closure and must disappear fully. No guessed standalone address is allowed.
- Legacy progress last reports247 matches,7 Non-matching and15124/3,092,336 word-similarity bytes. Strict function coverage is measured independently. Run it after the next source checkpoint.
- Ledger times include check-only rows and shared measured script windows. Related windows overlap and exclude unmeasured preparation; they are not independent person-hours. No cost tracking.
- Runtime lacks actor overrides, complete graphics/material decoding, rendering, game execution and replay. Three nested layout archives remain opaque.
- Stop only under current BRIEF conditions. Latest accepted functions keep the six-hour/100-attempt stall condition clear.
