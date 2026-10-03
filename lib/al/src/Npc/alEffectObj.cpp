#include <LiveActor/alActorInitializationImports.h>
#include <File/alFileFunction.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Npc/alEffectObj.h>
#include <Placement/alPlacementFunction.h>
#include <Se/alSeFunction.h>
#include <Util/alStringUtil.h>

extern "C" void fn_00266EF4( sead::Matrix34f* matrix, const al::LiveActor* actor );
extern "C" void fn_0027BEA0( al::IUseEffectKeeper* effectUser, const char* name, const sead::Vector3f* position );
extern "C" void fn_002796C0( al::IUseEffectKeeper* effectUser, const char* name );
extern "C" int fn_00262810( al::IUseAudioKeeper* audioUser, const sead::SafeString& name, int parameter );

namespace
{
struct EffectArchivePathBuffer
{
        void* mVirtualTable;
        char* mStringTop;
        int mBufferSize;
        char mBuffer[ 128 ];
};
}

extern "C" const sead::SafeString& fn_0028CB38( EffectArchivePathBuffer& path, const char* format, ... );
extern "C" bool fn_0032F3C8( const sead::SafeString& archive );
extern "C" void fn_001EBDDC( al::LiveActor* actor, const al::ActorInitInfo& info, const char* objectName );
extern "C" const char dat_003B142C[];
extern "C" const char dat_003B1440[];

namespace al
{

#ifdef NON_MATCHING
// something with mBaseMtx
EffectObj::EffectObj( const sead::SafeString& name )
    : MapObjActor( name ), mBaseMtx( sead::Matrix34f::ident )
{
}
#endif

void EffectObj::init( const ActorInitInfo& info )
{
        const char* objectName = nullptr;
        tryGetObjectName( &objectName, info );
        EffectObjFunction::initActorEffectObj( this, info, objectName );
}

void EffectObj::makeActorAppeared()
{
        LiveActor::makeActorAppeared();
        fn_00266EF4( &mBaseMtx, this );
        fn_0027BEA0( this, "Wait", nullptr );
}

void EffectObj::kill()
{
        fn_002796C0( this, "Wait" );
        LiveActor::kill();
}

const sead::Matrix34f* EffectObj::getBaseMtx() const
{
        return &mBaseMtx;
}

void EffectObj::control()
{
        fn_00266EF4( &mBaseMtx, this );
        fn_00262810( this, "Wait", 2 );
}

void EffectObjFunction::initActorEffectObj( EffectObj* actor, const ActorInitInfo& info, const char* objectName )
{
        EffectArchivePathBuffer archivePath;
        if ( ::fn_0032F3C8( ::fn_0028CB38( archivePath, dat_003B142C, objectName ) ) )
                initActor( actor, info );
        else
                initActorWithArchiveName( actor, info, dat_003B1440 );
        ::fn_001EBDDC( actor, info, objectName );
        ::fn_00266EF4( &actor->mBaseMtx, actor );
        ::fn_0027FAB8( actor );
}

} // namespace al
