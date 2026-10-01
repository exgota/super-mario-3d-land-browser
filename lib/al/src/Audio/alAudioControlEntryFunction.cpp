namespace
{
class AudioControlInterface
{
public:
        virtual void functionAtOffset00() = 0;
        virtual void functionAtOffset04() = 0;
        virtual void functionAtOffset08() = 0;
        virtual void functionAtOffset0C() = 0;
        virtual void functionAtOffset10() = 0;
        virtual void functionAtOffset14() = 0;
};

struct AudioControlEntryFields
{
        AudioControlInterface* mControl;
};

struct AudioControlFields
{
        int mEntryCount;
        int mEntryCapacity;
        void** mEntries;
};
}

extern "C" void fn_001C21A4( void* entry )
{
        AudioControlEntryFields* fields = static_cast<AudioControlEntryFields*>( entry );
        fields->mControl->functionAtOffset04();
}

extern "C" void fn_001D9D7C( void* control )
{
        AudioControlFields* fields = static_cast<AudioControlFields*>( control );
        for ( int index = 0; index < fields->mEntryCount; ++index )
        {
                AudioControlEntryFields* entry = static_cast<AudioControlEntryFields*>( fields->mEntries[ index ] );
                entry->mControl->functionAtOffset14();
        }
}
