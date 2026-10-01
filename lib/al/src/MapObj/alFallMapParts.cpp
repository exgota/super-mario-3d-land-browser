#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alSensorMsg.h>
#include <MapObj/alFallMapParts.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>
#include <Nerve/alNerveKeeper.h>
#include <Placement/alPlacementFunction.h>

extern "C" const char dat_003bd9dc[];
extern "C" const char dat_003bd9e4[];
extern "C" const char dat_003bd9f8[];
extern "C" const char dat_003bda04[];
extern "C" const char dat_003bda0c[];
extern "C" bool fn_0027063c( al::LiveActor*, const char* );
extern "C" void fn_0026a9fc( al::LiveActor* );
extern "C" void fn_00279e8c( al::LiveActor*, float );
extern "C" void fn_00279e5c( al::LiveActor*, float );

extern "C" void fn_0027C05C( al::LiveActor* );
extern "C" void fn_0026FB1C( al::LiveActor* );

extern "C" void fn_001C96B8( al::LiveActor* );

namespace al
{

namespace NrvFallMapParts
{

NERVE_DEF( FallMapParts, Appear );
NERVE_DEF( FallMapParts, Wait );
NERVE_DEF( FallMapParts, FallSign );
NERVE_DEF( FallMapParts, Fall );
NERVE_DEF( FallMapParts, End );

} // namespace NrvFallMapParts

#ifdef NON_MATCHING
// float
FallMapParts::FallMapParts( const sead::SafeString& name )
    : MapObjActor( name ), mStartTrans( sead::Vector3f::zero ), mFallFrames( 60 ), _70( false )
{
}
#endif

#ifdef NON_MATCHING
// instruction swap
void FallMapParts::init( const ActorInitInfo& info )
{
        initActorPoseTQSV( this );
        initMapPartsActor( this, info ); // this should be a thunk
        mStartTrans = getTrans( this );
        tryGetArg0( &mFallFrames, getPlacementInfo( info ) );
        initNerve( this, &NrvFallMapParts::Wait );
        initStageSwitchAppear( this, info );
        trySyncStageSwitchAppear( this );
}
#endif

bool FallMapParts::receiveMsg( u32 msg, HitSensor* other, HitSensor* me )
{
        if ( isMsg52( msg ) )
        {
                setNerve( this, &NrvFallMapParts::End );
                return true;
        }
        if ( isMsgPlayerFloorTouch( msg ) && isNerve( this, &NrvFallMapParts::Wait ) )
        {
                setNerve( this, &NrvFallMapParts::FallSign );
                invalidateClipping( this );
                return true;
        }
        return false;
}

void FallMapParts::exeAppear()
{
        if ( isFirstStep( this ) )
        {
                fn_0026FB1C( this );
                if ( !fn_0027063c( this, dat_003bd9dc ) )
                        goto end; // ?
        }
        if ( isActionEnd( this ) )
        end:
                setNerve( this, &NrvFallMapParts::Wait );
}

void FallMapParts::exeWait()
{
        if ( isFirstStep( this ) )
        {
                fn_0027063c( this, dat_003bd9e4 );
                validateClipping( this );
        }
}

#ifdef NON_MATCHING
void FallMapParts::exeFallSign()
{
}
#endif

extern "C" bool fn_00268df8( IUseAudioKeeper*,
        const sead::SafeString& name ); // something with sound

void FallMapParts::exeFall()
{
        if ( isFirstStep( this ) )
        {
                fn_0027063c( this, dat_003bda04 );
                fn_00268df8( this, dat_003bd9f8 );
                setTrans( this, mStartTrans );
        }
        fn_00279e8c( this, 1.0 );
        fn_00279e5c( this, 0.9 );
        if ( isGreaterStep( this, mFallFrames ) )
                setNerve( this, &NrvFallMapParts::End );
}

void FallMapParts::exeEnd()
{
        if ( isFirstStep( this ) )
        {
                fn_0027063c( this, dat_003bda0c );
                hideModel( this );
                fn_001C96B8( this );
                setVelocityZero( this );
        }
        if ( isGreaterStep( this, 60 ) )
        {
                setTrans( this, mStartTrans );
                fn_0027C05C( this );
                fn_0026a9fc( this );
                setNerve( this, &NrvFallMapParts::Appear );
        }
}

} // namespace al
