namespace
{
struct EffectEntryResourceFields
{
        unsigned char mOpaquePrefix[ 0x84 ];
        unsigned int mDeletionFilter;
};

struct EffectRuntimeEntryFields
{
        unsigned char mOpaquePrefix[ 0x14 ];
        unsigned int mIdentifier;
        unsigned char mOpaqueMembers18[ 0x90 ];
        EffectEntryResourceFields* mResource;
};

// The pointer array at +0x10 ends at the independently used record array at +0x30.
struct EffectObjectHeaderFields
{
        void* mOwner;
        unsigned char mOpaqueMember04[ 4 ];
        int mEntryCount;
        unsigned int mIdentifier;
        EffectRuntimeEntryFields* mEntries[ 8 ];
};
}

extern "C" void fn_002201C4( void* owner, void* entry );

extern "C" void fn_0024DD4C( void* effectObject )
{
        EffectObjectHeaderFields* fields = static_cast<EffectObjectHeaderFields*>( effectObject );
        for ( int index = 0; index < fields->mEntryCount; ++index )
        {
                EffectRuntimeEntryFields* entry = fields->mEntries[ index ];
                if ( entry->mIdentifier == fields->mIdentifier )
                        fn_002201C4( fields->mOwner, entry );
        }
}

extern "C" void fn_002E3BF8( void* effectObject )
{
        EffectObjectHeaderFields* fields = static_cast<EffectObjectHeaderFields*>( effectObject );
        for ( int index = 0; index < fields->mEntryCount; ++index )
        {
                EffectRuntimeEntryFields* entry = fields->mEntries[ index ];
                if ( entry->mIdentifier == fields->mIdentifier && entry->mResource->mDeletionFilter == 0x7FFFFFFFU )
                        fn_002201C4( fields->mOwner, entry );
        }
}
