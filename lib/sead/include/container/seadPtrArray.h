#pragma once

#include <nn/types.h>

namespace sead
{

template <typename T>
class PtrArray
{
private:
        s32 mCapacity;
        s32 mSize;
        T** mBuffer;

public:
        PtrArray() : mCapacity( 0 ), mSize( 0 ), mBuffer( 0 ) {}
        s32 size() const { return mSize; }
        T* at( s32 index ) const
        {
                if ( static_cast<u32>( index ) < static_cast<u32>( mCapacity ) )
                        return mBuffer[ index ];
                return 0;
        }
        T* unsafeAt( s32 index ) const { return mBuffer[ index ]; }
        T* operator[]( s32 index ) const { return mBuffer[ index ]; }
        void pushBack( T* value )
        {
                mBuffer[ mSize ] = value;
                ++mSize;
        }
        void allocBufferInline( s32 capacity )
        {
                mCapacity = capacity;
                mSize = 0;
                mBuffer = new T*[ capacity ];
        }
};

} // namespace sead
