#include "Enemy/BlockDragonGenerator.h"

#include <Functor/alFunctorV0M.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>

struct BlockDragonNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* ) const;
};
extern "C" const BlockDragonNerve dat_003F1E50;
extern "C" const BlockDragonNerve dat_003F1E54;

namespace al
{

template <>
void FunctorV0M<BlockDragonGenerator*, void ( BlockDragonGenerator::* )()>::operator()() const
{
        ( mParent->*mFuncPtr )();
}

template <>
FunctorV0M<BlockDragonGenerator*, void ( BlockDragonGenerator::* )()>*
FunctorV0M<BlockDragonGenerator*, void ( BlockDragonGenerator::* )()>::clone() const
{
        return new FunctorV0M<BlockDragonGenerator*, void ( BlockDragonGenerator::* )()>( *this );
}

} // namespace al

void BlockDragonGenerator::startAppear()
{
        if ( al::isNerve( this, &dat_003F1E50 ) )
                al::setNerve( this, &dat_003F1E54 );
}
