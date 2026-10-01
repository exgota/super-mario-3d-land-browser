# Project state

Last updated: 2026-10-01, autonomous Phase 2 and runtime session.

## Final objective
M2: 100% byte-exact matching and original EU whole-image hash from linked code.bin. No pilot or review stop. Follow project/BRIEF.md.

## Current milestones
M0 passes three committed-source compiler discriminators. M1 passes all 50 pilot outcomes and the report. M2 remains the final goal. M3 gameplay/replay is unverified.

## Verified
- Original dump and EU executable hashes remain unchanged. No game data is committed.
- Clean python make.py eu -ca compiles42 Game/106 al sources and links/exports after scaffold ordering and unaccepted-table repairs. Main remains an explicit scaffold. Every push requires successful clean linking.
- Accepted coverage: 557 functions from canonical project ARMCC objects with committed source/header provenance, covering 29628 / 2,756,024 complete function bytes (1.075027%). Legacy progress.py's byte percentage is word similarity, not exact coverage.
- Sensor-name lookup, fn_001C5A88 and Game fn_001BB19C pass791 and fail894. M0 evidence: build/game_compiler_check/evidence.json.
- Pilot: 40 matched, 1 NonMatching, 9 abandoned. See pilot_report.md, pilot_results.csv and pilot_iterations.csv. Phase 2 continues.
- Runtime verifies 73,421 RomFS IVFC blocks and 50 selected World1-1 files. Native reader validates 298 placements, 3853 collision prisms, 39 models, 137 mesh/material bindings,127 materials and217 direct texture references. No level has run.
- Native math matches3422 direct retail float bits in debug/O3. All145 texture headers/extents agree with bytes and226 bounded original consumer cases. Shape transfer/copy interfaces pass822 native words and548 original gate cases.
- RGB565 storage passes37824 packed words,113472 channel integers and all75648 bytes across8 images/11 supported mip levels. Independent traversal,65536 word fixtures,6 tile patterns,7 unsupported/cache cases and6 malformed cases pass. Frozen manifest: data/runtime/romfs/native_rgb565_frozen/command_result_manifest.json.

## Lanes and acceptance queue
- Root alone owns dot intake, canonical acceptance, map, ledger, shared docs, Git and origin pushes. Seven other lanes continue disjoint matching/runtime work. Maximum8 lanes.
- All24 exact claims from eight earlier dot proposals pass locally,3700 bytes total. The additional dot/item-type-lookup passes locally,1116 bytes. Dot/effect-action-update claims no match; its report/packet are imported while source/header intake awaits whole-table identities. No dot map/rank/ledger/tool edits are imported directly.
- Latest full preservation recheck passes397/397 accepted roots at23036 bytes and all415 canonical definitions, including19 weak Nerve copies. The new82 roots all pass canonical checks, adding2472 bytes; frozen in build/helper_family_checkpoint_candidates_canonical_acceptance.json.
- Latest reaction/placement/camera/Scene/Nerve38 checkpoint passes1032 bytes; stage/area/collision/effect-interface/NerveBase13 passes396 bytes. Independent NerveStateBase table ownership repair is committed separately as d5d925f.
- The82-root checkpoint,35-root follow-on1452 bytes and13-root actor/audio424 bytes are accepted. Prior479 roots/all497 canonical definitions pass; the next clean build preserves all144 prior canonical objects byte-for-byte. Older updateCollider168 remains under import/source review.
- Matching lanes continue collision/actor helpers, ActorInitInfo/link readers, actor-group helpers, math/layout work, effects and stage switches. Runtime verifies all four selected storage formats; GPU sampled output and geometry transforms remain under investigation. Production remains frozen during canonical checks.
- Guarded sensor execute records810 bounded ARM11 pairs, placement initialization76 and heap creation380 returning/30 fault pairs. These frozen NonMatching results add zero exact bytes.
- Active dot branches reserve blocked.md, unanswered packets and U functions>=0x400 bytes. Fetch every few hours. Twenty-seven hard-function packets are committed; Pro relay is paused.

## Next three tasks
1. Accept queued source-frozen helpers after the successful clean rebuild, preserving affected earlier roots. Keep matching lanes active; every push requires a successful clean -ca link.
2. Review new dot proposals in root's intake lane. New quaternion/translation/effect-context/integer branches claim zero exact additions; their bounded source/replay remains under review. Keep hourly private-origin pushes current.
3. Continue native geometry/palette interpretation. Installed make.py eu --split now links an isolated527-root full-image diagnostic. Keep unknown regions explicit and compare whole-image hashes. Legacy copied Game assembly cannot satisfy hard rule3.

## Blockers and measurement limits
- Whole U tables and named BSS have independently verified weak zero-filled compact-main imports. They do not reconstruct game behavior.
- Unmapped source helpers require provenance-checked inline closure and must disappear fully. No guessed standalone address is allowed.
- Latest measured legacy progress reports514 matches,7 NonMatching and25160/3092336 word-similarity bytes; a refresh follows the current clean527-root build. Strict coverage is27384/2756024 complete function bytes.
- Ledger times include check-only rows and shared measured script windows. Related windows overlap and exclude unmeasured preparation; they are not independent person-hours. No cost tracking.
- Runtime lacks actor overrides, complete texture/material decoding, rendering, game execution and replay. Three nested layout archives remain opaque. All145 selected images now have bounded raw storage intake over488 complete levels and1851264 bytes; unknown formats/tails remain unsupported. ETC1 raw storage passes142000 blocks/2272000 selectors across110 images/360 levels,1136000 bytes; sampled pixels and colors remain unverified.
- Stop only under current BRIEF conditions. Latest matches keep the six-hour/100-attempt stall condition clear.

- ETC1A4 raw storage passes all18 images/72 levels,15424 alpha/color pairs and246784 nibbles,246784 bytes. All491 unique frozen/current/preserved paths verify, with495 hash entries. Existing native regressions pass. Packing is verified; normalization, sampled colors and rendering remain unresolved.

- HILO8 raw storage passes9 images/45 levels,196416 words and392832 serialized bytes/components. Root verifies541 hash entries over537 unique frozen/current/preserved paths. Signedness, sampled channel mapping and rendering remain unresolved.

- Fresh separate clone of27af5f6 clean-builds and its full linked-map check preserves479/479 O roots in127 seconds. Five factory templates needed Type ft for their independently observed t.<symbol> sections. Canonical bodies already reproduced in the fresh clone; no new credit. The initial audit attempt lacked the asm-differ submodule; initializing the pinned submodule allowed the full check.

- Native RGBA8 reconstruction passes136 images/443 complete levels/2556608 pixels/10226432 bytes against two independent formulations and traversals. Root verifies613 hash entries over609 unique paths. HILO8 remains raw. RGB565 nearest-normalized output is an explicit representation policy with12968 pixels differing from bit replication by at mostone/component. GPU sampling, orientation, rendering and gameplay remain unverified.

- Installed full-image split verifies527/527 O intervals/27384 complete bytes. Actual3096576-byte image SHA b6df708858b6d793cac96d7a29e3a71c60aa9f20918cdad9c18107c4942f199f differs in2491041 bytes. Incidental zero/padding equality adds no credit; M2 remains open. See full_image_diagnostic.md.

- Fresh separate f224734 checkout clean-builds and its repaired full linked-map audit preserves527/527 O roots. The prior run lost only SystemKitC2 from scatter section aliasing, now explicitly repaired. All root O canonical intervals also remain equal in the isolated full-image link.

- Static mode-zero native CPU palette agrees with 404 original cases/509 joints/22,432 words per debug and optimized build. Root verifies 635 hashes over 631 paths. Eleven World1-1 instances construct the bounded palette; five preserve rotation-table blockers. Root records are controlled inputs, the actual model adapter and final vertex behavior remain unverified. One new malformed skeleton case and all prior native regressions pass.
