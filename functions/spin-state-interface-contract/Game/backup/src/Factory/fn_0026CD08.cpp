#include <Enemy/SpinStateSensorContracts.h>
#include <LiveActor/alHitSensorFunction.h>
#include <Nerve/alNerveFunction.h>
#include <LiveActor/alSensorMsg.h>

extern "C" void fn_0026CD08(al::IUseNerve* state, al::HitSensor* own, al::HitSensor* other)
{
    if (al::isSensorName(own, "SpinArm") &&
        al::isSensorPlayer(other)) {
        if (fn_0032DA20(state, own, other)) {
            al::sendMsg41(other, own);
            if (!al::isNerve(state, &dat_003F2E90))
                al::sendMsgEnemyAttack(other, own);
        }
    } else if (al::isSensorName(own, "Block") &&
               al::isSensorMapObj(other) &&
               fn_0032DA20(state, own, other)) {
        fn_0027A4EC(other, own);
    }
}
