#include <Factory/Observed00344BE0.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerveFunction.h>

extern "C" void fn_00344BE0( const al::Nerve*, al::NerveKeeper* keeper )
{
    ObservedActor00344BE0* actor =
        static_cast<ObservedActor00344BE0*>( keeper->getHost() );
    if (al::isLessStep(actor, 2))
        return;
    if (!al::isGreaterEqualStep(actor, 10)) {
        if (!fn_00256B58(actor, 2))
            return;
        if (fn_00263BB8(0, 3))
            return;
    }

    sead::Vector3f direction = al::getTrans(actor) - *fn_0026CCD0();
    fn_0027306C(&direction, &sead::Vector3f::ey, &direction);
    fn_0026AB58(&direction);
    actor->vector7c = direction;
    fn_0027D4D8(al::getQuatPtr(actor), &actor->vector7c, &sead::Vector3f::ey);
    actor->vector7c.y = 1.0f;
    fn_0027D5C4(&actor->vector7c);
    al::invalidateClipping(actor);
    al::setNerve(actor, &dat_003F256C);
}
