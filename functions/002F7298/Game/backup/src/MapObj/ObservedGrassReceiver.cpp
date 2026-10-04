#include <MapObj/ObservedGrassReceiver.h>
#include <LiveActor/alActorDistanceImports.h>
#include <LiveActor/alHitSensorFunction.h>
#include <LiveActor/alSensorMsg.h>
#include <Nerve/alNerveFunction.h>
#include <Player/PlayerActor.h>

extern "C" {
extern const al::Nerve dat_003F2804;
extern const al::Nerve dat_003F2808;
void* fn_0026EFD8();
bool fn_0026EF88(const al::LiveActor*, float);
bool fn_00154338(al::LiveActor*);
bool fn_00273E18(unsigned);
bool fn_00280418(unsigned);
void fn_0027D760(unsigned, const al::HitSensor*, const al::HitSensor*);
}

extern "C" bool fn_002F7298(observed_grass::Receiver* actor, unsigned message,
                           al::HitSensor* other, al::HitSensor* me) {
    if (al::isNerve(actor, &dat_003F2808))
        return false;
    if (al::isSensorPlayer(other)) {
        if (al::isSensorName(me, "Fire") && al::isMsgPlayerFireBallAttack(message)) {
            if (fn_0021E2D0(actor, static_cast<PlayerActor*>(fn_0026EFD8()), 100.0f)) {
                if ((fn_00280418(message) &&
                     fn_0026EF88(static_cast<PlayerActor*>(fn_0026EFD8()), 5.0f)) ||
                    (al::isMsgPlayerTailAttack(message) && !al::isNerve(actor, &dat_003F2804))) {
                    actor->triggered = true;
                    al::setNerve(actor, &dat_003F2804);
                    return true;
                }
                return false;
            }
            fn_0027D760(message, me, other);
            if (actor->reactionActor && fn_00154338(actor->reactionActor))
                return true;
            al::setNerve(actor, &dat_003F2808);
            return true;
        }
        if (al::isSensorName(me, "Body") && !actor->triggered) {
            if ((fn_00280418(message) &&
                 fn_0026EF88(static_cast<PlayerActor*>(fn_0026EFD8()), 5.0f)) ||
                (al::isMsgPlayerTailAttack(message) && !al::isNerve(actor, &dat_003F2804))) {
                actor->triggered = true;
                al::setNerve(actor, &dat_003F2804);
                return true;
            }
            return false;
        }
    }
    if (fn_00273E18(message) || al::isMsgKickStoneAttack(message)) {
        if (al::isNerve(actor, &dat_003F2804))
            return false;
        al::setNerve(actor, &dat_003F2804);
        return false;
    }
    return false;
}
