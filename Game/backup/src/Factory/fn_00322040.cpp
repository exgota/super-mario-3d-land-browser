#include <Factory/Observed00322040.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <Rail/alRailFunction.h>

extern "C" void fn_00322040(Observed00322040* actor)
{
    int placedCount = 0;
    sead::Vector3f base = al::getTrans(actor);
    if (al::isExistRail(actor))
        base.y = fn_0027D530(actor).y;

    for (int i = 0; i < actor->mChildCount; ++i)
    {
        if (!fn_002173E0(actor->mChildren[i]))
        {
            sead::Vector3f position(base.x, base.y + placedCount * 115.0f, base.z);
            al::setTrans(actor->mChildren[i], position);
            ++placedCount;
        }
    }

    base.y += placedCount * 115.0f;
    al::setTrans(actor, base);
    actor->mSavedTranslation.set(base);
}
