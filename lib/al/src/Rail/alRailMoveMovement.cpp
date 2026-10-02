#include <LiveActor/alActorInitInfo.h>
#include <Nerve/alNerve.h>
#include <Placement/alPlacementFunction.h>
#include <Rail/alRailFunction.h>
#include <Rail/alRailMoveMovement.h>

extern "C" void fn_002694a4( al::LiveActor*, float );
extern "C" void fn_00273e28( al::LiveActor*, float );
extern "C" void fn_0027139c( al::LiveActor*, float );

namespace al
{

namespace NrvRailMoveMovement
{

NERVE_DEF( RailMoveMovement, Move )

} // namespace NrvRailMoveMovement

#ifdef NON_MATCHING
// string
RailMoveMovement::RailMoveMovement( LiveActor* host, const ActorInitInfo& info, const char* speedParamName, const char* moveTypeParamName )
    : al::HostStateBase<al::LiveActor>( host, "ƒŒ[ƒ‹ˆÚ“®‹““®" ), mSpeed( 10 ), mMoveType( 0 )
{
        tryGetArg( &mSpeed, getPlacementInfo( info ), speedParamName );
        tryGetArg( (int*)&mMoveType, getPlacementInfo( info ), moveTypeParamName );
        if ( mMoveType >= 2 )
                mMoveType = 0;
        initNerve( &NrvRailMoveMovement::Move );
}
#endif

void RailMoveMovement::exeMove()
{
        if ( isExistRail( mHost ) )
        {
                if ( mMoveType == 0 )
                        fn_002694a4( mHost, mSpeed );
                else if ( mMoveType == 1 )
                        fn_00273e28( mHost, mSpeed );
                else if ( mMoveType == 2 )
                        fn_0027139c( mHost, mSpeed );
        }
}

} // namespace al
