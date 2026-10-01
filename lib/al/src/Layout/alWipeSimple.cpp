#include <Layout/alWipeSimple.h>
#include <Nerve/alNerveFunction.h>

extern "C" bool fn_00333bf0( const al::LayoutActor* actor );
extern "C" float fn_00250f38( const al::LayoutActor* actor );
extern "C" void fn_00250f5c( al::LayoutActor* actor, float rate );
extern "C" const char dat_003aebd4[];
extern "C" void fn_00270ccc( al::LayoutActor* actor, const sead::SafeString& name );

namespace al
{

namespace NrvWipeSimple
{

NERVE_DEF( WipeSimple, Close )
NERVE_DEF( WipeSimple, Wait )
NERVE_DEF( WipeSimple, Open )

} // namespace NrvWipeSimple

WipeSimple::WipeSimple( const char* name, const char* archive, const LayoutInitInfo& info, const char* suffix )
    : LayoutActor( name ), _30( -1 )
{
        initLayoutActor( this, info, archive, suffix );
        initNerve( &NrvWipeSimple::Close );
}

void WipeSimple::appear()
{
        LayoutActor::appear();
}

void WipeSimple::exeClose()
{
        if ( !isFirstStep( this ) && fn_00333bf0( this ) )
                setNerve( this, &NrvWipeSimple::Wait );
}

void WipeSimple::exeWait()
{
        if ( isFirstStep( this ) )
                fn_00270ccc( this, dat_003aebd4 );
}

void WipeSimple::exeOpen()
{
        if ( isFirstStep( this ) )
        {
                if ( _30 <= 0 )
                        fn_00250f5c( this, 1.0f );
                else
                        fn_00250f5c( this, fn_00250f38( this ) / _30 );
        }

        if ( fn_00333bf0( this ) )
                kill();
}

} // namespace al
