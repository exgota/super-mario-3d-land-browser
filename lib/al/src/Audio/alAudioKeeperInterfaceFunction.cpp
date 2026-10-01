#include <Audio/alAudioKeeper.h>
#include <prim/seadSafeString.h>

extern "C" int fn_001BF314( void* audioKeeper, const sead::SafeString& name, int parameter );

extern "C" int fn_00262810( al::IUseAudioKeeper* audioUser, const sead::SafeString& name, int parameter )
{
        if ( audioUser->getAudioKeeper() )
                return fn_001BF314( audioUser->getAudioKeeper(), name, parameter );
        return 0;
}
