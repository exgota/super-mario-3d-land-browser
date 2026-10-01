# Pilot report: 50 game functions

Closed 2026-10-01T06:11:30-04:00. M1 passes. The final objective remains M2, 100% byte-exact. Phase 2 and the runtime lane continue without a review stop.

The fixed sample produced **40/50 byte-exact matches (80%)**, one functionally correct NonMatching binary search, and nine abandoned fragments. Its accepted intervals cover **2524 bytes**. All accepted pilot results now pass `tools/check.py --object` from committed C++ compiled directly by the normal project build, with source/header/configuration provenance. Total repository coverage is53 functions and2964 /2,756,024 function bytes (0.107546%). M0 remains incomplete at one of three compiler discriminators.

## Results by class

| Class | Functions | Matched | Match rate | NonMatching | Abandoned | Iterations | Measured compile/check seconds |
|---|---:|---:|---:|---:|---:|---:|---:|
| small | 20 | 20 | 100% | 0 | 0 | 22 | 11.829 |
| medium | 20 | 19 | 95% | 1 | 0 | 43 | 20.934 |
| large or branch heavy | 10 | 1 | 10% | 0 | 9 | 13 | 6.304 |

The sample was selected before its first attempt in commit2333f00. It consists of20 small leaf functions,20 medium functions and10 large or branch-heavy functions. All50 were unmatched at selection. It draws from clean source already present in37 Game and75 al translation units. It is a convenience sample, not a random sample of18,055 functions. Most remaining map rows have no established name or recovered source.

## Time and iteration accounting

The cohort occupied **79.58 elapsed minutes** from its first recorded compile/check to final acceptance and closure. The manifest was committed **8.43 minutes** before that first check. These are observed project-clock intervals. Source reading before manifest selection, earlier header reconstruction, compiler investigation in other lanes and unrecorded preparation cannot be allocated honestly per function.

`pilot_iterations.csv` records78 candidate iterations, including one failed compiler invocation with a missing system-header environment. Successful initial passes record their measured compile/link/check time. Revisited or abandoned ledger rows measure time from the first recorded check to acceptance or closure, including interleaved work. These elapsed times are not independent labor estimates and must not be summed to forecast the project. The final project build and provenance revalidation are verification steps, not additional candidate revisions.

`pilot_results.csv` provides the outcome, recorded attempts, elapsed minutes and direct compile/check seconds for every selected function. The table below repeats the required per-function wall time. Millisecond-scale initial passes used existing source and established headers; they do not measure full reverse engineering.

| Address | Function | Outcome | Iterations | Ledger minutes |
|---|---|---|---:|---:|
| 0x0027DCE0 | PlayerProperty::setFrontVec(sead::Vector3<float> const&) | matched | 1 | 0.009 |
| 0x0027DCC4 | PlayerProperty::setUpVec(sead::Vector3<float> const&) | matched | 1 | 0.009 |
| 0x00262BA4 | al::isCollidedGround(al::LiveActor const*) | matched | 1 | 0.009 |
| 0x001C5D10 | al::KeyPoseKeeper::KeyPoseKeeper() | matched | 1 | 0.009 |
| 0x00292308 | al::isEqualString(char const*, char const*) | matched | 1 | 0.010 |
| 0x0026D698 | al::isNearZero(float, float) | matched | 3 | 9.574 |
| 0x00269790 | al::lerpVec(sead::Vector3<float>*, sead::Vector3<float> const&, sead::Vector3<float> const&, float) | matched | 1 | 0.009 |
| 0x001C70FC | al::LiveActorFlag::LiveActorFlag() | matched | 1 | 0.009 |
| 0x002711E0 | al::CameraParamVision::CameraParamVision() | matched | 1 | 0.009 |
| 0x00187970 | al::CameraRotatorParam::CameraRotatorParam() | matched | 1 | 0.009 |
| 0x001B657C | al::CameraDashAngleTunerParam::CameraDashAngleTunerParam() | matched | 1 | 0.009 |
| 0x001DCD78 | al::StageSwitchAccesser::StageSwitchAccesser() | matched | 1 | 0.009 |
| 0x00250EB4 | al::StageSwitchAccesser::isTypeKillDeadOn() const | matched | 1 | 0.009 |
| 0x001DCD20 | al::StageResourceKeeper::StageResourceKeeper() | matched | 1 | 0.008 |
| 0x0028C2F8 | al::ByamlIter::ByamlIter(al::ByamlIter const&) | matched | 1 | 0.010 |
| 0x002910E8 | al::ByamlIter::ByamlIter() | matched | 1 | 0.009 |
| 0x00276AA0 | al::ByamlIter::isValid() const | matched | 1 | 0.009 |
| 0x0024C85C | al::ByamlIter::isTypeArray() const | matched | 1 | 0.009 |
| 0x0024FE70 | al::ByamlIter::isTypeContainer() const | matched | 1 | 0.009 |
| 0x002910F8 | al::ByamlIter::getSize() const | matched | 1 | 0.009 |
| 0x00331350 | al::ByamlHashIter::findPair(int) const | nonmatching | 8 | 66.303 |
| 0x0028B518 | al::ByamlStringTableIter::findStringIndex(char const*) const | matched | 1 | 0.009 |
| 0x001DCD2C | al::StageSwitchAccesser::initWithPlacementInfo(al::StageSwitchType, al::ByamlIter const&, al::StageSwitchType) | matched | 1 | 0.009 |
| 0x001EBB24 | al::HitSensor::validate() | matched | 2 | 15.063 |
| 0x001EB960 | al::HitSensor::invalidate() | matched | 2 | 15.073 |
| 0x001EB9FC | al::HitSensor::validateBySystem() | matched | 2 | 15.067 |
| 0x001EBA48 | al::HitSensor::invalidateBySystem() | matched | 2 | 15.077 |
| 0x001CB508 | al::SceneObjHolder::create(int) | matched | 1 | 0.009 |
| 0x001CB4B8 | al::SceneObjHolder::initAfterPlacementSceneObj(al::ActorInitInfo const&) | matched | 1 | 0.009 |
| 0x0025065C | al::NerveKeeper::update() | matched | 2 | 6.597 |
| 0x00250610 | al::NerveKeeper::NerveKeeper(al::IUseNerve*, al::Nerve const*, int) | matched | 2 | 15.053 |
| 0x001BEE64 | al::NerveKeeper::setNerve(al::Nerve const*) | matched | 1 | 0.009 |
| 0x001CABD8 | al::NerveStateCtrl::startState(al::Nerve const*) | matched | 1 | 0.009 |
| 0x001CAD2C | al::NerveStateCtrl::NerveStateCtrl(int) | matched | 2 | 15.030 |
| 0x001E2130 | al::ExecuteTableHolderDraw::ExecuteTableHolderDraw() | matched | 1 | 0.009 |
| 0x001E3B64 | al::ExecuteTableHolderUpdate::ExecuteTableHolderUpdate() | matched | 1 | 0.008 |
| 0x0018D3A0 | GhostPlayerRecorder::create(int) | matched | 3 | 16.846 |
| 0x00252008 | PlayerActionMultiCondition::append(PlayerActionCondition*) | matched | 4 | 31.364 |
| 0x001B8BD8 | PlayerActionMultiCondition::setup() | matched | 5 | 78.780 |
| 0x00314DCC | FireBall::receiveMsg(unsigned int, al::HitSensor*, al::HitSensor*) | matched | 1 | 0.010 |
| 0x0011A498 | Fugumannen::init(al::ActorInitInfo const&) | abandoned | 1 | 66.137 |
| 0x0024F344 | al::MemorySystem::createSceneResourceHeap(char const*) | abandoned | 1 | 66.120 |
| 0x00274EF0 | al::initPlacementMap(al::Scene*, al::Resource const*, al::ActorInitInfo const&, char const*) | abandoned | 1 | 66.120 |
| 0x001D1F90 | al::HitSensorDirector::HitSensorDirector() | abandoned | 1 | 66.103 |
| 0x0016A11C | CourseList::init(al::Resource const*) | abandoned | 1 | 66.103 |
| 0x0030B60C | Togezo::init(al::ActorInitInfo const&) | abandoned | 1 | 66.087 |
| 0x001CCA94 | al::ExecuteDirector::init() | abandoned | 1 | 66.087 |
| 0x00325B44 | CourseList::World::World(al::ByamlIter const*) | abandoned | 1 | 66.070 |
| 0x001B9DCC | PlayerActionConditionAnimEnd::check() | matched | 4 | 26.295 |
| 0x00330868 | al::AreaShapeCube::isInVolume(sead::Vector3<float> const&) const | abandoned | 1 | 66.053 |

## Failure modes and lessons

Unknown import and data identities blocked nine larger functions. The checker refused unmapped constructors, resource helpers, zero-vector data and compiler data sections. The compact main link independently exposed their reachable dependency gaps. They remain source fragments, with rank U and abandoned outcomes, until binary-derived identities and layouts support them. Repeating an unchanged compile cannot supply those identities.

The Byaml hash search reaches the eight-iteration limit. Its null handling,24-bit sorted keys, signed midpoint and comparison behavior are reconstructed, but its register allocation and conditional-instruction order differ. It remains NonMatching and is parked in `blocked.md`. The failed compiler invocation counts toward the cap.

Shared layout and API corrections compound. Sensor add/remove identities unblock four validation methods. Scalar/array nothrow allocation and array-delete identities unblock several constructors and the ghost recorder. NerveKeeper required a semantic fix: transition stores clear the pending nerve, then update executes the current nerve. The near-zero helper required the retail unordered floating comparison behavior. The pointer-list node constructor takes its value by value, and iteration stores an element pointer with its offset.

Control-flow structure matters after semantics are correct. Positive animation branches remove unnecessary Boolean materialization. Ordinary C++ labels in list setup preserve a shared loop condition and reproduce72 bytes. No game function uses assembly, copied instruction bytes or a per-function flag change.

The earlier direct scratch checks were provisional under the updated hard rule5. Every accepted result was rebuilt from committed source and checked again using canonical ARMCC output. Projected objects used to repair the compact main link are outside the provenance-eligible tree and cannot establish a match. The normal build now links and exports, and the normal single-function checker preserves the new setup match.

## Projection and next work

A linear projection at the observed cohort clock would take **478.9 hours (20.0 continuous days)** to attempt18,055 functions once. Dividing by the observed80% acceptance throughput gives **598.7 hours (24.9 continuous days)** for18,055 accepted functions. This is an optimistic conditional calculation, not a credible completion deadline. It assumes the same source availability, difficulty mix, shared-header benefit and lack of revisit work. Nine of ten large/branch-heavy functions did not match; unnamed source reconstruction dominates the remaining work, and this sample gives no finite upper bound for100%.

Keep the eight-candidate cap for the first Phase2 pass. Park identity blockers early with concrete evidence, then recover shared layouts, heap/container helpers and vtables to unblock groups. Continue compiler discrimination in parallel. A newly reconstructed724-byte course-map update matches diagnostically under both compilers and awaits strict canonical integration; it improves reconstruction but does not advance M0.

The runtime lane begins with loading and indexing the owner's dump and locating World1-1 resources in ignored data directories. No level has run and no differential replay has passed. Static recompilation scaffolding remains a separate future runtime component and will not count as decompiled code.

The unchanged legacy progress script runs again and reports53 matching rows,2 NonMatching rows and0.095% word similarity (2936/3,092,336 bytes). Its "Total Functions" is28,043 map rows. These values remain distinct from the full-interval exact coverage reported above.
