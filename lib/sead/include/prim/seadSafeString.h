#pragma once

#include <nn/types.h>
#include <stdarg.h>

namespace sead
{

template <typename Character>
class SafeStringBase
{
protected:
        const Character* mStringTop;

public:
        SafeStringBase( const Character* string ) : mStringTop( string ) {}
        virtual ~SafeStringBase() {}
        virtual void assureTermination() const {}

        const Character* cstr() const
        {
                assureTermination();
                return mStringTop;
        }
};

typedef SafeStringBase<char> SafeString;

template <typename Character>
class BufferedSafeStringBase : public SafeStringBase<Character>
{
protected:
        s32 mBufferSize;

public:
        BufferedSafeStringBase( Character* buffer, s32 bufferSize )
            : SafeStringBase<Character>( buffer ), mBufferSize( bufferSize )
        {
                buffer[ bufferSize - 1 ] = 0;
        }
        virtual void assureTermination() const
        {
                const_cast<Character*>( this->mStringTop )[ mBufferSize - 1 ] = 0;
        }
        s32 format( const Character* format, ... );
        s32 formatV( const Character* format, va_list arguments );
};

typedef BufferedSafeStringBase<char> BufferedSafeString;

template <typename Character, s32 BufferSize>
class FixedSafeStringBase : public BufferedSafeStringBase<Character>
{
private:
        Character mBuffer[ BufferSize ];

public:
        FixedSafeStringBase() : BufferedSafeStringBase<Character>( mBuffer, BufferSize )
        {
                mBuffer[ 0 ] = 0;
        }
};

template <s32 BufferSize>
class FixedSafeString : public FixedSafeStringBase<char, BufferSize>
{
public:
        FixedSafeString() : FixedSafeStringBase<char, BufferSize>() {}
};

} // namespace sead
