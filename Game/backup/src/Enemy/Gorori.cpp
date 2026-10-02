#include "Enemy/Gorori.h"

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Placement/alPlacementFunction.h>
#include <Util/alStringUtil.h>

// Address-named imports have existing retail map rows. Their observed contracts
// are limited to the uses here; names for unrecovered operations stay neutral.
extern "C" float fn_002700E0( const al::LiveActor* actor );
extern "C" al::LiveActor* fn_00218950( al::LiveActor* actor,
        const al::ActorInitInfo& info, const char* suffix, const char* action );
extern "C" const char* fn_00227CE4( const al::LiveActor* actor );
extern "C" void fn_0027ee34( al::LiveActor* actor, const al::ActorInitInfo& info, int count );
extern "C" void fn_0027cf20( al::LiveActor* actor, const al::ActorInitInfo& info, int count );
extern "C" void fn_001EA31C( al::IUseEffectKeeper* actor, const char* name,
        const sead::Matrix34f* matrix );
extern "C" void fn_00270E68( al::LiveActor* actor );

extern "C" const char dat_003BB074[];
extern "C" const char dat_003BB080[];
extern "C" const char dat_003BB088[];
extern "C" const char dat_003BB090[];
extern "C" const al::Nerve dat_003F2058;

// Independently emitted at 0x00305ECC and inlined by this initializer.
inline void Gorori::setMoveSpeed( int speed )
{
        mMoveSpeed = speed;
        mMoveFrames = static_cast<int>( 2.0f * fn_002700E0( this ) / mMoveSpeed );
}

// The same complete phase also appears in the archive-name initializer at
// 0x00305074; it starts after archive loading in both entry points.
inline void Gorori::initCommon( const al::ActorInitInfo& info )
{
        mBreakActor = fn_00218950( this, info, nullptr, dat_003BB080 );
        al::initNerve( this, &dat_003F2058 );
        if ( al::isEqualString( fn_00227CE4( this ), dat_003BB074 ) )
                fn_0027ee34( this, info, 3 );
        else
                fn_0027cf20( this, info, 1 );

        fn_001EA31C( this, dat_003BB090, &mMoveEffectMtx );
        fn_001EA31C( this, dat_003BB088, &mLandEffectMtx );
        makeActorAppeared();
        al::offCollide( this );
        fn_00270E68( this );
}

void Gorori::init( const al::ActorInitInfo& info )
{
        al::initActorPoseTQSV( this );
        const char* objectName = nullptr;
        const char* archiveName = "Gorori";
        if ( al::tryGetObjectName( &objectName, info )
             && ( al::isEqualString( objectName, "GororiBigGenerator" )
                  || al::isEqualString( objectName, "GororiBigGeneratorAir" ) ) )
                archiveName = "GororiBig";
        if ( mForceBig )
                archiveName = "GororiBig";
        al::initActorWithArchiveName( this, info, archiveName );

        if ( al::isPlaced( info ) )
        {
                float speed;
                al::tryGetArg0( &speed, info );
                setMoveSpeed( static_cast<int>( speed ) );
        }

        initCommon( info );
}
