#include <LiveActor/alActorInitializationImports.h>
#include "MapObj/AquariumSwimDebris.h"

#include <LiveActor/alActorActionKeeper.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>
#include <Placement/alPlacementFunction.h>
#include <Stage/alStageSwitchKeeper.h>

extern "C" const char dat_003BCDFC[];

namespace NrvAquariumSwimDebris
{

NERVE_DEF( AquariumSwimDebris, Appear );

} // namespace NrvAquariumSwimDebris

AquariumSwimDebris::AquariumSwimDebris( const sead::SafeString& name ) : MapObjActor( name )
{
}

void AquariumSwimDebris::init( const al::ActorInitInfo& info )
{
        al::initActorWithArchiveName( this, info, "AquariumSwimDebris", nullptr );
        al::initNerve( this, &NrvAquariumSwimDebris::Appear, 1 );
        ::fn_00280538( this, info );
        ::fn_0027FAB8( this );
}

void AquariumSwimDebris::exeAppear()
{
        if ( al::isFirstStep( this ) )
                al::startHitReaction( this, dat_003BCDFC );
}
