# Combined actor and utility proposal preservation

Five non-held source proposals were composed without source edits on current main `3de056e0dcb33619c32f8b46ef64f74c90d28934`: Tree, NoteObjGenerator, Executor/Kit, Collider, and BombHei. The combined ordinary C++ tree clean-builds, links and exports. The unchanged canonical checker passes **all663 prior accepted roots plus the same64-byte Tree proposal**, including every duplicate emitted definition: **683/683 object checks exact**. Main's prior36,640 accepted bytes are preserved. This is one pending64-byte proposal, not a new batch of exact additions; the other bodies remain unaccepted partials.

The tested local source commit is `36a86e9b72b5b7cd90d0f853871ba308cada32ed`, tree `e21dcc7b2fca05dd575b8fc4cc09fffe65f88b99`. Ten source/header blobs are copied byte-for-byte from the immutable published commits in the evidence. No additional source form, flag, metadata identity or local main patch was introduced. This branch publishes notes only; individual branches remain the source proposals.

## Why this composition was checked

The proposals independently changed NoteObjGenerator fields, the LiveActorGroup interface and Collider fields. Separate successful checks would not establish that these headers compose. This test builds them together with Tree's call-boundary annotation and BombHei's isolated header/body, then examines every accepted root from the frozen main map. The inventory has no missing root; all emitted definitions, including weak copies, are checked, rather than choosing one copy and silently ignoring the rest.

All six affected function objects also retain **identical allocated sections**, including byte hashes, flags, alignment, sizes, symbol definitions, relocation offsets/types/targets and linked-section relationships, versus their individually tested builds. This is stronger than comparing only the root instruction bytes. Whole-object hashes differ in non-runtime metadata and are not claimed equal.

| Object root | Complete source bytes | Result carried into this composition |
|---|---:|---|
|Tree push-front002F2AD4|64|Canonical exact, three accepted companions preserved|
|Note initializer00171334|644|Prior diagnostic8 differences; canonical data closure remains unresolved|
|Executor holder001E37D8|912|Prior canonical size mismatch versus908|
|Kit end-init00274990|228|Prior canonical size mismatch versus236|
|Collider invalidation0024C9EC|88|Prior canonical8 differences|
|BombHei control0030E678|704|Prior canonical49 differences|

This pass does not add or rerun the earlier bounded behavioral replay counts. Their compiler-generated allocated sections and relocations are preserved; their original real-callee/model/exceptional-input limits still apply. It does not claim complete runtime integration or activate missing data identities. The clean link uses the unchanged main map, where the proposed nonmatching bodies remain unaccepted. Three Pro-held proposals (Beat, Fixed-point, Scale) are excluded completely.

## Exact evidence

The check batch ran for399.240386 seconds measured by the driver; this excludes the preceding build, comparison work and report preparation, and overlaps other work on the cloud host. All683 CLI checks exited0 and printed the complete-interval exact message. The map was restored byte-for-byte afterward; SHA256 `514c3ec6f2343e8a40fc73996df78fc95b2e33f4897fbdda6bb0311df6cf40fa`.

The [complete evidence](proposal-preservation/evidence.json) contains every symbol/object pair, canonical output and object hash, the source Gitblob manifest, all section/relocation contracts, and exact checker hashes. The [reproduction appendix](proposal-preservation-replay.md) reconstructs the pinned source tree from published Git objects and provides the full audit and comparison scripts. No game input, compiled object or binary artifact is included. Main owns acceptance and integration.
