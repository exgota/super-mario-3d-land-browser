#include "Enemy/Bubble.h"
#include "Enemy/EnemyStateBlowDown.h"

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alHitSensorFunction.h>
#include <LiveActor/alSensorMsg.h>
#include <Nerve/alNerveFunction.h>
#include <Nerve/alNerve.h>
#include <Placement/alPlacementFunction.h>
#include <Stage/alStageSwitchKeeper.h>

struct BubbleInitialNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};

struct BubbleSwitchNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};

struct BubbleBlowDownNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};

extern "C" const BubbleInitialNerve dat_003F1EF8;
extern "C" const BubbleSwitchNerve dat_003F1EF0;
extern "C" const BubbleBlowDownNerve dat_003F1F00;
extern "C" bool fn_0027D180( int* value, const al::ActorInitInfo& info );
extern "C" void fn_00270FC4( al::LiveActor* actor, float amount, int direction );
extern "C" void fn_0027CF20( al::LiveActor* actor, const al::ActorInitInfo& info, int mode );
extern "C" bool fn_00279B64( al::IUseStageSwitch* actor );
extern "C" bool fn_00218B90( al::HitSensor* other, al::HitSensor* me );

Bubble::Bubble( const sead::SafeString& name )
    : MapObjActor( name ), _60( 0 ), _64( 30 ), _68( nullptr ), _6C( 500.0f ), _70( 2.4f ),
      _74( sead::Vector3f::zero ), _80( sead::Quatf::unit ), _90( nullptr )
{
}

namespace
{
class BubbleTranslationCopy
{
        sead::Vector3f& mTarget;
public:
        explicit BubbleTranslationCopy( sead::Vector3f& target ) : mTarget( target ) {}
        void set( const sead::Vector3f& source )
        {
                mTarget.x = source.x;
                mTarget.y = source.y;
                mTarget.z = source.z;
        }
};

class BubbleQuaternionCopy
{
        sead::Quatf& mTarget;
public:
        explicit BubbleQuaternionCopy( sead::Quatf& target ) : mTarget( target ) {}
        void set( const sead::Quatf& source )
        {
                mTarget.x = source.x;
                mTarget.y = source.y;
                mTarget.z = source.z;
                mTarget.w = source.w;
        }
};
}

void Bubble::init( const al::ActorInitInfo& info )
{
        al::initActor( this, info );
        al::tryGetArg0( &_6C, info );
        al::tryGetArg1( &_70, info );
        fn_0027D180( &_64, info );
        al::tryGetArg3( &_60, info );
        fn_00270FC4( this, -50.0f, 1 );
        BubbleTranslationCopy( _74 ).set( al::getTrans( this ) );
        BubbleQuaternionCopy( _80 ).set( al::getQuat( this ) );
        _90 = new EnemyStateBlowDown( this, nullptr, nullptr, 0 );
        fn_0027CF20( this, info, 1 );
        if ( fn_00279B64( this ) )
                al::initNerve( this, &dat_003F1EF0, 1 );
        else
                al::initNerve( this, &dat_003F1EF8, 1 );
        al::initNerveState( this, _90, &dat_003F1F00, "state:BlowDown" );
        al::offCollide( this );
        makeActorAppeared();
}

void Bubble::attackSensor( al::HitSensor* me, al::HitSensor* other )
{
        if ( al::isNerve( this, &dat_003F1F00 ) )
                return;
        if ( !al::isSensorName( me, "Attack" ) )
                return;
        if ( al::isSensorPlayer( other ) )
        {
                if ( !al::sendMsg50( other, me ) )
                        fn_00218B90( other, me );
        }
        else
                al::sendMsg50( other, me );
}

