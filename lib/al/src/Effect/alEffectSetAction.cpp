#include <Effect/alEffectSetAction.h>
#include <Util/alStringUtil.h>
#include <stddef.h>

namespace
{
struct EffectActionConfiguration
{
        const char* mEffectName;
        const char* mActionName;
        int mUnrecovered08;
        unsigned char mDeletionFlag0C;
        unsigned char mDeletionFlag0D;
};

struct EffectActionObject
{
        unsigned char mOpaquePrefix[ 0x1CC ];
        unsigned char mDeletionRequested;
};

struct EffectActionReference
{
        EffectActionObject* mObject;
        unsigned int mIdentifier;
};

struct EffectActionEntry
{
        void* mUnrecovered00;
        EffectActionReference* mReference;
        EffectActionConfiguration* mConfiguration;
};

struct EffectActionSet
{
        const char* mName;
        EffectActionEntry** mEntries;
        int mEntryCount;
        unsigned char mOpaqueMembers0C[ 0x0C ];
        bool mIsActive;
        unsigned char mAlignment19[ 3 ];
        // The embedded string has pointer/capacity at +0x20/+0x24 and a
        // 64-byte buffer at +0x28. Its complete derived identity is unknown.
        sead::BufferedSafeString mCurrentAction;
        char mCurrentActionBuffer[ 64 ];
};

typedef char ConfigurationActionOffset[ offsetof( EffectActionConfiguration, mActionName ) == 4 ? 1 : -1 ];
typedef char ConfigurationFlagOffset[ offsetof( EffectActionConfiguration, mDeletionFlag0C ) == 0x0C ? 1 : -1 ];
typedef char EntryReferenceOffset[ offsetof( EffectActionEntry, mReference ) == 4 ? 1 : -1 ];
typedef char EntryConfigurationOffset[ offsetof( EffectActionEntry, mConfiguration ) == 8 ? 1 : -1 ];
typedef char SetEntryOffset[ offsetof( EffectActionSet, mEntries ) == 4 ? 1 : -1 ];
typedef char SetCountOffset[ offsetof( EffectActionSet, mEntryCount ) == 8 ? 1 : -1 ];
typedef char SetActiveOffset[ offsetof( EffectActionSet, mIsActive ) == 0x18 ? 1 : -1 ];
typedef char SetActionOffset[ offsetof( EffectActionSet, mCurrentAction ) == 0x1C ? 1 : -1 ];
typedef char SetActionBufferOffset[ offsetof( EffectActionSet, mCurrentActionBuffer ) == 0x28 ? 1 : -1 ];
typedef char ActionObjectFlagOffset[ offsetof( EffectActionObject, mDeletionRequested ) == 0x1CC ? 1 : -1 ];

bool hasAction( const EffectActionSet* set, const char* name )
{
        for ( int index = 0; index < set->mEntryCount; ++index )
        {
                const char* entryName = set->mEntries[ index ]->mConfiguration->mActionName;
                if ( entryName && al::isEqualString( entryName, name ) )
                        return true;
        }
        return false;
}

bool isEqualAction( const char* left, const char* right )
{
        if ( !left && !right )
                return true;
        if ( left && right )
                return al::isEqualString( left, right );
        return false;
}
} // namespace

// The data row contains a pointer to the six-byte "Water" string. Keep the
// independently observed pointer data external rather than replacing it.
extern "C" const char* dat_003F02B4;
extern "C" bool fn_0023F45C( void* effectEntry );
extern "C" bool fn_0023EF90( void* effectEntry, bool force );
extern "C" void fn_0023F02C( void* effectSet, void* effectEntry, bool useOverride );

namespace
{
al::StringTmp<64> selectAction( const EffectActionSet* set, const char* actionName, int actionChangeMode )
{
        if ( actionChangeMode )
        {
                al::StringTmp<64> waterAction( "Water%s", actionName ? actionName : "" );
                if ( hasAction( set, actionName ) )
                        return waterAction;
                for ( int index = 0; index < set->mEntryCount; ++index )
                {
                        const char* entryName = set->mEntries[ index ]->mConfiguration->mActionName;
                        if ( entryName && al::isEqualString( entryName, dat_003F02B4 ) )
                                return al::StringTmp<64>( dat_003F02B4 );
                }
        }
        if ( hasAction( set, actionName ) )
                return al::StringTmp<64>( actionName );
        return al::StringTmp<64>( "" );
}
} // namespace

#ifdef NON_MATCHING
extern "C" void fn_001E9B30( void* effectSet, const char* actionName, int actionChangeMode )
{
        EffectActionSet* set = static_cast<EffectActionSet*>( effectSet );
        al::StringTmp<64> nextAction = selectAction( set, actionName, actionChangeMode );
        if ( isEqualAction( set->mCurrentAction.cstr(), nextAction.cstr() ) )
                return;

        for ( int index = 0; index < set->mEntryCount; ++index )
        {
                EffectActionEntry* entry = set->mEntries[ index ];
                if ( isEqualAction( entry->mConfiguration->mActionName, set->mCurrentAction.cstr() )
                     && fn_0023F45C( entry ) && !entry->mReference->mObject->mDeletionRequested
                     && ( entry->mConfiguration->mDeletionFlag0C || entry->mConfiguration->mDeletionFlag0D ) )
                        fn_0023EF90( entry, false );
        }

        if ( set->mIsActive )
        {
                for ( int index = 0; index < set->mEntryCount; ++index )
                {
                        EffectActionEntry* entry = set->mEntries[ index ];
                        if ( isEqualAction( entry->mConfiguration->mActionName, nextAction.cstr() )
                             && ( entry->mConfiguration->mDeletionFlag0C || entry->mConfiguration->mDeletionFlag0D ) )
                                fn_0023F02C( set, entry, false );
                }
        }
        set->mCurrentAction.format( "%s", nextAction.cstr() );
}

#endif
