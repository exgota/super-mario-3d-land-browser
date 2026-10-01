# HitSensorDirector constructor

## Result

`al::HitSensorDirector::HitSensorDirector()` (`_ZN2al17HitSensorDirectorC1Ev`) occupies `0x001D1F90`–`0x001D20FC`: 364 bytes (`0x16C`), including the literal pool beginning at `0x001D20BC`.

The parent lane ran the repository build under ARMCC 4.1 build 791 and then:

```sh
. ./development_environment.sh
python tools/check.py _ZN2al17HitSensorDirectorC1Ev --object build/eu/obj/lib/al/src/LiveActor/alHitSensorDirector.o
```

Reported checker result:

```text
U -> O: The complete source-generated function interval matches byte for byte.
```

The checked constructor source and headers were already committed and unchanged from refreshed main `9fba58fa1db1aaabad80d2b77d87139bd83b80e6`. At that constructor-only checkpoint, this lane made no source or header edits. Across the follow-on work it ran no concurrent build and made no map, rank, tool, configuration, ledger, or STATE changes. The parent owns the build, exact check and rank update. The prior blocked entry was stale: `al::SensorHitGroup::SensorHitGroup(int, const char*)` is now independently mapped at `0x002496F8` and ranked O on main.

## Independent retail observations

The owner-provided executable has the approved SHA-256 `e1d7e188ff88467df776c17cec45c44857fadf5b699944baa8cddcae7d939e64`. Source inspection and Capstone disassembly established:

- The object is 28 bytes: one vtable pointer followed by six `SensorHitGroup*` fields at offsets `4`, `8`, `12`, `16`, `20`, and `24`.
- Each allocation requests 12 bytes through the already-mapped nothrow operator new at `0x002932B0`, then conditionally calls the now-mapped group constructor at `0x002496F8`.
- The six capacities and names, in order, are `16 / Player`, `128 / Ride`, `512 / Eye`, `2048 / Simple`, `1024 / MapObj`, and `1024 / Character`.
- The final call is the mapped `al::registerExecutorUser(al::IUseExecutor*, const char*)` at `0x001CC79C`.
- Its registration name at `0x001D20F0` is CP932 `83 5A 83 93 83 54 81 5B 00`, decoding to センサー. The source already contains the correct original CP932 bytes. A UTF-8 terminal displays replacement characters; that display is not evidence of source corruption. No conversion was needed or performed.
- The constructor stores `0x003D6920`, which is exactly the existing mapped `_ZTVN2al17HitSensorDirectorE` address `0x003D6918` plus the ordinary eight-byte ABI header. Unlike the separately blocked NerveExecutor/LayoutActor constructors, this data identity needs no map-boundary repair.

The canonical object's constructor section is exactly `0x16C` bytes. Independent inspection found zero differences across the 77 non-relocation words, including every string and padding byte. Its remaining 14 words are relocations: six allocations, six group constructors, one executor registration, and one vtable address. This inspection supported the diagnostic; only the parent lane's unchanged project checker established the match.

## Follow-on class evidence

The retail vtable has zero words at `0x003D6918` and `0x003D691C`, then function pointers:

- `0x003D6920 -> 0x001D1DA0`: the execute slot, currently an unnamed 496-byte (`0x1F0`) function. It clears/tests the six sensor groups, calls a pair-testing helper at `0x002497B0`, and performs character-group self-pair overlap checks using actor identity at sensor offset `0x28`, position at `0x08`, and radius at `0x14`.
- `0x003D6924 -> 0x00330450`: the draw slot, a single `bx lr` instruction.

These are independently grounded identities for later work, not additional matched functions. At the constructor-only checkpoint the header inherited the placeholder `IUseExecutor::execute()` and `draw()`. Accepting the constructor's reference to the independently mapped vtable did not establish its runtime execution behavior. The follow-on below reconstructs execute separately; only the constructor is currently exact.

## Current canonical checker revalidation

On 2026-10-01, a clean worktree of main `a360142fbddd9ab875ab68314c55445ade05fd00` was reconstructed and its entire Git tree verified. The proposed source/header edits were applied and committed as local verification checkpoint `eace3c0`. The unchanged current project build (`python make.py eu`, ARMCC 4.1/791 for game code) compiled, linked, and exported successfully. The current canonical `tools/check.py --object` commands were then rerun on those freshly built objects. No checker/tool changes were made. The same independently evidenced local-only map identities were supplied; no existing function interval changed.

The unchanged constructor reports `U -> O: The complete source-generated function interval matches byte for byte.` All 364 bytes pass without extra identities specific to this function. This is revalidation of existing source and resolution of a stale blocker.


## Execute follow-on, final first-pass nonmatching result

`al::HitSensorDirector::execute()` (`_ZN2al17HitSensorDirector7executeEv`) is independently identified by the vtable slot and spans `0x001D1DA0`–`0x001D1F90`, 496 bytes (`0x1F0`). Its normal C++ implementation and local helper are guarded by `NON_MATCHING`; the project enables that macro. The parent added the verified execute identity to its local map and owns all rank changes. No map changes belong in the dot proposal.

The reconstructed routine clears contact counts in all six groups, tests thirteen cross-group combinations in the retail order, then tests unordered character-group pairs. The inner loop starts at the outer index, including the self-pair that is rejected by equal host identity. Each directional contact is added only when the other sensor's type is nonzero (`SensorType_Eye` is zero). The parent must preserve the ordered rejection `radiusSquared <= distanceSquared`; replacing it with acceptance `radiusSquared > distanceSquared` changes the retail unordered-float path.

The same-sized header reconstruction replaces the unused `u32 _8`, `u32 _C`, and `float _10` fields by a `sead::Vector3f mPosition` at offset `0x08`. `getPos()` returns a const reference, and `SensorHitGroup::getSensorCount()` reads the existing field at `0x04`. The existing `sizeof(HitSensor) == 0x40` assertion remains. No field offset or type after position changes. `HitSensorDirector` now declares the execute override; draw remains the behaviorally empty inherited implementation.

Four imports retain address names because their original semantic names are unproven:

- `fn_0024977C(HitSensor*, HitSensor*)`, `0x28` bytes: append the other sensor at `mSensors[mSensorCount]` if the unsigned 16-bit count is below capacity, then increment count.
- `fn_002497A4(SensorHitGroup*, int)`, `0x0C` bytes: return `mSensors[index]`.
- `fn_002497B0(HitSensorDirector*, SensorHitGroup*, SensorHitGroup*)`, `0xF4` bytes: nested cross-group tests with the same host, distance, radius, type and directional-contact behavior as the self-group loop. Its director argument is unused by the retail body.
- `fn_002498A4(SensorHitGroup*)`, `0x74` bytes: clear each member sensor's 16-bit contact count. The retail compiler unrolls the loop in pairs.

These identities come from complete retail disassembly of the callees and the independently identified class vtable, not from guessed imported symbol addresses. All four already have unchanged function intervals in the map. This lane has not implemented or claimed matches for these helpers.

The parent used the canonical project build and ran:

```sh
. ./development_environment.sh
python tools/check.py _ZN2al17HitSensorDirector7executeEv --object build/eu/obj/lib/al/src/LiveActor/alHitSensorDirector.o
```

Every attempt below emitted a `0x1F0`-byte section but failed the complete byte check. The first check changed U to m; subsequent checks remained m. The final eighth check on committed source `628f84c` reported m to m. Execute is not an exact match.

1. `5385fe4`: direct nested-loop source and scalar differences. Control flow and initial group handling matched; inner-loop register allocation, float load scheduling, and the unordered comparison differed.
2. `4020a6e`: Vector3f constructor and ordered early rejection. All float operations, their load order and the branch matched, but three integer registers were cyclically permuted.
3. `db09380`: inline pair helper. Integer register allocation matched; twelve non-call instructions differed in host comparison and float scheduling.
4. `d10e722`: inline self-group loop. Integer and float allocation regressed.
5. `f7b20eb`: inline pair helper with an explicit second-host local and componentwise vector assignments. All integer/control instructions matched; fifteen VFP instructions differed.
6. `f73c0d4`: vector copy followed by component subtraction. Integer and VFP register allocation differed. Exact checker result remained m.
7. `e18f312`: direct overlap tests with only contact registration factored into an inline helper. Delayed second-pointer preservation changed broader integer and VFP allocation; exact checker result remained m.
8. `628f84c`: inline whole-pair helper, explicit second-host local, and Vector3f constructor. All integer/control instructions match in the diagnostic comparison. Nine non-call VFP/load instructions differ, principally the y/z register allocation, one paired load versus scalar loads, and their scheduling. Exact checker result remained m. This is the retained guarded source and the smallest diagnostic instruction difference in the eight-attempt first pass.

The first pass stops after eight measured variants. The retained function is a source reconstruction with measured nonmatching status, not an additional exact match. The retail loop structure, scalar addition order, and ordered rejection that leaves the unordered/NaN path open are represented in the source; neither runtime behavior nor floating-point exception behavior has been validated by gameplay or differential replay. The nine-instruction diagnostic difference is not an exact-match metric and does not contribute to matched coverage.

No inline assembly, byte arrays, global compiler-flag changes, target changes, or edited compiler objects were used. The remaining evidence points to source-expression/inlining/register-allocation differences, not an unidentified callee or data address. Revisit only with a new grounded compiler/dataflow hypothesis, rather than repeating any of these eight variants. Both functions were rechecked before publication against current canonical main, as recorded below.

## Final proposal revalidation

The final source/header proposal was committed as local verification checkpoint `ba501af` in the clean current-main worktree based on `a360142fbddd9ab875ab68314c55445ade05fd00`. Its canonical project object compiled successfully. With only the verified execute symbol added to the existing `0x001D1DA0` row locally, the unchanged current checker reports:

```text
execute: U -> m: The linked candidate differs from the unchanged original interval.
constructor: O -> O: The complete source-generated function interval matches byte for byte.
```

The constructor remains the sole exact result in this branch (364 bytes). Execute remains a 496-byte nonmatching proposal and is excluded from exact coverage. The compact scaffold link at this combined verification checkpoint still reports unrelated unresolved Fugumannen vtable helpers and the previously identified zero-vector global; this does not affect the provenance-verified direct object checks, and no complete runtime validation is claimed.
