#include "Player/PlayerTrigger.h"

extern "C" bool fn_0026EA54( void* self )
{
        return reinterpret_cast<PlayerTrigger*>( *reinterpret_cast<PlayerTrigger**>( reinterpret_cast<char*>( self ) + 4 ) )->isOn( static_cast<PlayerTrigger::ESensorTrigger>( 1 ) );
}
