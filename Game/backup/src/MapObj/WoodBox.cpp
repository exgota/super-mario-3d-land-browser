#include "MapObj/WoodBox.h"
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alSensorMsg.h>
#include <Nerve/alNerveFunction.h>

extern "C" const al::Nerve dat_003F2D6C;
extern "C" const char dat_003BFDB0[],dat_003BFDBC[],dat_003BFDC8[];
extern "C" void fn_001C96B8(al::LiveActor*);
extern "C" bool fn_00277D74(unsigned);
extern "C" bool fn_002CD9C8(const al::HitSensor*,const al::HitSensor*,float);
extern "C" void fn_002784BC(unsigned,al::LiveActor*);
extern "C" void fn_00270E74(al::LiveActor*);
extern "C" bool fn_0026503C(unsigned);
extern "C" bool fn_0027F1A8(unsigned);
extern "C" bool fn_0027F188(unsigned);
extern "C" bool fn_0027F228(unsigned);

inline void WoodBox::finishBreak(unsigned msg)
{
    WoodBox* actor=this;
    fn_001C96B8(actor);
    fn_002784BC(msg,actor);
    fn_00270E74(actor->mBreakActor);
    if (actor->mWater)
        al::startAction(actor->mBreakActor,dat_003BFDB0);
    else if (al::isMsgPlayerFireBallAttack(msg) || fn_0026503C(msg))
        al::startAction(actor->mBreakActor,dat_003BFDBC);
    else
        al::startAction(actor->mBreakActor,dat_003BFDC8);
    al::setNerve(actor,&dat_003F2D6C);
}
bool WoodBox::receiveMsg(u32 msg,al::HitSensor* other,al::HitSensor* me)
{
    WoodBox* actor=this;
    if (al::isNerve(actor,&dat_003F2D6C)) return false;
    if ((al::isMsgPlayerTailAttack(msg) || fn_00277D74(msg) ||
         al::isMsgPlayerBoomerangAttack(msg)) && fn_002CD9C8(other,me,150.0f)) {
        finishBreak(msg);
        return true;
    }
    if (al::isMsgPlayerRollingAttack(msg) || al::isMsgPlayerFireBallAttack(msg) ||
        fn_00277D74(msg) || al::isMsgGororiAttack(msg) || fn_0027F1A8(msg) ||
        al::isMsgGororiBigAttack(msg) || al::isMsgPlayerInvincibleTouch(msg) ||
        fn_0027F188(msg) || fn_0027F228(msg)) {
        finishBreak(msg);
        return true;
    }
    return false;
}
