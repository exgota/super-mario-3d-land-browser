#include <MapObj/WarpDoor.h>

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Placement/alPlacementFunction.h>

namespace al
{
bool tryGetArg0( bool* out, const ActorInitInfo& info );
}

// 0x00269A2C gets the placement camera id, then creates its camera ticket.
extern "C" void* fn_00269A2C( al::LiveActor* actor,
        const al::ActorInitInfo& info, const char* name );
// 0x0027A9D4 constructs a linked ActorInitInfo and dispatches actor->init.
extern "C" void fn_0027A9D4( al::LiveActor* actor,
        const al::ActorInitInfo& info, int childIndex );

class WarpDoorInitialNerve : public al::Nerve
{
public:
        virtual void execute( al::NerveKeeper* keeper ) const;
};
extern "C" const WarpDoorInitialNerve dat_003F2D10;

WarpDoor::WarpDoor( const sead::SafeString& name )
    : MapObjActor( name ), mPlayer( nullptr ), mNextDoor( nullptr ),
      mExitCamera( nullptr ), mPlayerSensor( nullptr ), mPlayerQuat( sead::Quatf::unit ),
      mPlayerTranslation( sead::Vector3f::zero ),
      mInitialTranslation( sead::Vector3f::zero ),
      mExitTranslation( sead::Vector3f::zero ), mReactionTimer( 0 ),
      mDoorType( 0 ), mInitiallyHidden( false )
{
}

namespace
{
// A bound destination preserves component-wise copy semantics; the shared
// vector header remains compatible with its existing users.
class WarpDoorTranslationCopy
{
        sead::Vector3f& mTarget;
public:
        explicit WarpDoorTranslationCopy( sead::Vector3f& target ) : mTarget( target ) {}
        void set( const sead::Vector3f& source )
        {
                mTarget.x = source.x;
                mTarget.y = source.y;
                mTarget.z = source.z;
        }
};
}

void WarpDoor::init( const al::ActorInitInfo& info )
{
        al::initActor( this, info );
        if ( al::isObjectName( info, "ChangeDoorFire" ) )
                mDoorType = 1;
        if ( al::isObjectName( info, "ChangeDoorRaccoonDog" ) )
                mDoorType = 2;
        WarpDoorTranslationCopy( mInitialTranslation ).set( al::getTrans( this ) );
        mExitCamera = fn_00269A2C( this, info, "èoå˚ÉJÉÅÉâ" );

        WarpDoor* previous = this;
        int childCount = al::calcLinkChildNum( info );
        if ( mDoorType == 0 )
        {
                for ( int childIndex = 0; childIndex < childCount; ++childIndex )
                {
                        WarpDoor* child = new WarpDoor(
                                al::getLinksActorObjectName( info, childIndex ) );
                        fn_0027A9D4( child, info, childIndex );
                        previous->mNextDoor = child;
                        previous = child;
                }
        }
        previous->mNextDoor = this;
        al::tryGetArg0( &mInitiallyHidden, info );
        al::initNerve( this, &dat_003F2D10 );
        if ( mInitiallyHidden )
                makeActorDead();
        else
                makeActorAppeared();
}
