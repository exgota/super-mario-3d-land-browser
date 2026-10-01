namespace ExecuteRequestKeeperReconstruction
{

struct ExecutorList;
class ModelDrawList
{
public:
        virtual void functionAtOffset00();
        virtual void functionAtOffset04();
        virtual void functionAtOffset08();
        virtual void functionAtOffset0C();
        virtual void functionAtOffset10();
        virtual void functionAtOffset14();
        virtual void removeModel( void* model );
};

struct Model
{
        unsigned char mOpaqueMembers00[ 8 ];
        void* mPointerAtOffset08;
};

struct ModelKeeper
{
        Model* mModel;
};

struct ActorExecutionInfo
{
        void* mRequestKeeper;
        int mUpdateListCount;
        ExecutorList* mUpdateLists[ 4 ];
        ModelDrawList* mDrawList;
};

struct Actor
{
        unsigned char mOpaqueMembers00[ 0x18 ];
        ActorExecutionInfo* mExecutionInfo;
        unsigned char mOpaqueMembers1C[ 0xC ];
        ModelKeeper* mModelKeeper;
};

struct RequestQueue
{
        int mCapacity;
        int mSize;
        Actor** mBuffer;
};

struct RequestKeeper
{
        RequestQueue* mQueues[ 4 ];
};

extern "C" void fn_001E2CE0( ExecutorList* list, Actor* actor );
extern "C" void fn_001E2BEC( ExecutorList* list, Actor* actor );

extern "C" void fn_001DD9E8( RequestKeeper* keeper )
{
        RequestQueue* queue = keeper->mQueues[ 0 ];
        for ( int actorIndex = 0; actorIndex < queue->mSize; actorIndex++ )
        {
                Actor* actor = queue->mBuffer[ actorIndex ];
                ActorExecutionInfo* executionInfo = actor->mExecutionInfo;
                for ( int listIndex = 0; listIndex < executionInfo->mUpdateListCount; listIndex++ )
                        fn_001E2CE0( executionInfo->mUpdateLists[ listIndex ], actor );
        }
        queue->mSize = 0;
}

extern "C" void fn_001DDA58( RequestKeeper* keeper )
{
        RequestQueue* queue = keeper->mQueues[ 1 ];
        for ( int actorIndex = 0; actorIndex < queue->mSize; actorIndex++ )
        {
                Actor* actor = queue->mBuffer[ actorIndex ];
                ActorExecutionInfo* executionInfo = actor->mExecutionInfo;
                for ( int listIndex = 0; listIndex < executionInfo->mUpdateListCount; listIndex++ )
                        fn_001E2BEC( executionInfo->mUpdateLists[ listIndex ], actor );
        }
        queue->mSize = 0;
}

extern "C" void fn_001DD98C( RequestKeeper* keeper )
{
        RequestQueue* queue = keeper->mQueues[ 3 ];
        for ( int actorIndex = 0; actorIndex < queue->mSize; actorIndex++ )
        {
                Actor* actor = queue->mBuffer[ actorIndex ];
                void* model = actor->mModelKeeper->mModel->mPointerAtOffset08;
                actor->mExecutionInfo->mDrawList->removeModel( model );
        }
        queue->mSize = 0;
}

} // namespace ExecuteRequestKeeperReconstruction
