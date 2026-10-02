#include "MapObj/NoteObj.h"

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Placement/alPlacementFunction.h>
#include <Scene/alSceneObjHolder.h>

#include "MapObj/CoinRotater.h"

NoteObj::NoteObj( const char* name )
    : MapObjActor( name ), mStartQuat( sead::Quatf::unit ), _70( false ), _71( true ), _74( -1 ),
      _78( sead::Vector3f::zero ), mGenerator( nullptr )
{
        rp::createCoinRotater();
}

NoteObj::NoteObj( NoteObjGenerator* generator )
    : MapObjActor( "音符オブジェ" ), mStartQuat( sead::Quatf::unit ), _70( false ), _71( true ), _74( -1 ),
      _78( sead::Vector3f::zero ), mGenerator( generator )
{
        rp::createCoinRotater();
}

extern "C" void fn_00270fc4( al::LiveActor*, float, int );

extern "C" const char dat_003BEABC[];
extern "C" bool fn_00280418( u32 msg );
extern "C" bool fn_00277D74( u32 msg );
extern "C" void fn_001710D4( NoteObjGenerator* generator, NoteObj* note );
extern "C" void fn_00279e8c( al::LiveActor* actor, float amount );
extern "C" void fn_0026C758( al::LiveActor* actor, const sead::Quatf& from, float degrees );

namespace al
{
bool isSensorPlayer( const HitSensor* sensor );
bool isMsgPlayerBoomerangAttack( u32 msg );
void startHitReaction( const LiveActor* actor, const char* reactionName );
}

namespace
{
inline void applyNoteGravity( al::LiveActor* actor, bool fixed )
{
        if ( !fixed )
                fn_00279e8c( actor, 0.5f );
}
}

static const char* sNoteObjArchive = "NoteObj";

#ifdef NON_MATCHING
void NoteObj::init( const al::ActorInitInfo& info )
{
        if ( al::isPlaced( info ) )
        {
                al::initActorWithArchiveName( this, info, sNoteObjArchive );
                mStartQuat = al::getQuat( this );
                fn_00270fc4( this, 70.0, 1 );
                _78 = al::getTrans( this );
                al::invalidateClipping( this );
        }
        al::initActorWithArchiveNameNoPlacementInfo( this, info, sNoteObjArchive );
        makeActorDead();
}
#endif

void NoteObj::initAfterPlacement()
{
}

void NoteObj::control()
{
        applyNoteGravity( this, _71 );
        fn_0026C758( this, mStartQuat, rp::getCoinRotateY() );
}

bool NoteObj::receiveMsg( u32 msg, al::HitSensor* other, al::HitSensor* me )
{
        if ( ( al::isSensorPlayer( other ) && fn_00280418( msg ) ) ||
                fn_00277D74( msg ) || al::isMsgPlayerBoomerangAttack( msg ) )
        {
                if ( _70 )
                        return false;
                al::startHitReaction( this, dat_003BEABC );
                _70 = true;
                kill();
                if ( mGenerator )
                        fn_001710D4( mGenerator, this );
                return true;
        }
        return false;
}
