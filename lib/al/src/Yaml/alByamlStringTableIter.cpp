#include <Yaml/alByamlStringTableIter.h>
#include <cstring>

namespace al
{

int ByamlStringTableIter::findStringIndex( const char* str ) const
{
        int        low = 0, index = 0;
        int        high  = mHeader->stringAmount;
        const u32* table = getAddressTable();

        if ( high <= 0 )
                return -1;

        while ( high > low )
        {
                index   = ( low + high ) / 2;
                int res = std::strcmp( str, (const char*)&mData[ table[ index ] ] );
                if ( res < 0 )
                        high = index;
                else if ( res > 0 )
                        low = index + 1;
                if ( res == 0 )
                        return index;
        }

        return -1;
}

#ifdef NON_MATCHING
const char* ByamlStringTableIter::getString( int index ) const
{
        return reinterpret_cast<const char*>( getAddressTable()[ index ] + mDataPtr );
}
#endif

} // namespace al
