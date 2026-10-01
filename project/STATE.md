# Project state

Last updated: 2026-10-01, autonomous Phase 2 and runtime session.

## Final objective
M2: 100% byte-exact matching and original EU whole-image hash from linked code.bin. No pilot or review stop. Follow project/BRIEF.md.

## Current milestones
M0 passes three committed-source compiler discriminators. M1 passes all 50 pilot outcomes and the report. M2 remains the final goal. M3 gameplay/replay is unverified.

## Verified
- Original dump and EU executable hashes remain unchanged. No game data is committed.
- Clean python make.py eu -ca now compiles42 Game/96 al sources and links/exports on its first pass after e49d2bf scaffold ordering repair. Main remains a scaffold. Every push requires successful clean linking.
- Accepted coverage: 407 functions from canonical project ARMCC objects with committed source/header provenance, covering 23416 / 2,756,024 complete function bytes (0.849630%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup, fn_001C5A88 and Game fn_001BB19C pass791 and fail894. M0 evidence: build/game_compiler_check/evidence.json.
- Pilot: 40 matched, 1 NonMatching, 9 abandoned. See pilot_report.md, pilot_results.csv and pilot_iterations.csv. Phase 2 continues.
- Runtime verifies 73,421 RomFS IVFC blocks and 50 selected World1-1 files. Native reader validates 298 placements, 3853 collision prisms, 39 models, 137 mesh/material bindings,127 materials and217 direct texture references. No level has run.
- Native math matches3422 direct retail float bits in debug/O3. All145 texture headers/extents agree with bytes and226 bounded original consumer cases. Shape transfer/copy interfaces pass822 native words and548 original gate cases.
- RGB565 storage passes37824 packed words,113472 channel integers and all75648 bytes across8 images/11 supported mip levels. Independent traversal,65536 word fixtures,6 tile patterns,7 unsupported/cache cases and6 malformed cases pass. Frozen manifest: data/runtime/romfs/native_rgb565_frozen/command_result_manifest.json.

## Lanes and acceptance queue
- Root alone owns dot intake, canonical acceptance, map, ledger, shared docs, Git and origin pushes. Seven other lanes continue disjoint matching/runtime work. Maximum8 lanes.
- All24 exact claims from eight earlier dot proposals pass locally,3700 bytes total. The additional dot/item-type-lookup passes locally,1116 bytes. Dot/effect-action-update claims no match; its report/packet are imported while source/header intake awaits whole-table identities. No dot map/rank/ledger/tool edits are imported directly.
- Latest full shared-header recheck preserves391/391 at21576 bytes, frozen in build/stage_matrix_header_recheck_frozen.json. All19 weak Nerve::executeOnEnd definitions pass independently. Later5 canonical roots add1376 bytes.
- Latest reaction/placement/camera/Scene/Nerve38 checkpoint passes1032 bytes; stage/area/collision/effect-interface/NerveBase13 passes396 bytes. Independent NerveStateBase table ownership repair is committed separately as d5d925f.
- Ready queue:82 enrolled source-frozen helpers,2472 proposed bytes. Canonical acceptance is pending after clean build and full earlier397-root recheck; build/helper_family_checkpoint_candidates.json. Older updateCollider168 remains under import/source review.
- Matching lanes continue collision/actor helpers, ActorInitInfo/link readers, actor-group helpers, math/layout work, effects and stage switches. Runtime verifies all four selected storage formats; sampled colors and geometry transforms remain under investigation. Production remains frozen during canonical checks.
- Guarded sensor execute records810 bounded ARM11 pairs, placement initialization76 and heap creation380 returning/30 fault pairs. These frozen NonMatching results add zero exact bytes.
- Active dot branches reserve blocked.md, unanswered packets and U functions>=0x400 bytes. Fetch every few hours. Fourteen hard-function packets are committed; Pro relay is paused.

## Next three tasks
1. Accept queued source-frozen helpers after the successful clean rebuild, preserving affected earlier roots. Keep matching lanes active; every push requires a successful clean -ca link.
2. Review the two new dot branches in root's intake lane. Keep main linking/exporting, STATE and hourly private-origin pushes current.
3. Continue native texture/color and geometry interpretation. Prepare an isolated linker-produced full-image diagnostic, with explicit unimplemented regions and whole-image comparisons. Legacy copied Game assembly cannot satisfy hard rule 3.

## Blockers and measurement limits
- Whole U tables and named BSS have independently verified weak zero-filled compact-main imports. They do not reconstruct game behavior.
- Unmapped source helpers require provenance-checked inline closure and must disappear fully. No guessed standalone address is allowed.
- Legacy progress now reports391 matches,7 Non-matching and20528/3,092,336 word-similarity bytes. Strict function coverage uses complete intervals independently.
- Ledger times include check-only rows and shared measured script windows. Related windows overlap and exclude unmeasured preparation; they are not independent person-hours. No cost tracking.
- Runtime lacks actor overrides, complete texture/material decoding, rendering, game execution and replay. Three nested layout archives remain opaque. All145 selected images now have bounded raw storage intake over488 complete levels and1851264 bytes; unknown formats/tails remain unsupported. ETC1 raw storage passes142000 blocks/2272000 selectors across110 images/360 levels,1136000 bytes; sampled pixels and colors remain unverified.
- Stop only under current BRIEF conditions. Latest matches keep the six-hour/100-attempt stall condition clear.

- ETC1A4 raw storage passes all18 images/72 levels,15424 alpha/color pairs and246784 nibbles,246784 bytes. All491 unique frozen/current/preserved paths verify, with495 hash entries. Existing native regressions pass. Packing is verified; normalization, sampled colors and rendering remain unresolved.

- HILO8 raw storage passes9 images/45 levels,196416 words and392832 serialized bytes/components. Root verifies541 hash entries over537 unique frozen/current/preserved paths. Signedness, sampled channel mapping and rendering remain unresolved.
