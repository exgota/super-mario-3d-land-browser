#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActor.h>
#include <Rail/alRailFunction.h>
#include <Rail/alRail.h>
#include <Rail/alRailKeeper.h>
#include <Rail/alRailRider.h>

static inline int readRailPointInteger( const char* key, const al::Rail* rail, int index )
{
        const al::ByamlIter* iter = rail->getPointPlacementInfo( index );
        if ( iter == nullptr )
                return -1;
        int value = -1;
        iter->tryGetIntByKey( &value, key );
        return value;
}

extern "C" int fn_001D2A84( const al::LiveActor* actor, int index )
{
        return readRailPointInteger( "Arg2", actor->getRailKeeper()->getRail(), index );
}

extern "C" int fn_001D2AC8( const al::LiveActor* actor, int index )
{
        return readRailPointInteger( "Arg6", actor->getRailKeeper()->getRail(), index );
}

extern "C" int fn_001D2B0C( const al::LiveActor* actor, int index )
{
        return readRailPointInteger( "Arg7", actor->getRailKeeper()->getRail(), index );
}

extern "C" int fn_0025AB3C( const al::LiveActor* actor, int index )
{
        return readRailPointInteger( "Arg0", actor->getRailKeeper()->getRail(), index );
}

extern "C" int fn_0025AC28( const al::LiveActor* actor, int index )
{
        return readRailPointInteger( "Arg1", actor->getRailKeeper()->getRail(), index );
}

namespace al
{

#ifdef NON_MATCHING
// 4 instructions scrambled at the end
void setSyncRailToStart( LiveActor* actor )
{
        actor->getRailKeeper()->getRailRider()->moveToRailStart();
        setTrans( actor, actor->getRailKeeper()->getRailRider()->getCurrentPos() );
}
#endif

const sead::Vector3f& getRailDir( const LiveActor* actor )
{
        return actor->getRailKeeper()->getRailRider()->getCurrentDir();
}

bool isRailReachedGoal( const LiveActor* actor )
{
        return actor->getRailKeeper()->getRailRider()->isReachedGoal();
}

bool isLoopRail( const LiveActor* actor )
{
        return actor->getRailKeeper()->getRailRider()->isLoop();
}

bool isExistRail( const LiveActor* actor )
{
        if ( actor->getRailKeeper() )
                return actor->getRailKeeper()->isExistRail();
        return false;
}

} // namespace al
