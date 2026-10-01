#include <LiveActor/alActorInitByaml.h>
#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActor.h>
#include <Placement/alPlacementFunction.h>
#include <Util/alStringUtil.h>

namespace al
{

#ifdef NON_MATCHING

// SafeString construction backwards
void initActor( LiveActor* actor, const ActorInitInfo& info )
{
        const char* objectName = nullptr;
        tryGetObjectName( &objectName, info );
        fn_002417E8( actor, info, objectName, StringTmp<256>( "ObjectData/%s", objectName ), nullptr );
}

// ???
void initActorWithArchiveName( LiveActor* actor, const ActorInitInfo& info, const sead::SafeString& archiveName, const char* suffix )
{
        fn_002417E8( actor, info, archiveName.cstr(), StringTmp<256>( "ObjectData/%s", archiveName.cstr() ), suffix );
}
#endif

void initCreateActorNoPlacementInfo( LiveActor* actor, const ActorInitInfo& hostInfo )
{
        PlacementInfo placementInfo;
        ActorInitInfo info;
        info.initViewIdHost( &placementInfo, hostInfo );
        actor->init( info );
}

void initCreateActorWithPlacementInfo( LiveActor* actor, const ActorInitInfo& hostInfo )
{
        actor->init( hostInfo );
}

} // namespace al
