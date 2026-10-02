#include <Effect/alEffectSetUpdateMath.h>
#include <stddef.h>

namespace
{
struct EffectTransformationFields;

// Offset-zero native storage bases provide defined reference conversions
// for the genuinely mapped Vector3CalcCtr<float>::mul API. The local
// constructors are the packet's source-shape hypothesis, not recovered names.
struct EffectTranslationFields : public nn::math::VEC3
{
        EffectTranslationFields( const EffectTransformationFields& transformation,
                                 const EffectTranslationFields& translation );
};

struct EffectTransformationFields : public nn::math::MTX34
{
        EffectTransformationFields( const EffectTransformationFields& transformation );
};

struct EffectConfigurationFields
{
        unsigned char mOpaquePrefix[ 0x14 ];
        EffectTranslationFields mTranslationOffset;
        unsigned char mOpaqueMembers20[ 0x1F ];
        unsigned char mFlag3F;
        unsigned char mFlag40;
        unsigned char mOpaqueMembers41[ 4 ];
        unsigned char mFlag45;
};

struct EffectSetFields
{
        unsigned char mOpaquePrefix[ 4 ];
        void** mEntries;
        int mEntryCount;
        EffectConfigurationFields* mConfiguration;
        const EffectTranslationFields* mTranslation;
        const EffectTransformationFields* mTransformation;
};

typedef char TranslationSize[ sizeof( EffectTranslationFields ) == 12 ? 1 : -1 ];
typedef char TransformationSize[ sizeof( EffectTransformationFields ) == 48 ? 1 : -1 ];
typedef char ConfigurationTranslationOffset[ offsetof( EffectConfigurationFields, mTranslationOffset ) == 0x14 ? 1 : -1 ];
typedef char ConfigurationFlag3FOffset[ offsetof( EffectConfigurationFields, mFlag3F ) == 0x3F ? 1 : -1 ];
typedef char ConfigurationFlag40Offset[ offsetof( EffectConfigurationFields, mFlag40 ) == 0x40 ? 1 : -1 ];
typedef char ConfigurationFlag45Offset[ offsetof( EffectConfigurationFields, mFlag45 ) == 0x45 ? 1 : -1 ];
typedef char SetEntriesOffset[ offsetof( EffectSetFields, mEntries ) == 4 ? 1 : -1 ];
typedef char SetCountOffset[ offsetof( EffectSetFields, mEntryCount ) == 8 ? 1 : -1 ];
typedef char SetConfigurationOffset[ offsetof( EffectSetFields, mConfiguration ) == 0x0C ? 1 : -1 ];
typedef char SetTranslationOffset[ offsetof( EffectSetFields, mTranslation ) == 0x10 ? 1 : -1 ];
typedef char SetTransformationOffset[ offsetof( EffectSetFields, mTransformation ) == 0x14 ? 1 : -1 ];
} // namespace

// Independently observed callees at existing unchanged function boundaries.
// The original identities and ignored return types remain unrecovered.
extern "C" bool fn_0023F45C( void* effectEntry );
extern "C" void fn_00291470( EffectTransformationFields& result, const EffectTransformationFields& transformation );
extern "C" void fn_0023F294( void* effectEntry, const EffectConfigurationFields* configuration, const EffectTransformationFields* transformation );
extern "C" void fn_002D37D0( void* effectEntry, const EffectConfigurationFields* configuration, const EffectTranslationFields* translation, const EffectTransformationFields* transformation );

namespace
{
inline EffectTranslationFields::EffectTranslationFields( const EffectTransformationFields& transformation,
                                                        const EffectTranslationFields& translation )
{
        sead::Vector3CalcCtr<float>::mul( *this, transformation, translation );
}

inline EffectTransformationFields::EffectTransformationFields( const EffectTransformationFields& transformation )
{
        fn_00291470( *this, transformation );
}
} // namespace

#ifdef NON_MATCHING
extern "C" bool fn_001EA220( void* effectSet )
{
        EffectSetFields* fields = static_cast<EffectSetFields*>( effectSet );
        bool updated = false;
        for ( int index = 0; index < fields->mEntryCount; ++index )
        {
                void* entry = fields->mEntries[ index ];
                if ( fn_0023F45C( entry ) )
                {
                        EffectConfigurationFields* configuration = fields->mConfiguration;
                        if ( configuration->mFlag40 )
                        {
                                const EffectTransformationFields* transformation = fields->mTransformation;
                                if ( configuration->mFlag45 )
                                {
                                        EffectTranslationFields translation( *transformation, configuration->mTranslationOffset );
                                        EffectTransformationFields localTransformation( *transformation );
                                        localTransformation.m[ 0 ][ 3 ] = translation.x;
                                        localTransformation.m[ 1 ][ 3 ] = translation.y;
                                        localTransformation.m[ 2 ][ 3 ] = translation.z;
                                        fn_0023F294( entry, configuration, &localTransformation );
                                }
                                else
                                        fn_0023F294( entry, configuration, transformation );
                                updated = true;
                        }
                        if ( fields->mConfiguration->mFlag3F )
                        {
                                fn_002D37D0( entry, fields->mConfiguration, fields->mTranslation, fields->mTransformation );
                                updated = true;
                        }
                }
        }
        return updated;
}
#endif
