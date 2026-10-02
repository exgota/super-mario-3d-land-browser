#include <Execute/alExecuteRequestKeeper.h>
#include <string.h>

namespace al
{

namespace
{

struct RequestSlot
{
LiveActor** mAddress;

explicit RequestSlot( LiveActor** address ) : mAddress( address ) {}

LiveActor* get() const
{
LiveActor* value;
::memcpy( &value, mAddress, sizeof( value ) );
return value;
}

void set( LiveActor* value ) const
{
::memcpy( mAddress, &value, sizeof( value ) );
}
};

} // anonymous namespace

struct ExecuteRequestQueue
{
int mOpaque00;
int mSize;
LiveActor** mBuffer;

void cancel( const LiveActor* actor )
{
int limit = mSize;
if ( limit <= 0 )
return;
int index = 0;
do
{
LiveActor** buffer = mBuffer;
RequestSlot element( buffer + index );
if ( element.get() == actor )
{
int lastIndex = mSize;
if ( lastIndex > index )
{
--lastIndex;
element.set( buffer[ lastIndex ] );
}
limit = --mSize;
}
else
limit = mSize;
++index;
} while ( limit > index );
}

bool contains( const LiveActor* actor ) const
{
bool found = false;
for ( int index = 0; index < mSize; ++index )
{
if ( mBuffer[ index ] == actor )
{
found = true;
break;
}
}
return found;
}

RequestSlot extend()
{
++mSize;
return RequestSlot( &mBuffer[ mSize - 1 ] );
}
};

void ExecuteRequestKeeper::request( LiveActor* actor, int requestType )
{
ExecuteRequestQueue* requestQueue = _0[ requestType ];
ExecuteRequestQueue* opposingQueue = 0;
if ( requestType == 0 )
opposingQueue = _0[ 1 ];
else if ( requestType == 1 )
opposingQueue = _0[ 0 ];
else if ( requestType == 2 )
opposingQueue = _0[ 3 ];
else if ( requestType == 3 )
opposingQueue = _0[ 2 ];

opposingQueue->cancel( actor );
if ( !requestQueue->contains( actor ) )
requestQueue->extend().set( actor );
}

} // namespace al
