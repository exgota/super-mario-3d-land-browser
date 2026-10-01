# Project state

Last updated: 2026-10-01, autonomous Phase 2 and runtime session.

## Final objective
M2: 100% byte-exact matching. No pilot or review stop. Follow project/BRIEF.md.

## Current milestones
M0 passes three committed-source compiler discriminators. M1 passes all 50 pilot outcomes and the report. M2 remains the final goal. M3 gameplay/replay is unverified.

## Verified
- Original dump and EU executable hashes remain unchanged. No game data is committed.
- Normal ARMCC build compiles 42 Game and 93 al sources plus the clean SDK unit. The compact main links/exports; it remains a scaffold.
- Accepted coverage: 393 functions from canonical project ARMCC objects with committed source/header provenance, covering 22776 / 2,756,024 complete function bytes (0.826408%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup, fn_001C5A88 and Game fn_001BB19C pass791 and fail894. M0 evidence: build/game_compiler_check/evidence.json.
- Pilot: 40 matched, 1 NonMatching, 9 abandoned. See pilot_report.md, pilot_results.csv and pilot_iterations.csv. Phase 2 continues.
- Runtime verifies 73,421 RomFS IVFC blocks and 50 selected World1-1 files. Native reader validates 298 placements, 3853 collision prisms, 39 models, 137 mesh/material bindings,127 materials and217 direct texture references. No level has run.
- Native math matches3422 direct retail float bits in debug/O3. All145 texture headers/extents agree with bytes and226 bounded original consumer cases. Shape transfer/copy interfaces pass822 native words and548 original gate cases.
- RGB565 storage passes37824 packed words,113472 channel integers and all75648 bytes across8 images/11 supported mip levels. Independent traversal,65536 word fixtures,6 tile patterns,7 unsupported/cache cases and6 malformed cases pass. Frozen manifest: data/runtime/romfs/native_rgb565_frozen/command_result_manifest.json.

## Lanes and acceptance queue
- Root alone owns dot intake, canonical acceptance, map, ledger, shared docs, Git and origin pushes. Seven other lanes continue disjoint matching/runtime work. Maximum8 lanes.
- All24 exact claims from eight earlier dot proposals pass locally,3700 bytes total. New dot/effect-action-update and dot/item-type-lookup proposals await root review. No dot map/rank/ledger/tool edits are imported directly.
- Full shared-header recheck passes197/197 at14784 bytes, frozen in build/executor_header_recheck_frozen.json. Later source-only checkpoints preserve affected previous roots: Byaml12, actor/execution33, effect4 and Scene7. No later shared-header edits.
- Latest reaction/placement/camera/Scene/Nerve38 checkpoint passes1032 bytes; stage/area/collision/effect-interface/NerveBase13 passes396 bytes. Independent NerveStateBase table ownership repair is committed separately as d5d925f.
- Ready queue: updateCollider168; StageSwitchKeeper constructor84 with source-generated inline closure; Byaml numeric/rail3 with176 bytes; Matrix34 makeST84. Scratch proposals count only after canonical checks.
- Matching lanes continue collision/actor helpers, ActorInitInfo/link readers, actor-group helpers, math/layout work, effects and stage switches. Runtime inspects ETC1 storage independently. Production remains frozen during canonical checks.
- Guarded sensor execute records810 bounded ARM11 pairs, placement initialization76 and heap creation380 returning/30 fault pairs. These frozen NonMatching results add zero exact bytes.
- Active dot branches reserve blocked.md, unanswered packets and U functions>=0x400 bytes. Fetch every few hours. Ten hard-function packets are committed; Pro relay is paused.

## Next three tasks
1. Accept fresh source-frozen helper proposals in small checkpoints and preserve affected earlier matches. Keep the seven other lanes on eligible fresh families.
2. Review the two new dot branches in root's intake lane. Keep main linking/exporting, STATE and hourly private-origin pushes current.
3. Continue native World1-1 storage/runtime work. RGB565 is committed; inspect a bounded independently verifiable ETC1 slice next.

## Blockers and measurement limits
- Whole U tables and named BSS have independently verified weak zero-filled compact-main imports. They do not reconstruct game behavior.
- Unmapped source helpers require provenance-checked inline closure and must disappear fully. No guessed standalone address is allowed.
- Legacy progress now reports391 matches,7 Non-matching and20528/3,092,336 word-similarity bytes. Strict function coverage uses complete intervals independently.
- Ledger times include check-only rows and shared measured script windows. Related windows overlap and exclude unmeasured preparation; they are not independent person-hours. No cost tracking.
- Runtime lacks actor overrides, complete texture/material decoding, rendering, game execution and replay. Three nested layout archives remain opaque. Other137 image formats retain explicit unsupported storage status.
- Stop only under current BRIEF conditions. Latest matches keep the six-hour/100-attempt stall condition clear.
