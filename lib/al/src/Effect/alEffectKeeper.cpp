#include <Effect/alEffectKeeper.h>
#include <Util/alStringUtil.h>

namespace al
{

extern "C" void fn_001EA174( void* effectSet );
extern "C" bool fn_001EA220( void* effectSet );
extern "C" void fn_001EA1DC( void* effectSet );
extern "C" void fn_001E9B30( void* effectSet, const char* actionName, signed char actionChangeMode );

void EffectKeeper::deleteAndClearEffectAll()
{
        for ( int index = 0; index < mEffectSetCount; ++index )
                fn_001EA174( mEffectSets[ index ] );
}

void EffectKeeper::update()
{
        if ( mIsUpdateActive )
        {
                bool isUpdateActive = false;
                for ( int index = 0; index < mEffectSetCount; ++index )
                {
                        if ( fn_001EA220( mEffectSets[ index ] ) )
                                isUpdateActive = true;
                }
                mIsUpdateActive = isUpdateActive;
        }
}

void EffectKeeper::deleteEffectAll()
{
        for ( int index = 0; index < mEffectSetCount; ++index )
                fn_001EA1DC( mEffectSets[ index ] );
}

void EffectKeeper::setActionName( const char* actionName )
{
        if ( !isEqualString( mActionName, actionName ) )
        {
                mActionName = actionName;
                for ( int index = 0; index < mEffectSetCount; ++index )
                        fn_001E9B30( mEffectSets[ index ], mActionName, mActionChangeMode );
        }
}

} // namespace al
