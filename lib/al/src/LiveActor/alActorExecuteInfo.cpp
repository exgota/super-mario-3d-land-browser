namespace ActorExecuteInfoReconstruction
{

struct UpdateList;
struct DrawList;
struct RequestKeeper;

struct ActorExecutionInfo
{
        RequestKeeper* mRequestKeeper;
        int mUpdateListCount;
        UpdateList* mUpdateLists[ 4 ];
        DrawList* mDrawList;
};

extern "C" void fn_001CDCF4( ActorExecutionInfo* executionInfo, DrawList* list )
{
        executionInfo->mDrawList = list;
}

extern "C" ActorExecutionInfo* fn_001CDCFC( ActorExecutionInfo* executionInfo, RequestKeeper* requestKeeper )
{
        executionInfo->mRequestKeeper = requestKeeper;
        executionInfo->mUpdateListCount = 0;
        executionInfo->mDrawList = 0;
        for ( int index = 0; index < 4; index++ )
                executionInfo->mUpdateLists[ index ] = 0;
        return executionInfo;
}

} // namespace ActorExecuteInfoReconstruction
