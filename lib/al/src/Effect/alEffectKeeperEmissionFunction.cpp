#include <Util/alStringUtil.h>
#include <math/seadVector.h>

namespace
{
struct EffectSetNameFields
{
        const char* mName;
};
struct EffectKeeperEmissionFields
{
        unsigned char mOpaquePrefix[ 4 ];
        int mEffectSetCount;
        EffectSetNameFields** mEffectSets;
        unsigned char mOpaqueMember0C[ 4 ];
        bool mIsUpdateActive;
};
}
extern "C" void fn_001E98DC( void* effectSet, const sead::Vector3f* position );
extern "C" void fn_001BF98C( void* effectKeeper, const char* name, const sead::Vector3f* position )
{
        const sead::Vector3f* effectPosition = position;
        const char* effectName = name;
        EffectKeeperEmissionFields* fields = static_cast<EffectKeeperEmissionFields*>( effectKeeper );
        fields->mIsUpdateActive = true;
        for ( int index = 0; index < fields->mEffectSetCount; ++index )
        {
                if ( al::isEqualString( effectName, fields->mEffectSets[ index ]->mName ) )
                {
                        EffectSetNameFields* effectSet = fields->mEffectSets[ index ];
                        if ( effectSet )
                                fn_001E98DC( effectSet, effectPosition );
                        return;
                }
        }
}
