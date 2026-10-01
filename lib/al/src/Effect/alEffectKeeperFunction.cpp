#include <Util/alStringUtil.h>

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
extern "C" void fn_001E9B30( void* effectSet, const char* actionName, int mode );
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

namespace
{
struct EffectSetNameFields
{
        const char* mName;
};
}

extern "C" bool fn_001E9ADC( void* effectSet );

extern "C" void fn_001BFA24( void* effectKeeper, const char* name )
{
        EffectKeeperFields* fields = static_cast<EffectKeeperFields*>( effectKeeper );
        for ( int index = 0; index < fields->mEffectSetCount; ++index )
        {
                EffectSetNameFields* effectSet = static_cast<EffectSetNameFields*>( fields->mEffectSets[ index ] );
                if ( al::isEqualString( name, effectSet->mName ) )
                {
                        void* effectSet = fields->mEffectSets[ index ];
                        if ( effectSet )
                                fn_001E9ADC( effectSet );
                        return;
                }
        }
}

extern "C" void fn_001BFB30( void* effectKeeper, const char* name )
{
        EffectKeeperFields* fields = static_cast<EffectKeeperFields*>( effectKeeper );
        for ( int index = 0; index < fields->mEffectSetCount; ++index )
        {
                EffectSetNameFields* effectSet = static_cast<EffectSetNameFields*>( fields->mEffectSets[ index ] );
                if ( al::isEqualString( name, effectSet->mName ) )
                {
                        void* effectSet = fields->mEffectSets[ index ];
                        if ( effectSet )
                                fn_001E9ADC( effectSet );
                        return;
                }
        }
}

extern "C" void* fn_002494A0( void* effectKeeper, const char* name )
{
        EffectKeeperFields* fields = static_cast<EffectKeeperFields*>( effectKeeper );
        for ( int index = 0; index < fields->mEffectSetCount; ++index )
        {
                EffectSetNameFields* effectSet = static_cast<EffectSetNameFields*>( fields->mEffectSets[ index ] );
                if ( al::isEqualString( name, effectSet->mName ) )
                        return fields->mEffectSets[ index ];
        }
        return 0;
}
