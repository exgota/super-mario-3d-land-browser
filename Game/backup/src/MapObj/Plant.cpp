#include "MapObj/Plant.h"
#include "Enemy/EnemyStateBlowDown.h"

#include <LiveActor/alActorInitializationImports.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>

namespace al
{
bool tryGetArg1( bool* out, const ActorInitInfo& info );
bool tryGetArg2( bool* out, const ActorInitInfo& info );
}

struct PlantInitialNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};
struct PlantBlowDownNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};
extern "C" const PlantInitialNerve dat_003F2AB4;
extern "C" const PlantBlowDownNerve dat_003F2AC0;

// Randomize the actor quaternion around Y, preserving its existing rotation.
extern "C" void fn_0026EF68( al::LiveActor* actor );
// Configure the item encoded after the given prefix in the placement name.
extern "C" int fn_002D11E4( al::LiveActor* actor,
        const al::ActorInitInfo& info, const char* objectNamePrefix );
extern "C" float fn_00332C74( const al::LiveActor* actor );
extern "C" void fn_002687D0( al::LiveActor* actor, float frame );

void Plant::init( const al::ActorInitInfo& info )
{
        al::initActorWithArchiveName( this, info, "Plant" );
        al::initNerve( this, &dat_003F2AB4, 1 );
        mInitialQuat.set( al::getQuat( this ) );
        fn_0026EF68( this );
        mBlowDownState = new EnemyStateBlowDown( this, nullptr, nullptr, 0 );
        al::initNerveState( this, mBlowDownState, &dat_003F2AC0, "state:BlowDown" );
        mItemType = fn_002D11E4( this, info, "Plant" );

        int color = 0;
        fn_002794F8( &color, info );
        if ( al::tryStartMclAnimIfExist( this, "Color" ) )
        {
                int lastFrame = static_cast<int>( fn_00332C74( this ) );
                if ( color > lastFrame )
                        color = lastFrame;
                if ( color < 0 )
                        color = 0;
                fn_002687D0( this, static_cast<float>( color ) );
        }

        bool hasPot = false;
        al::tryGetArg1( &hasPot, info );
        if ( hasPot )
        {
                al::LiveActor* pot = new al::LiveActor( "\x94\xAB\x83\x82\x83\x66\x83\x8B" );
                al::initActorWithArchiveName( pot, info, "FlowerPot" );
                pot->makeActorAppeared();
        }

        bool hasTrace = true;
        al::tryGetArg2( &hasTrace, info );
        if ( hasTrace )
        {
                mTrace = new al::LiveActor( "\x90\xD5" );
                al::initActorWithArchiveName( mTrace, info, "PlantTrace" );
                mTrace->makeActorDead();
        }
        makeActorAppeared();
        al::offCollide( this );
}
