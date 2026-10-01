#pragma once

#include <nn/types.h>
#include <stdarg.h>

// Existing retail memory-copy entry used by the independently observed
// fixed-string deep-copy closure. Its return value is not needed here.
extern "C" void* nnnstdMemCpy( void* destination, const void* source, unsigned int size );

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

        // The retail fixed-string copy at 0x001090CC bounds both scans at
        // 0x10000 characters and reports zero when no terminator is found.
        s32 calcLength() const
        {
                assureTermination();
                s32 length = 0;
                while ( length < 0x10000 && this->mStringTop[ length ] != 0 )
                        ++length;
                if ( length >= 0x10000 )
                        return 0;
                return length;
        }

        const Character* cstr() const
        {
                assureTermination();
                return this->mStringTop;
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
                const_cast<Character*>( this->mStringTop )[ mBufferSize - 1 ] = 0;
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
                const_cast<Character*>( this->mStringTop )[ 0 ] = 0;
        }

        // A fixed string must rebind its pointer to its own buffer when copied.
        // The same closure occurs independently at 0x001090CC (capacity 128)
        // and in the action-name return at 0x001E9BAC (capacity 64).
        FixedSafeStringBase( const FixedSafeStringBase& other )
            : BufferedSafeStringBase<Character>( mBuffer, BufferSize )
        {
                if ( this != &other )
                {
                        const_cast<Character*>( this->mStringTop )[ 0 ] = 0;
                        Character* buffer = const_cast<Character*>( this->mStringTop );
                        s32 position = 0;
                        s32 previousLength = this->calcLength();
                        s32 length = other.calcLength();
                        if ( position + length >= this->mBufferSize )
                                length = this->mBufferSize - position - 1;
                        if ( length > 0 )
                        {
                                nnnstdMemCpy( buffer + position, other.cstr(), length * sizeof(Character) );
                                if ( position + length > previousLength )
                                        buffer[ position + length ] = 0;
                        }
                }
        }
};

// The generic fixed-buffer base and char wrapper are distinct retail layers.
template <s32 BufferSize>
class FixedSafeString : public FixedSafeStringBase<char, BufferSize>
{
public:
        FixedSafeString() : FixedSafeStringBase<char, BufferSize>() {}
};

} // namespace sead
