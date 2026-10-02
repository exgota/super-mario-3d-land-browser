extern "C" void nnnstdMemMove( void* destination, const void* source, unsigned int size );

namespace sead
{

class PtrArrayImpl
{
public:
        void setBuffer( int capacity, void* buffer );
        void erase( int index, int count );
        void heapSort( int ( *compare )( const void*, const void* ) );
        void sort( int ( *compare )( const void*, const void* ) );

private:
        int mSize;
        int mCapacity;
        void** mBuffer;
};

void PtrArrayImpl::setBuffer( int capacity, void* buffer )
{
        if ( capacity > 0 && buffer )
        {
                mSize = 0;
                mCapacity = capacity;
                mBuffer = static_cast<void**>( buffer );
        }
}

void PtrArrayImpl::erase( int index, int count )
{
        if ( index < 0 || count < 0 )
                return;
        int end = index + count;
        if ( mSize < end )
                return;
        if ( mSize > end )
        {
                void** destination = mBuffer + index;
                void** source = mBuffer + end;
                unsigned int size = ( mSize - end ) * sizeof( void* );
                nnnstdMemMove( destination, source, size );
        }
        mSize -= count;
}

void PtrArrayImpl::heapSort( int ( *compare )( const void*, const void* ) )
{
        void** buffer = mBuffer;
        int count = mSize;
        for ( int parent = count / 2; parent >= 1; --parent )
        {
                int index = parent;
                void* value = buffer[ index - 1 ];
                for ( int child = index * 2; child <= count; child = index * 2 )
                {
                        if ( child < count && compare( buffer[ child - 1 ], buffer[ child ] ) < 0 )
                                ++child;
                        if ( compare( value, buffer[ child - 1 ] ) >= 0 )
                                break;
                        buffer[ index - 1 ] = buffer[ child - 1 ];
                        index = child;
                }
                buffer[ index - 1 ] = value;
        }
        while ( count > 1 )
        {
                void* value = buffer[ count - 1 ];
                buffer[ count - 1 ] = buffer[ 0 ];
                --count;
                int index = 1;
                for ( int child = 2; child <= count; child = index * 2 )
                {
                        if ( child < count && compare( buffer[ child - 1 ], buffer[ child ] ) < 0 )
                                ++child;
                        if ( compare( value, buffer[ child - 1 ] ) >= 0 )
                                break;
                        buffer[ index - 1 ] = buffer[ child - 1 ];
                        index = child;
                }
                buffer[ index - 1 ] = value;
        }
}

void PtrArrayImpl::sort( int ( *compare )( const void*, const void* ) )
{
        int count = mSize;
        void** buffer = mBuffer;
        if ( count > 1 )
        {
                int lower = 0;
                int upper = count - 1;
                do
                {
                        int last = lower;
                        for ( int index = lower; index < upper; ++index )
                        {
                                if ( compare( buffer[ index ], buffer[ index + 1 ] ) > 0 )
                                {
                                        void* value = buffer[ index + 1 ];
                                        buffer[ index + 1 ] = buffer[ index ];
                                        buffer[ index ] = value;
                                        last = index;
                                }
                        }
                        upper = last;
                        if ( lower == upper )
                                break;
                        for ( int index = upper; index > lower; --index )
                        {
                                if ( compare( buffer[ index ], buffer[ index - 1 ] ) < 0 )
                                {
                                        void* value = buffer[ index - 1 ];
                                        buffer[ index - 1 ] = buffer[ index ];
                                        buffer[ index ] = value;
                                        last = index;
                                }
                        }
                        lower = last;
                } while ( lower != upper );
        }
}

} // namespace sead
