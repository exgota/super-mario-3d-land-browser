#include <Factory/alActorFactory.h>
#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alActorInitUtil.h>
#include <Placement/alPlacementFunction.h>
#include <Scene/alSceneFunction.h>

namespace al
{

extern "C" const char dat_003B7848[]; // StageData
extern "C" const char dat_003B783C[]; // AllInfos

void initActorInitInfo( ActorInitInfo* info, const PlacementInfo* placement,
        const ActorInitInfo& baseInfo );

bool tryGetPlacementInfo( PlacementInfo* out, const Resource* stageArchive, const char* infoIterName )
{
        if ( !stageArchive )
                return false;
        const u8* stageData = stageArchive->getByml( dat_003B7848 );
        ByamlIter stageDataByaml( stageData );
        ByamlIter allInfosIter;
        bool found = stageDataByaml.tryGetIterByKey( &allInfosIter, dat_003B783C );
        if ( found )
        {
                ByamlIter unused;
                found = allInfosIter.tryGetIterByKey( out, infoIterName );
                if ( found )
                        return true;
        }
        return found;
}

#ifdef NON_MATCHING
static inline void initPlacementActors( Scene* scene, const PlacementInfo& infoIter,
        const ActorInitInfo& infoTemplate )
{
        if ( scene->getActorFactory() )
        {
                int size = infoIter.getSize();
                for ( int i = 0; i < size; i++ )
                {
                        PlacementInfo placementInfo;
                        infoIter.tryGetIterByIndex( &placementInfo, i );
                        const char* objectName = nullptr;
                        if ( al::tryGetObjectName( &objectName, placementInfo ) )
                        {
                                al::CreateActorFuncPtr create = scene->getActorFactory()->getCreator( objectName );
                                if ( create )
                                {
                                        ActorInitInfo info;
                                        initActorInitInfo( &info, &placementInfo, infoTemplate );
                                        LiveActor* actor = create( objectName );
                                        al::initCreateActorWithPlacementInfo( actor, info );
                                }
                        }
                }
        }
}

void initPlacementMap( Scene* scene, const Resource* stageArchive, const ActorInitInfo& infoTemplate, const char* infoIterName )
{
        PlacementInfo infoIter;
        if ( stageArchive && tryGetPlacementInfo( &infoIter, stageArchive, infoIterName ) )
                initPlacementActors( scene, infoIter, infoTemplate );
}
#endif


} // namespace al
