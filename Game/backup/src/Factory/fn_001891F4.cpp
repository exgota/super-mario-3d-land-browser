#include <System/Application.h>
#include "System/RootTask.h"

extern "C" void* fn_0011A8B0( GameSystem* object, void* argument );

extern "C" void* fn_001891F4( void* argument )
{
        return fn_0011A8B0( fn_0028E678( al::getApplication() )->getGameSystem(), argument );
}
