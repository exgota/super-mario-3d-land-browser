#include <MapObj/FlowerInitNerve.h>
#include <Nerve/alNerveFunction.h>

extern "C" void fn_00351070( const al::Nerve*, al::NerveKeeper* keeper )
{
        al::updateNerveStateAndNextNerve( keeper->getHost(), &dat_003F15D0 );
}

extern "C" void fn_00351080( const al::Nerve*, al::NerveKeeper* keeper )
{
        al::updateNerveStateAndNextNerve( keeper->getHost(), &dat_003F15E4 );
}

extern "C" void fn_00351090( const al::Nerve*, al::NerveKeeper* keeper )
{
        al::updateNerveStateAndNextNerve( keeper->getHost(), &dat_003F15E4 );
}

extern "C" void fn_003510A0( const al::Nerve*, al::NerveKeeper* keeper )
{
        al::updateNerveStateAndNextNerve( keeper->getHost(), &dat_003F15E4 );
}

