#include "Enemy/BlockDragonGenerator.h"

#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>

struct BlockDragonNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* ) const;
};
extern "C" const BlockDragonNerve dat_003F1E50;
extern "C" const BlockDragonNerve dat_003F1E54;

void BlockDragonGenerator::startAppear()
{
        if ( al::isNerve( this, &dat_003F1E50 ) )
                al::setNerve( this, &dat_003F1E54 );
}
