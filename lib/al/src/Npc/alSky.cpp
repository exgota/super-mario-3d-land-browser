#include <Camera/alCamera.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Npc/alSky.h>
#include <Stage/alStageSwitchKeeper.h>

extern "C" const sead::Vector3f* fn_0026CCD0();
extern "C" void fn_00280538( al::IUseStageSwitch* receiver, const al::ActorInitInfo& info );
extern "C" bool fn_0027FAB8( al::LiveActor* actor );

namespace al
{

Sky::Sky( const char* name ) : MapObjActor( name ), mCameraTransPtr( nullptr )
{
}

void Sky::init( const ActorInitInfo& info )
{
        initActor( this, info );
        ::fn_00280538( this, info );
        mCameraTransPtr = ::fn_0026CCD0();
        invalidateClipping( this );
        makeActorAppeared();
        ::fn_0027FAB8( this );
}

void Sky::calcAnim()
{
        setTrans( this, *mCameraTransPtr );
        LiveActor::calcAnim();
}

} // namespace al
