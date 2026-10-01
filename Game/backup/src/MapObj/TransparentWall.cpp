#include "MapObj/TransparentWall.h"

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Placement/alPlacementFunction.h>
#include <Stage/alStageSwitchKeeper.h>

extern "C" void fn_00280538( al::IUseStageSwitch* receiver, const al::ActorInitInfo& info );
extern "C" bool fn_0027FAB8( al::LiveActor* actor );
extern "C" void fn_00270724( al::IUseStageSwitch* receiver, const al::ActorInitInfo& info );
extern "C" void fn_0027AE3C( al::LiveActor* actor );

TransparentWall::TransparentWall( const sead::SafeString& name ) : MapObjActor( name )
{
}

void TransparentWall::init( const al::ActorInitInfo& info )
{
        if ( al::isObjectName( info, "TransparentWallMoveLimit" ) )
                al::initActorWithArchiveName( this, info, "TransparentWall", "MoveLimit" );
        else
                al::initActor( this, info );
        ::fn_00280538( this, info );
        ::fn_0027FAB8( this );
        ::fn_00270724( this, info );
        ::fn_0027AE3C( this );
}

void TransparentWall::makeActorDead()
{
        LiveActor::makeActorDead();
}
