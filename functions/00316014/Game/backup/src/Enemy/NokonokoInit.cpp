#include <Enemy/Nokonoko.h>
#include <Enemy/NokonokoKickShell.h>
#include <Enemy/EnemyStateBlowDown.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorInitializationImports.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerveFunction.h>
#include <Rail/alRailFunction.h>

extern "C" {
extern const al::Nerve dat_003F2274;
extern const al::Nerve dat_003F2288;
void fn_00273F7C(al::LiveActor*);
void fn_0027CF6C(sead::Vector3f*, al::LiveActor*, float, float);
NokonokoJointControl* fn_0026CA54(al::LiveActor*, int);
void fn_001E5B8C(NokonokoJointControl*, al::LiveActor*, const char*,
                 const sead::Vector2f&, const sead::Vector2f&,
                 const sead::Vector3f&, const sead::Vector3f&, float);
void fn_0027CF20(al::LiveActor*, const al::ActorInitInfo&, int);
}

static inline sead::Vector3f negateHeadAxis(const sead::Vector3f& axis)
{
    sead::Vector3f result;
    result.x = -axis.x;
    result.y = -axis.y;
    result.z = -axis.z;
    return result;
}

// Non-matching reconstruction of the complete initializer. The retained
// virtual declaration identifies the original slot; this neutral entry is
// checked independently without emitting a reconstructed actor vtable.
extern "C" void fn_00316014(Nokonoko* actor, const al::ActorInitInfo& info)
{
    al::initActorWithArchiveName(actor, info, "Nokonoko", 0);
    if (al::isExistRail(actor))
    {
        fn_00273F7C(actor);
        fn_0027CF6C(&actor->mRailDirection, actor, 100.0f, 100.0f);
        al::offCollide(actor);
    }
    else
        al::calcFrontDir(&actor->mFrontDirection, actor);

    actor->mKickShell = new NokonokoKickShell(
        "\x83\x6d\x83\x52\x83\x6d\x83\x52\x97\x70\x83\x4c\x83\x62\x83\x4e\x8d\x62\x97\x85");
    actor->mKickShell->init(info);
    actor->mKickShell->makeActorDead();
    actor->mHeadControl = fn_0026CA54(actor, 1);
    fn_001E5B8C(actor->mHeadControl, actor, "Head",
                 sead::Vector2f(-30.0f, 30.0f),
                 sead::Vector2f(-30.0f, 30.0f),
                 negateHeadAxis(sead::Vector3f::ey),
                 sead::Vector3f::ex, 0.07f);
    fn_0027CF20(actor, info, 1);
    al::initNerve(actor, &dat_003F2274, 1);
    actor->mBlowDownState = new EnemyStateBlowDown(actor, 0, 0, 0);
    al::initNerveState(actor, actor->mBlowDownState, &dat_003F2288, "state:Blow");
    fn_0027FAB8(actor);
}
