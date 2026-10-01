namespace
{
// Only the offsets used by these helpers are recovered.
struct EffectConfigurationFields
{
        unsigned char mOpaquePrefix[ 0x0C ];
        unsigned char mDeletionFlag0C;
        unsigned char mDeletionFlag0D;
};

struct EffectEntryFields
{
        unsigned char              mOpaquePrefix[ 8 ];
        EffectConfigurationFields* mConfiguration;
};

struct EffectSetFields
{
        unsigned char       mOpaquePrefix[ 4 ];
        EffectEntryFields** mEntries;
        int                 mEntryCount;
        unsigned char       mOpaqueMembers0C[ 0x0C ];
        bool                mIsActive;
};
} // namespace

extern "C" bool fn_0023EF90( void* effectEntry, bool force );
extern "C" void fn_001C5A88( void* effectEntry );

extern "C" void fn_001EA174( void* effectSet )
{
        EffectSetFields* fields = static_cast<EffectSetFields*>( effectSet );
        for ( int index = 0; index < fields->mEntryCount; ++index )
        {
                EffectEntryFields* entry = fields->mEntries[ index ];
                if ( entry->mConfiguration->mDeletionFlag0C || entry->mConfiguration->mDeletionFlag0D )
                        fn_0023EF90( entry, false );
                fn_001C5A88( entry );
        }
        fields->mIsActive = false;
}

extern "C" void fn_001EA1DC( void* effectSet )
{
        EffectSetFields* fields = static_cast<EffectSetFields*>( effectSet );
        for ( int index = 0; index < fields->mEntryCount; ++index )
                fn_0023EF90( fields->mEntries[ index ], true );
        fields->mIsActive = false;
}
