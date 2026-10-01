namespace ExecutorActorListReconstruction
{

struct Actor;

struct ExecutorActorList
{
        const void* mVirtualFunctionTable;
        const char* mName;
        int mCapacity;
        int mSize;
        Actor** mBuffer;
};

extern "C" void fn_001E2BEC( ExecutorActorList* list, Actor* actor )
{
        for ( int index = 0; index < list->mSize; index++ )
        {
                if ( list->mBuffer[ index ] == actor )
                {
                        if ( list->mSize > 1 )
                        {
                                list->mBuffer[ index ] = list->mBuffer[ list->mSize - 1 ];
                                list->mBuffer[ list->mSize - 1 ] = 0;
                        }
                        list->mSize--;
                        return;
                }
        }
}

extern "C" void fn_001E2C60( ExecutorActorList* list )
{
        if ( list->mCapacity > 0 )
        {
                list->mBuffer = new Actor*[ list->mCapacity ];
                for ( int index = 0; index < list->mCapacity; index++ )
                        list->mBuffer[ index ] = 0;
        }
}

extern "C" void fn_001E2CE0( ExecutorActorList* list, Actor* actor )
{
        for ( int index = 0; index < list->mSize; index++ )
                if ( list->mBuffer[ index ] == actor )
                        return;
        list->mBuffer[ list->mSize ] = actor;
        list->mSize++;
}

} // namespace ExecutorActorListReconstruction
