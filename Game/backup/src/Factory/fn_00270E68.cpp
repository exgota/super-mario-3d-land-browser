#include <LiveActor/alLiveActor.h>

extern "C" uint32_t fn_001C1F50( al::LiveActor* actor );

extern "C" uint32_t fn_00270E68( al::LiveActor* actor )
{
        *( reinterpret_cast<bool*>( reinterpret_cast<char*>( actor ) + 0x5B ) ) = true;
        return fn_001C1F50( actor );
}
