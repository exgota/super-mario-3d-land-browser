#include <Nerve/alNerveFunction.h>

namespace al
{

bool isLessStep( const IUseNerve* p, int step )
{
        return p->getNerveKeeper()->getStep() < step;
}

int getNerveStep( const IUseNerve* p )
{
        return p->getNerveKeeper()->getStep();
}

} // namespace al
