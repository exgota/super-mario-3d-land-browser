#include <Effect/alEffectSetAction.h>

namespace
{
// Only the fields used by these dispatch helpers are represented.
struct EffectKeeperFields
{
        unsigned char mOpaquePrefix[ 4 ];
        int mEffectSetCount;
        void** mEffectSets;
        const char* mActionName;
        unsigned char mOpaqueMember10;
        signed char mActionChangeMode;
};
}
extern "C" void fn_001EA00C( void* effectSet, bool value );
extern "C" void fn_001BFAA4( void* effectKeeper )
{
        EffectKeeperFields* fields = static_cast<EffectKeeperFields*>( effectKeeper );
        for ( int index = 0; index < fields->mEffectSetCount; ++index )
                fn_001EA00C( fields->mEffectSets[ index ], false );
}
extern "C" void fn_001BFAE0( void* effectKeeper, int mode )
{
        EffectKeeperFields* fields = static_cast<EffectKeeperFields*>( effectKeeper );
        if ( fields->mActionChangeMode == mode )
                return;
        fields->mActionChangeMode = mode;
        for ( int index = 0; index < fields->mEffectSetCount; ++index )
                fn_001E9B30( fields->mEffectSets[ index ], fields->mActionName, fields->mActionChangeMode );
}
