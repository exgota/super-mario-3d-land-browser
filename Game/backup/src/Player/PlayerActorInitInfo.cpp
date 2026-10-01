#include "Player/PlayerActorInitInfo.h"

#include "Player/PlayerInitFunc.h"

extern "C" const PlayerModelInfo* fn_00260600();
extern "C" const PlayerAnimInfo* fn_00260640();
extern "C" void* fn_00260620();

PlayerActorInitInfo::PlayerActorInitInfo()
    : mModelInfo( ::fn_00260600() ), mAnimInfo( ::fn_00260640() ),
      _8( ::fn_00260620() )
{
}
