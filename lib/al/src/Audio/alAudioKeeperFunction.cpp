#include <Audio/alAudioKeeper.h>

namespace
{
struct AudioKeeperFields
{
        bool mIsActive;
        unsigned char mOpaquePrefix[ 3 ];
        void* mResource;
        void* mControl;
        void* mSoundController;
        unsigned char mOpaqueMembers[ 0x24 ];
        void* mMaterialSelection;
};

struct SoundControllerFields
{
        unsigned char mOpaquePrefix[ 0x28 ];
        int mActiveSoundCount;
};
}

extern "C" bool fn_00336028( void* resource );
extern "C" void* fn_00335FF8( void* resource, const void* material );
extern "C" void fn_001D9DC0( void* control );
extern "C" void fn_001DA074( void* soundController );

extern "C" void fn_001BF134( al::AudioKeeper* audioKeeper )
{
        reinterpret_cast<AudioKeeperFields*>( audioKeeper )->mIsActive = true;
}

extern "C" void fn_001BF2E4( al::AudioKeeper* audioKeeper, const void* material )
{
        AudioKeeperFields* fields = reinterpret_cast<AudioKeeperFields*>( audioKeeper );
        if ( fn_00336028( fields->mResource ) )
                fields->mMaterialSelection = fn_00335FF8( fields->mResource, material );
}

extern "C" void fn_00268E80( al::AudioKeeper* audioKeeper )
{
        AudioKeeperFields* fields = reinterpret_cast<AudioKeeperFields*>( audioKeeper );
        SoundControllerFields* soundController = static_cast<SoundControllerFields*>( fields->mSoundController );
        if ( soundController->mActiveSoundCount > 0 )
        {
                fn_001D9DC0( fields->mControl );
                fn_001DA074( fields->mSoundController );
        }
}
