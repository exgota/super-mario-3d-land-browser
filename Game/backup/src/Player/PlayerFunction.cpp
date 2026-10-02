#include "Player/PlayerFunction.h"

#include <LiveActor/alActorPoseKeeper.h>
#include <System/Application.h>

#include "Player/PlayerActor.h"

namespace rp
{

extern "C" PlayerActor* fn_00189164();

#pragma no_inline
PlayerActor* getPlayerActor()
{
        return fn_00189164();
}

#ifdef NON_MATCHING

const sead::Vector3f& getPlayerPos()
{
        return al::getTrans( getPlayerActor() );
}
#endif

} // namespace rp
