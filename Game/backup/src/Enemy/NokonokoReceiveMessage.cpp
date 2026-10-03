#include <Enemy/Nokonoko.h>
#include <Enemy/EnemyStateBlowDown.h>
#include <LiveActor/alActorActionKeeper.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alSensorMsg.h>
#include <Nerve/alNerveFunction.h>

extern "C" {
extern const al::Nerve dat_003F2288;
extern const al::Nerve dat_003F228C;
extern const al::Nerve dat_003F2290;
extern const al::Nerve dat_003F2294;
bool fn_0027A5FC(const al::IUseNerve*, int);
void fn_0031CD28(NokonokoKickShell*, const sead::Vector3f&, const sead::Quatf&, float);
bool fn_0027DB4C(u32, al::HitSensor* other, al::HitSensor* me);
bool fn_0027B180(u32);
void fn_0027D760(u32, const al::HitSensor* me, const al::HitSensor* other);
bool fn_00277D74(u32);
bool fn_0027b704(u32, al::HitSensor* other, al::HitSensor* me, al::NerveStateBase*);
}

extern "C" bool fn_00315C6C(Nokonoko* actor, u32 message,
                           al::HitSensor* other, al::HitSensor* me)
{
    if (al::isNerve(actor, &dat_003F2288) ||
        al::isNerve(actor, &dat_003F228C) ||
        al::isNerve(actor, &dat_003F2294))
        return false;
    if (al::isNerve(actor, &dat_003F2290) || al::isNerve(actor, &dat_003F228C))
    {
        if (!al::isMsgPlayerKick(message) && !al::isMsgPlayerTailAttack(message))
            return false;
        if (fn_0027A5FC(actor, 8))
            return false;
        const sead::Quatf& rotation = al::getQuat(actor);
        const sead::Vector3f& position = al::getTrans(actor);
        fn_0031CD28(actor->mKickShell, position, rotation, 0.0f);
        actor->kill();
        return true;
    }
    if (fn_0027DB4C(message, other, me) || fn_0027B180(message))
    {
        fn_0027D760(message, me, other);
        al::startHitReaction(actor, "\x93\xA5\x82\xDD");
        al::setNerve(actor, &dat_003F228C);
        return true;
    }
    if (al::isMsgPlayerTailAttack(message))
    {
        fn_0027D760(message, me, other);
        al::startHitReaction(actor, "\x82\xB5\x82\xC1\x82\xDB");
        al::setNerve(actor, &dat_003F2290);
        return true;
    }
    if (al::isMsgPlayerFireBallAttack(message))
    {
        fn_0027D760(message, me, other);
        al::startHitReaction(actor, "\x83\x74\x83\x40\x83\x43\x83\x41");
        al::setNerve(actor, &dat_003F2290);
        return true;
    }
    if (al::isMsgPlayerBoomerangAttack(message))
    {
        fn_0027D760(message, me, other);
        al::startHitReaction(actor, "\x83\x75\x81\x5B\x83\x81\x83\x89\x83\x93");
        al::setNerve(actor, &dat_003F228C);
        return true;
    }
    if (al::isMsgPlayerInvincibleAttack(message) || fn_00277D74(message) ||
        al::isMsgKickStoneAttack(message))
    {
        fn_0027b704(message, other, me, actor->mBlowDownState);
        al::setNerve(actor, &dat_003F2288);
        return true;
    }
    return false;
}
