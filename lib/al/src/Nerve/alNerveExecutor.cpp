#include <Nerve/alNerveExecutor.h>

namespace al
{

#ifdef NON_MATCHING
NerveExecutor::NerveExecutor( const char* name ) : mNerveKeeper( nullptr )
{
}
#endif

NerveKeeper* NerveExecutor::getNerveKeeper() const
{
        return mNerveKeeper;
}

void NerveExecutor::initNerve( const Nerve* nrv, int step )
{
        mNerveKeeper = new NerveKeeper( this, nrv, step );
}

void NerveExecutor::updateNerve()
{
        if ( mNerveKeeper )
                mNerveKeeper->update();
}

} // namespace al
