namespace
{
struct AudioControlFields
{
        int mEntryCount;
        int mEntryCapacity;
        void** mEntries;
};
}

extern "C" void fn_001C21A4( void* entry );

extern "C" void fn_001D9DC0( void* control )
{
        AudioControlFields* fields = static_cast<AudioControlFields*>( control );
        for ( int index = 0; index < fields->mEntryCount; ++index )
                fn_001C21A4( fields->mEntries[ index ] );
}
