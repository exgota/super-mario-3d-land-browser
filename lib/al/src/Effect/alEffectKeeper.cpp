#include <Effect/alEffectKeeper.h>

namespace al
{

extern "C" void fn_001EA174( void* effectSet );
extern "C" bool fn_001EA220( void* effectSet );

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

} // namespace al
