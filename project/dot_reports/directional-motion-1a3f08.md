# Directional motion 001A3F08: NonMatching, inherited gate blocker

## Result and blocker

The 1924-byte root is reconstructed in
lib/al/src/Movement/DirectionalMotionUpdate.cpp. It remains NON_MATCHING.
ARMCC 4.1/791 produces an 1844-byte complete section; the canonical checker
rejects its extent. Exact claims: none. New accepted bytes: zero.

The final clean build links, but the full all-definition preservation gate does
not pass: 1655 of 1656 definitions are exact. The inherited failure is
_ZN2rp14getPlayerActorEv in Game/backup/src/Player/PlayerFunction.o, rejected for
an unresolved non-branch data relocation. Its Factory definition passes. Every
one of the 1635 baseline O roots has at least one passing definition; 1634 roots
have all definitions passing. This is not reported as full preservation.

The same canonical rejection was independently reproduced in a clean checkout
of the frozen main without this candidate. The 20-byte function section is
identical in both worktrees, SHA-256
475947a2c99dac4b6184647d1cea74392b1fb86ad5da8eda5bffa1871072c0ca.
All 352 prior compiler objects are byte-identical to this candidate worktree's
own clean frozen baseline. No source, header or oracle in that other family was
changed. The parent/integrator was notified. The independent command, log hashes,
compiler command and complete recorded input provenance are preserved in the
accompanying inherited-baseline JSON note.

Final modeled replay passes 239 cases, all returning normally, and visits all
474 original executable instructions. Exact float argument bits, memory and
register preservation agree under the explicit fixtures. Direct imports and
virtual methods do not execute their original bodies. This does not prove
full-domain floating behavior, actual gameplay, or the semantics of those helpers.

## Frozen base and ownership

Fresh main 3d17628733ab941c39229f725ea9f496fe5dde70 was materialized only after
verifying every required Git blob, recursive tree and the exact commit identity.
The updated BRIEF, unchanged AGENTS/DOT_BRIEF/STATE, and new daily entry were read.
Factory batching and periodic 45-minute submission checks are the new intake
rules; dot ownership and hard rules are unchanged. Source/report/branch screening
found no prior body ownership for this U root. This isolated branch owns its new
TU and own notes only, with no shared-header or other family edits.

The frozen baseline contains 1635 O rows and 80604 complete accepted bytes.
Those are main's recorded totals, not candidate credit or an all-definition
validation claim. The owner binary SHA-256 was reverified as
e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64.
Original data, generated objects, ranks/map changes, tools, STATE and ledger are
excluded. The map is restored to its exact frozen bytes. Nothing was published
or submitted to the integrator.

## Actual attempts and checks

Form one generated an 1868-byte section plus a local constant up-vector
initializer. The unchanged closure checker rejected its non-branch data reference.
Explicit scalar up-vector construction removes that compiler initializer. This
second form produces 1844 bytes, fails canonical size, and passes all 239 modeled
pairs. No third speculative form, alternate compiler, flag rescue, assembly
stand-in or oracle change was used.

The final command, after sourcing development_environment.sh, was:

```
python tools/check.py fn_001A3F08 --object build/eu/obj/lib/al/src/Movement/DirectionalMotionUpdate.o
M -> M: The complete compiled section, including its literal pool, has a different size from the original interval.
```

Checker exit code: 1. Scratch enrollment uses only M and the ordinary address
fallback. Boundaries, type, section and pool stay unchanged. The final independent
all-definition pass attempt took 200.432 seconds and explicitly retains its one
inherited rejection. Final paired replay took 5.369 seconds.

Work window: 2026-10-02 11:10–11:32 UTC. Accepted throughput is zero. The next
screened disjoint root is 00259B20, U, 1908 bytes, a floating-power-style routine.
Its isolated clean baseline on the same freshly checked main also provided the
independent PlayerFunction rejection above. It has no candidate claim yet.

Layout and reproduction notes give the ABI, bit predicates, scalar quaternion
arithmetic, exact fixture scope and the inherited preservation limit. Existing
module placement is a build carrier, not proof of original compiler identity.

## Final input/output hashes

`lib/al/src/Movement/DirectionalMotionUpdate.cpp`
f02bcd534db9164951d37d2f34238efd04416d7e2d5a1706ce758ca054440daf

`build/eu/obj/lib/al/src/Movement/DirectionalMotionUpdate.o`
5f598f441a2abded48692853ef6cafddaec0664c7bebf6412eb665817b44a86d

`build/eu/obj/lib/al/src/Movement/DirectionalMotionUpdate.provenance.json`
ed39472ee60664eb28f3ea475235f1b7cf909059ead0f458d4ae0eb7ddb45efe

`build/root1a3f/final-clean-build.log`
6c0134abfbf4fd9db0de029e4c65c4ac83a5b4b8b0350176d0bfe108ca0690a4

`build/root1a3f/final-check.log`
45a54c1af5d739b44f5160edbcdf1fc69f44ef2fcbd9a80fcf97a4b1b6682ae0

`build/root1a3f/replay.py`
e05cd79c409cf55074b62b340f786f3dbe2dd58a15cf44660517de64d06a9353

`build/root1a3f/preservation.json`
cd19eb3ba3414a73adac4147ef018bf5b82e5a4a1f792aae035edf0ca867fe37

`data/ver/eu/map.csv`
2e366cd4686b9ab78ab5bdc771d99bc113bec5ad9a11865390be232302b24e59
