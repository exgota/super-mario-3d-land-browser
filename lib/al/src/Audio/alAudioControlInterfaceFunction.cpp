#include <Audio/alAudioKeeper.h>

extern "C" void fn_001D9D74( void* audioKeeper );

extern "C" void fn_00279D3C( al::IUseAudioKeeper* audioUser )
{
        fn_001D9D74( audioUser->getAudioKeeper() );
}
