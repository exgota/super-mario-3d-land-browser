#include "Enemy/Teresa.h"

#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Placement/alPlacementFunction.h>

struct TeresaInitialNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};

extern "C" const TeresaInitialNerve dat_003F23E0;
extern "C" bool fn_002794F8( int* out, const al::ActorInitInfo& info );
extern "C" void fn_0027cf20( al::LiveActor* actor, const al::ActorInitInfo& info, int count );
extern "C" void fn_00227988( al::LiveActor* actor, const al::ActorInitInfo& info );
extern "C" void fn_00280538( al::IUseStageSwitch* actor, const al::ActorInitInfo& info );
extern "C" bool fn_0027FAB8( al::LiveActor* actor );
extern "C" TeresaChildAreaGroup* fn_0026F410( al::LiveActor* actor,
        const al::ActorInitInfo& info, const char* filter, const char* groupName,
        const char* areaName );
extern "C" void* fn_0026F404( void* holder, int index );

namespace
{
class TeresaTranslationCopy
{
        sead::Vector3f& mTarget;
public:
        explicit TeresaTranslationCopy( sead::Vector3f& target ) : mTarget( target ) {}
        void set( const sead::Vector3f& source )
        {
                mTarget.x = source.x;
                mTarget.y = source.y;
                mTarget.z = source.z;
        }
};
}

void Teresa::init( const al::ActorInitInfo& info )
{
        const char* objectName = nullptr;
        if ( !al::tryGetObjectName( &objectName, info ) )
        {
                al::LiveActor::kill();
                return;
        }
        fn_002794F8( &mArg0, info );
        al::initActorWithArchiveName( this, info, objectName );
        if ( al::isObjectName( info, "TeresaBig" ) )
                mCoinCount = 3;
        else
                mCoinCount = 1;
        fn_0027cf20( this, info, mCoinCount );
        if ( al::isObjectName( info, "Teresa" ) )
                mKind = 0;
        if ( al::isObjectName( info, "TeresaBig" ) )
                mKind = 1;
        if ( al::isObjectName( info, "TeresaTail" ) )
                mKind = 2;
        if ( al::isObjectName( info, "TeresaTail" ) && mHost )
                fn_00227988( this, info );
        al::initNerve( this, &dat_003F23E0, 0 );
        TeresaTranslationCopy( mInitialTrans ).set( al::getTrans( this ) );
        TeresaTranslationCopy( mHomeTrans ).set( mInitialTrans );
        fn_00280538( this, info );
        if ( fn_0027FAB8( this ) )
                makeActorDead();
        if ( !mChildAreas )
                mChildAreas = fn_0026F410( this, info, nullptr, "子供エリアグループ", "子供エリア" );
        if ( mChildAreas && mChildAreas->mCount > 0 )
                mChildArea = fn_0026F404( mChildAreas, 0 );
}
