#include <Execute/alExecuteRequestKeeper.h>

namespace al
{

// Provisional ordinary C++ types, not recovered original class names.
// The keeper constructor constructs capacity elements with stride four and a
// callback that zeros the first word. A pointer-wrapper entry is one possible
// explanation; scalar pointer value-initialization is not ruled out.
struct ExecuteRequestEntry
{
        LiveActor* mActor;

        ExecuteRequestEntry() : mActor( nullptr ) {}
};

struct ExecuteRequestQueue
{
        int                  mCapacity;
        int                  mSize;
        ExecuteRequestEntry* mEntries;

        ExecuteRequestEntry* at( int index )
        {
                return &mEntries[ index ];
        }

        const ExecuteRequestEntry* at( int index ) const
        {
                return &mEntries[ index ];
        }

        void eraseFast( int index )
        {
                ExecuteRequestEntry* element = at( index );
                if ( index < mSize )
                {
                        int lastIndex = mSize;
                        lastIndex--;
                        *element = *at( lastIndex );
                }
                mSize--;
        }

        bool contains( const LiveActor* actor ) const
        {
                bool found = false;
                for ( int index = 0; index < mSize; index++ )
                {
                        if ( at( index )->mActor == actor )
                        {
                                found = true;
                                break;
                        }
                }
                return found;
        }

        ExecuteRequestEntry* appendSlot()
        {
                mSize++;
                return at( mSize - 1 );
        }
};

typedef char ExecuteRequestEntrySizeCheck[ sizeof( ExecuteRequestEntry ) == 4 ? 1 : -1 ];
typedef char ExecuteRequestQueueSizeCheck[ sizeof( ExecuteRequestQueue ) == 12 ? 1 : -1 ];
typedef char ExecuteRequestKeeperSizeCheck[ sizeof( ExecuteRequestKeeper ) == 16 ? 1 : -1 ];

#ifdef NON_MATCHING
void ExecuteRequestKeeper::request( LiveActor* actor, int requestType )
{
        ExecuteRequestQueue* requestQueue = _0[ requestType ];
        ExecuteRequestQueue* opposingQueue = nullptr;
        if ( requestType == 0 )
                opposingQueue = _0[ 1 ];
        else if ( requestType == 1 )
                opposingQueue = _0[ 0 ];
        else if ( requestType == 2 )
                opposingQueue = _0[ 3 ];
        else if ( requestType == 3 )
                opposingQueue = _0[ 2 ];

        for ( int index = 0; index < opposingQueue->mSize; index++ )
        {
                if ( opposingQueue->at( index )->mActor == actor )
                        opposingQueue->eraseFast( index );
        }

        if ( !requestQueue->contains( actor ) )
                requestQueue->appendSlot()->mActor = actor;
}

#endif

} // namespace al
