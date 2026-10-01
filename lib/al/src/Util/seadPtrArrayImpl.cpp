extern "C" void nnnstdMemMove( void* destination, const void* source, unsigned int size );

namespace sead
{

class PtrArrayImpl
{
public:
        void setBuffer( int capacity, void* buffer );
        void erase( int index, int count );

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

} // namespace sead
