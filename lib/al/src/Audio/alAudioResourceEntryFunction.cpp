namespace
{
struct AudioResourceEntryFields
{
        void* mDescriptor;
        int mEntryCount;
        int mEntryCapacity;
        void** mEntries;
};
}

extern "C" void* fn_00335FE0( void* resource, int index )
{
        AudioResourceEntryFields* fields = static_cast<AudioResourceEntryFields*>( resource );
        return fields->mEntries[ index ];
}
