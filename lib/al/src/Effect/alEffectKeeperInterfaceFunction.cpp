#include <Effect/alEffectKeeper.h>

extern "C" void fn_001BF98C( void* effectKeeper, const char* name, const sead::Vector3f* position );
extern "C" void fn_001BFB30( void* effectKeeper, const char* name );

extern "C" void fn_0027BEA0( al::IUseEffectKeeper* effectUser, const char* name, const sead::Vector3f* position )
{
        fn_001BF98C( effectUser->getEffectKeeper(), name, position );
}

extern "C" void fn_002796C0( al::IUseEffectKeeper* effectUser, const char* name )
{
        fn_001BFB30( effectUser->getEffectKeeper(), name );
}

extern "C" void fn_001BFA24( void* effectKeeper, const char* name );

extern "C" void fn_001BFA04( al::IUseEffectKeeper* effectUser, const char* name )
{
        fn_001BFA24( effectUser->getEffectKeeper(), name );
}

extern "C" void fn_001BFC48( al::IUseEffectKeeper* effectUser )
{
        effectUser->getEffectKeeper()->deleteEffectAll();
}
