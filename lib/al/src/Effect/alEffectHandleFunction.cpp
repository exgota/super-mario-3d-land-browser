namespace
{
// Only the offsets read by the reference-clear helper are established.
struct EffectObjectIdentifierFields
{
        unsigned char mOpaquePrefix[ 0x0C ];
        unsigned int  mIdentifier;
};

struct EffectHandleReferenceFields
{
        EffectObjectIdentifierFields* mEffectObject;
        unsigned int                  mIdentifier;
};

struct EffectEntryFields
{
        unsigned char                mOpaquePrefix[ 4 ];
        EffectHandleReferenceFields* mReference;
};
} // namespace

extern "C" void fn_001C5A88( void* effectEntry )
{
        EffectEntryFields* entry = static_cast<EffectEntryFields*>( effectEntry );
        EffectHandleReferenceFields* reference = entry->mReference;
        if ( reference->mEffectObject && reference->mIdentifier == reference->mEffectObject->mIdentifier )
                reference->mEffectObject = 0;
}
