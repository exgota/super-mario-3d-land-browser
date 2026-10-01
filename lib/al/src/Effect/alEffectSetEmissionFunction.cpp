#ifdef NON_MATCHING
#include <Util/alStringUtil.h>
#include <math/seadVector.h>

namespace
{
struct EffectEntryEmissionConfigurationFields
{
        unsigned char mOpaquePrefix[ 4 ];
        const char* mFilterName;
        unsigned char mOpaqueMember08[ 4 ];
        unsigned char mActivityFlag0C;
        unsigned char mActivityFlag0D;
};
struct EffectEntryEmissionFields
{
        unsigned char mOpaquePrefix[ 8 ];
        EffectEntryEmissionConfigurationFields* mConfiguration;
};
struct EffectSetEmissionFields
{
        unsigned char mOpaquePrefix[ 4 ];
        EffectEntryEmissionFields** mEntries;
        int mEntryCount;
        unsigned char mOpaqueMembers0C[ 0x0C ];
        bool mIsActive;
        unsigned char mOpaqueMembers19[ 3 ];
        sead::SafeString mFilterName;
};
}
extern "C" bool fn_0023F23C( void* effectEntry );
extern "C" void fn_0023F02C( void* effectSet, void* effectEntry, const sead::Vector3f* position );

extern "C" void fn_001E98DC( void* effectSet, const sead::Vector3f* position )
{
        const sead::Vector3f* emissionPosition = position;
        EffectSetEmissionFields* fields = static_cast<EffectSetEmissionFields*>( effectSet );
        for ( int index = -1; fields->mEntryCount > ++index; )
        {
                EffectEntryEmissionFields* entry = fields->mEntries[ index ];
                const sead::SafeString& setFilter = fields->mFilterName;
                const char* filterName = entry->mConfiguration->mFilterName;
                const char* setFilterName = setFilter.cstr();
                if ( ( !filterName && !setFilterName ) ||
                     ( filterName && setFilterName && al::isEqualString( filterName, setFilterName ) ) )
                {
                        if ( fn_0023F23C( entry ) )
                                fn_0023F02C( effectSet, entry, emissionPosition );
                }
                if ( entry->mConfiguration->mActivityFlag0C || entry->mConfiguration->mActivityFlag0D )
                        fields->mIsActive = true;
        }
}

extern "C" bool fn_001E9A10( void* effectSet, const sead::Vector3f* position )
{
        const sead::Vector3f* emissionPosition = position;
        EffectSetEmissionFields* fields = static_cast<EffectSetEmissionFields*>( effectSet );
        bool emitted = false;
        for ( int index = -1; fields->mEntryCount > ++index; )
        {
                EffectEntryEmissionFields* entry = fields->mEntries[ index ];
                const sead::SafeString& setFilter = fields->mFilterName;
                const char* filterName = entry->mConfiguration->mFilterName;
                const char* setFilterName = setFilter.cstr();
                if ( ( !filterName && !setFilterName ) ||
                     ( filterName && setFilterName && al::isEqualString( filterName, setFilterName ) ) )
                {
                        if ( fn_0023F23C( entry ) )
                        {
                                fn_0023F02C( effectSet, entry, emissionPosition );
                                emitted = true;
                        }
                }
                if ( entry->mConfiguration->mActivityFlag0C || entry->mConfiguration->mActivityFlag0D )
                        fields->mIsActive = true;
        }
        return emitted;
}

#endif
