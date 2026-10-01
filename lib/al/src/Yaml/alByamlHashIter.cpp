#include <Yaml/alByamlHashIter.h>

namespace al
{

// The binary-search behavior is reconstructed. Register allocation still differs.
#ifdef NON_MATCHING
const ByamlHashPair* ByamlHashIter::findPair( int keyIdx ) const
{
        const ByamlHashPair* table = getPairTable();
        int low = 0;
        if ( !mData )
                return nullptr;
        int high = getSize();
        if ( high <= 0 )
                return nullptr;
        while ( high > low )
        {
                int index = ( low + high ) / 2;
                int difference = keyIdx - table[ index ].getKeyIndex();
                if ( difference < 0 )
                        high = index;
                else if ( difference > 0 )
                        low = index + 1;
                else
                        return &table[ index ];
        }
        return nullptr;
}

#endif

} // namespace al
