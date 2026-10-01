namespace
{
struct AudioResourceDescriptorFields
{
        unsigned char mOpaquePrefix[ 0x24 ];
        void* mMaterialSelectionTable;
};

struct AudioResourceFields
{
        AudioResourceDescriptorFields* mDescriptor;
};
}

extern "C" void* fn_00335FEC( void* resource )
{
        AudioResourceFields* fields = static_cast<AudioResourceFields*>( resource );
        return reinterpret_cast<unsigned char*>( fields->mDescriptor ) + 8;
}

extern "C" bool fn_00336028( void* resource )
{
        AudioResourceFields* fields = static_cast<AudioResourceFields*>( resource );
        return fields->mDescriptor->mMaterialSelectionTable != 0;
}
