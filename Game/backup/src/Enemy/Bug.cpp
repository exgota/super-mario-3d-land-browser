#include <LiveActor/alActorInitializationImports.h>
#include "Enemy/Bug.h"
#include "Enemy/EnemyStateBlowDown.h"
#include "Enemy/EnemyStateHipDropDown.h"

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>

struct BugInitialNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};
struct BugBlowDownNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};
struct BugHipDropDownNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};

extern "C" const BugInitialNerve dat_003F1F08;
extern "C" const BugBlowDownNerve dat_003F1F2C;
extern "C" const BugHipDropDownNerve dat_003F1F28;
extern "C" void fn_00270FC4( al::LiveActor* actor, float amount, int direction );
extern "C" const char* fn_0026C984( const al::LiveActor* actor, const char* action );
extern "C" void fn_0027CF20( al::LiveActor* actor, const al::ActorInitInfo& info, int mode );
extern "C" void fn_002535C8( al::LiveActor* actor, bool enabled );
extern "C" const sead::Vector3f& fn_00273DE0( const al::LiveActor* actor, int index );
extern "C" void fn_00273BD4( al::LiveActor* actor, const sead::Vector3f& offset, int index );

struct BugActionNames
{
        const char* mWait;
        const char* mMoveFar;
        const char* mMoveNear;

        explicit BugActionNames( const al::LiveActor* actor )
        {
                mWait = fn_0026C984( actor, "Wait" );
                mMoveFar = fn_0026C984( actor, "MoveFar" );
                mMoveNear = fn_0026C984( actor, "MoveNear" );
        }
};

namespace
{
class BugTranslationCopy
{
        sead::Vector3f& mTarget;
public:
        explicit BugTranslationCopy( sead::Vector3f& target ) : mTarget( target ) {}
        void set( const sead::Vector3f& source )
        {
                mTarget.x = source.x;
                mTarget.y = source.y;
                mTarget.z = source.z;
        }
};
}

void Bug::init( const al::ActorInitInfo& info )
{
        al::initActor( this, info );
        al::initNerve( this, &dat_003F1F08, 2 );
        fn_00270FC4( this, 80.0f, 1 );
        BugTranslationCopy( mStartTrans ).set( al::getTrans( this ) );
        mActionNames = new BugActionNames( this );
        mBlowDownState = new EnemyStateBlowDown( this, nullptr, nullptr, 0 );
        al::initNerveState( this, mBlowDownState, &dat_003F1F2C, "state::BlowDown" );
        mHipDropDownState = new EnemyStateHipDropDown( this, nullptr, nullptr );
        al::initNerveState( this, mHipDropDownState, &dat_003F1F28, "state:HipDropDown" );
        fn_0027CF20( this, info, 1 );
        fn_002535C8( this, false );
        int shadowHeight = -1;
        if ( fn_002794F8( &shadowHeight, info ) )
        {
                sead::Vector3f offset = fn_00273DE0( this, 0 );
                offset.y = shadowHeight;
                fn_00273BD4( this, offset, 0 );
        }
        makeActorAppeared();
}
