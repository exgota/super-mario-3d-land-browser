namespace
{
// The flag meanings and complete object identity remain unrecovered.
struct EffectObjectFlagFields
{
        unsigned char mOpaquePrefix[ 0x1CC ];
        unsigned char mFlag1CC;
        unsigned char mOpaqueMember1CD;
        unsigned char mFlag1CE;
};

struct EffectHandleReferenceFields
{
        EffectObjectFlagFields* mEffectObject;
};

struct EffectEntryFields
{
        unsigned char                mOpaquePrefix[ 4 ];
        EffectHandleReferenceFields* mReference;
};
} // namespace

extern "C" void fn_001C5A70( void* effectEntry, unsigned char value )
{
        EffectEntryFields* entry = static_cast<EffectEntryFields*>( effectEntry );
        EffectObjectFlagFields* object = entry->mReference->mEffectObject;
        if ( !object->mFlag1CC )
                object->mFlag1CE = value;
}
