#include "MapObj/Garigari.h"

#include <Functor/alFunctorV0M.h>
#include <Nerve/alNerveFunction.h>

extern "C" const al::Nerve dat_003F277C;
extern "C" const al::Nerve dat_003F2780;
extern "C" const al::Nerve dat_003F2784;
extern "C" const al::Nerve dat_003F2788;
extern "C" const al::Nerve dat_003F278C;

extern "C" void fn_0027BEA0( al::IUseEffectKeeper*, const char*, const sead::Vector3f* );
extern "C" void fn_001BFA04( al::IUseEffectKeeper*, const char* );
extern "C" void fn_00267364( al::IUseAudioKeeper*, const sead::SafeString& );
extern "C" void fn_00273B1C( al::IUseAudioKeeper*, const sead::SafeString&, int );

namespace al
{

template <>
void FunctorV0M<Garigari*, void ( Garigari::* )()>::operator()() const
{
        ( mParent->*mFuncPtr )();
}

} // namespace al

extern "C" void fn_00315598( Garigari* actor )
{
        if ( al::isNerve( actor, &dat_003F277C ) )
                al::setNerve( actor, &dat_003F2780 );
}

void Garigari::control()
{
        if ( _6C < _70 && _74 <= _70 &&
                ( al::isNerve( this, &dat_003F2784 ) || al::isNerve( this, &dat_003F2788 ) ||
                        al::isNerve( this, &dat_003F278C ) ) )
        {
                if ( _74 == _6C )
                {
                        fn_0027BEA0( this, "Cut", nullptr );
                        fn_00267364( this, "SeEmLvGarigariCutting" );
                }
                if ( _74 == _70 )
                {
                        fn_001BFA04( this, "Cut" );
                        fn_00273B1C( this, "SeEmLvGarigariCutting", 0 );
                }
                _74++;
        }
}
