namespace
{
// This view contains only independently observed retail fields.
struct EffectObjectFlagFields
{
        unsigned char mOpaquePrefix[ 0xCC ];
        unsigned char mFlag1CC;
        unsigned char mOpaqueMember1CD;
        unsigned char mFlag1CE;
        unsigned char mFlag1CF;
};

struct EffectObjectFields
{
        unsigned char mOpaquePrefix[ 4 ];
        int mLiveCount;
        unsigned char mOpaqueMember08[ 4 ];
        unsigned int mIdentifier;
        unsigned char mOpaqueMembers10[ 0xF0 ];
        EffectObjectFlagFields mFlagFields;
};

struct EffectReferenceFields
{
        EffectObjectFields* mEffectObject;
        unsigned int mIdentifier;
};

struct EffectEntryFields
{
        unsigned char mOpaquePrefix[ 4 ];
        EffectReferenceFields* mReference;
};
}

extern "C" void fn_002E3BF8( void* object );
extern "C" void fn_0024DD4C( void* object );

extern "C" bool fn_0023EF90( void* effectEntry, bool force )
{
        EffectEntryFields* entry = static_cast<EffectEntryFields*>( effectEntry );
        EffectReferenceFields* reference = entry->mReference;
        EffectObjectFields* object = reference->mEffectObject;
        if ( !object || reference->mIdentifier != object->mIdentifier || object->mLiveCount <= 0 )
                return false;
        EffectObjectFlagFields* flags = &object->mFlagFields;
        if ( flags->mFlag1CE )
                object->mFlagFields.mFlag1CE = 0;
        if ( force )
        {
                if ( flags->mFlag1CF )
                        fn_002E3BF8( object );
                else
                        fn_0024DD4C( object );
        }
        else if ( !flags->mFlag1CC )
                object->mFlagFields.mFlag1CC = 1;
        return true;
}
