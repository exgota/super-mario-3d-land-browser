# Shared declaration blockers for the matching driver

2026-10-04 UTC. Claims: none. Skip the blocked roots below pending owner-led interface repair and preservation of affected accepted roots.
Basis: immutable main `238bbbecba3340df1ad542fc2e2a420a4ff89670`; verified source-only WIP `de16f556de1026b0b516d9fb689a111d3dc23395` (74 files); `CURRENT.md`, `preflight/`, `recovered-preflights/`, and restored function notes.
This is a source/record consolidation, not a new binary audit or recovered compiler proof. Main declarations were read without modification. Historical lost/unreviewed scope is not exhaustive; absent records do not establish zero prior attempts.
The 780-tier and 00119644/00122144 call observations were recovered from the coordinator's retained findings; their accepted declarations were checked against main. Other call evidence is in the named preflights/notes. `Factory/` below means `Game/backup/src/Factory/`.

## Known live declaration conflicts

- **Vector add/multScalar:** roots `001D8D8C`, `0023FEE0`, `0017974C`, `0026BAB0` (780), `00173790` and `002D8F3C` (784).
  Helpers `0027CB48`/`0027CC64` use accepted literal-mangled C imports over unrelated anonymous Vec3/Vector3 types; `math/seadVectorCalcCtr.h` lacks the ordinary declarations. Adding declarations alone leaves the old contracts conflicting.
  Repair one coherent vector API and migrate the 15-TU accepted caller union. Owners: Factory `fn_0016D830.cpp`, `fn_00171BE0.cpp`, `fn_00173698.cpp`, `fn_001BAD90.cpp`, `fn_001BD204.cpp`, `fn_00213854.cpp`, `fn_00213B74.cpp`, `fn_0030E468.cpp`, `fn_0032787C.cpp`, `fn_003449B4.cpp`, `fn_0034D9AC.cpp`, `fn_00355820.cpp`, `fn_003600F8.cpp`, `fn_00360DC0.cpp`, `fn_0036844C.cpp`.
  Evidence: restored `001D8D8C`/`0023FEE0`/`0017974C` notes; recovered `173790`/`2d8f3c` preflights; retained `26BAB0` finding.

- **Projection/normalization and actor-vector helpers:** roots `0017974C`, `00173790`, and `00137E30`.
  `0027306C` is projection rejection, with incompatible private Vec3 imports in Factory `fn_00173698.cpp`/`fn_001BAD90.cpp`. `0027D5C4` has established Boolean output but private Boolean declarations in `fn_0017E4B8.cpp`/`fn_00355820.cpp` versus void declarations in `fn_003600F8.cpp`/`group_00263CE8.cpp`.
  `00137E30` calls `0027C0C8` at `001380C8`; Factory `fn_00355820.cpp` declares private Actor/Vector3. Migrate these actual owners to compatible ordinary actor/vector types and preserve callers; ignored results do not cure conflicting return types.
  Evidence: recovered `173790` preflight; restored `0017974C`/`00137E30` notes.

- **Actor initialization and rail index:** roots `0019ADFC` (784) and `00310588` (780).
  `0019ADFC` calls `0026F5CC`, defined with anonymous Context in Factory `fn_0026F5CC.cpp`; its storage resemblance to ActorInitInfo does not establish type identity. `00270AB0`/`0026F5A8`/`0026F56C` import private Actor in `fn_0018938C.cpp`; `00310588` also needs `00270AB0`. `0025AB80` uses private StateActor in `fn_0016DABC.cpp`.
  Reconcile the real receiver/context declarations and extract compatible qualified APIs. Current `fn_0018938C.cpp` already uses ordinary `al::ActorInitInfo`; do not report its info parameter as still private. Preserve those accepted owners and dependent objects.
  Evidence: recovered `19adfc` preflight; retained `310588` finding; `LiveActor/alActorInitializationImports.h`.

- **Placement Boolean result and offset-holder receiver:** root `00119644` (784); related result inconsistency also reaches `0014C810` (788).
  `00119644` consumes Boolean `0027D180` at `001196A8`; Factory `fn_0018938C.cpp` declares `void(float*, const al::ActorInitInfo*)`. Reconcile actual output/return semantics and preserve that caller. This is a known live result mismatch, not an unknown unused return.
  `002519A8` is accepted with anonymous OffsetHolder in `group_001C2D9C.cpp`; extract a coherent receiver contract and preserve the group. `0014C810` also encounters Boolean `0027D1DC` declared void in `fn_0018938C.cpp`, although its own result is unused.
  Evidence: retained `119644` finding; recovered `14c810` preflight; current accepted source.

- **Quaternion/rail:** root `00122144` (784), call `001223E0` to `002695C4`.
  Factory `group_0016C1B8.cpp` imports anonymous Quaternion/Rail. Reconcile these with the real quaternion/rail API and preserve accepted `0016C1B8`/`0016C34C`; a matching layout or private alias is insufficient. Evidence: retained finding and current source.

- **Sensor Wrapper:** roots `00310588` (780) and `002D8F3C` (784).
  `001EB9A4` receives MapObjActor/Body/matrix in `00310588`; `001EB9D0` is called at `002D908C`/`002D909C`. Both accepted exports in Factory `group_001EB9A4.cpp` take anonymous Wrapper and use a TU-local partial HitSensorKeeper declaration.
  Repair the real actor/sensor interface, reconcile keeper ownership and preserve both accepted helpers. Evidence: retained `310588` finding; recovered `2d8f3c` preflight.

- **State, service accessor and animator:** root `00173790` (784).
  Factory `fn_001BAD90.cpp` imports the root with private State/Holder/Body; `fn_00173698.cpp` defines private Motion/MotionState/MotionControl. Shared accessor `0026E1DC` is inconsistently declared Settings*() in `fn_00171BE0.cpp`, Limit*() in `fn_001BAD90.cpp`, and void(Motion*) in `fn_00173698.cpp`, although the retained body evidence establishes a parameterless pointer accessor.
  Migrate those owner contracts. `Player/PlayerAnimator.h` declares `IUsePlayerAnimator::v_8()` without the live SafeString argument used at `001737F8`/`00173970`/`0017398C`, targeting `0014F488`; correct the real interface and preserve its users. Other concrete service targets remain unresolved separately.
  Evidence: recovered `173790` preflight and current owners.

- **Texture format, dimensions and release arguments:** root `002FA29C` (780).
  Factory `fn_002FA294.cpp` imports root argument 2 as void*; retained call evidence establishes a scalar format byte at +60. Dimension helpers `0021DEFC`/`0021DEC8` return 16-bit integer dimensions but are declared void* by `group_0021DEC0.cpp`/`group_00374CE8.cpp`, affecting accepted wrappers `0021DEC0`/`0021DEF4`/`00374CE8`/`00374CF0`.
  Allocation-route slot `003A8AA8+4` targets accepted `00283D70` in `group_0024C880.cpp`, whose one-argument wrapper loses the live release-buffer argument. Repair all three interfaces and preserve their accepted cohorts. Evidence: retained `2FA29C` finding and current source.

- **Threshold-preserving vector accessor:** root `00305338` (780).
  Calls `00337264` with actor and threshold 0, consuming a vector pointer. Factory `fn_00337264.cpp` accepts one private object argument and forwards only void* to `0033726C`, losing the live threshold consumed after fallthrough; `fn_00213B74.cpp` already imports the two-argument shape with private Actor/Vec3.
  Reconcile both owners with one actual actor/vector/threshold API and preserve the wrapper boundary and accepted callers. Evidence: retained `305338` finding and current source.

- **Stage-switch receiver:** root `0014C810` (788), helper `0027FCBC`.
  Factory `fn_001794C0.cpp` (confirmed O) declares unsigned-int* and passes configuration +0C; actual helper dispatches slots 0/4 of `al::IUseStageSwitch`. Repair the owner to the actual interface and preserve `001794C0`. Evidence: recovered `14c810` preflight; current rank/source.

- **Scene entry and message lookup:** root `0017E820` (788).
  Factory `group_0017C2D0.cpp` imports this one-owner root as unsigned-int(unsigned-int,unsigned-int*) through accepted fallthrough wrapper `0017E818`; `group_0014C520.cpp` defines `00242AB0` as int(int), conflicting with live char-pointer input/message-pointer output.
  Reconcile the scene and lookup contracts, preserve both groups and keep `0017E818` separate. Concrete owner+34/+5C construction ancestry remains incomplete. `00276D98`'s private Opaque import in O `fn_0035487C.cpp` is an unresolved deeper-contract question, not an established third mismatch. Evidence: recovered `17e820` preflight.

- **Time storage and intrusive-list insertion:** roots `0021BFB0` and `002BCE68` (788).
  `0021BFB0` needs `0021A5E0` for an eight-byte time output; Factory `group_0010CAB0.cpp` defines it with anonymous pointer Pair. Reconcile storage/API ownership and preserve all 21 accepted group definitions, including the genuine `0021A5BC` fallthrough boundary.
  `002BCE68` makes six three-input calls to `002323E8`; Factory `group_0022A6D4.cpp` declares four Ptr inputs. Reconcile the real three-input insertion contract and preserve accepted `0022A6D4`/`002B5590`. Existing list-dtor void*(void*) APIs in `group_00298380.cpp` are reusable; their returns are not unknown.
  Evidence: `preflight/root-21bfb0/preflight.md`, `preflight/root-2bce68/preflight.md`.

- **Other proven receiver/layout repairs:** roots `00158D70` and `00229434`.
  `00158D70`: private Object in Factory `fn_00265C2C.cpp` and DispatchObject returned by `00265E80` in `group_00277674.cpp`; unify actual interfaces and preserve owners. `00229434`: `fn_00229370.cpp` exposes float* for a mixed-layout track and reads float members at +34+4*index; correct the receiver API, not a cast-only adapter.
  Evidence: restored function notes. Ordinary owner-header extraction/correction is not automatically a driver blocker when the matching family can preserve its affected accepted cohort.

## Unresolved contracts, not established shared-declaration mismatches

- `002E8628` (784): actual non-null callback target/return at `002E88A0` remains unproved; `002AE220`: collection-entry slot +4 targets at `002AE2AC`/`002AE3F0` remain unproved. Establish actual target contracts before declaration choice; unused results do not establish void.
- `001C9970`: `0025F980` and `0027C52C` original return types remain unestablished; no conflicting accepted import was found. This uncertainty is separate from coherent CollisionParts/KCollisionServer/matrix owner-layout work that the family may perform with fresh preservation; that owner work alone is not a driver-only blocker.
- `002C2C00` (792) is source-admissible: accepted void() `002C6374` in Factory `group_002C1F24.cpp` conflicts only if implementing the separate callback's two-argument slot. This root transports the callback representation and does not call/redeclare that slot; do not block or broaden it for this issue.
- Pure missing-data holds and compiler/size mismatches are excluded. `002F46F0`, `001BBFAC`, `0021503C` and equal-size near misses have no established shared-declaration blocker from these records. Earlier vector sub/multScalarAdd prerequisites for `00249CE4` were already repaired; do not restore them as current holds.
- Owner-authorized plain-C++ iteration remains available up to eight forms, stopping early after two consecutive no-closer forms or a concrete blocker. Historical attempt counts/results remain evidence; former assistant-imposed one/few-form restrictions are superseded.
