#include <Nerve/alNerveActionCtrl.h>
#include <Nerve/alNerveFunction.h>
#include <Nerve/alNerveStateCtrl.h>

namespace al
{

bool isStep( IUseNerve* p, int step )
{
        return p->getNerveKeeper()->getStep() == step;
}

bool isNerve( const IUseNerve* p, const Nerve* nerve )
{
        return p->getNerveKeeper()->getCurrentNerve() == nerve;
}

#pragma no_inline

void setNerve( IUseNerve* p, const Nerve* nerve )
{
        p->getNerveKeeper()->setNerve( nerve );
}

bool isFirstStep( const IUseNerve* p )
{
        return p->getNerveKeeper()->getStep() == 0;
}

bool isGreaterStep( const IUseNerve* p, int t )
{
        return p->getNerveKeeper()->getStep() > t;
}

bool isGreaterEqualStep( const IUseNerve* p, int t )
{
        return p->getNerveKeeper()->getStep() >= t;
}

#pragma inline
bool updateNerveState( IUseNerve* p )
{
        return p->getNerveKeeper()->getStateCtrl()->updateCurrentState();
}

#pragma no_inline

void initNerveState( IUseNerve* p, NerveStateBase* state, const Nerve* stateNrv, const char* name )
{
        state->init();
        p->getNerveKeeper()->getStateCtrl()->registerState( state, stateNrv, name );
}

// registers

bool updateNerveStateAndNextNerve( IUseNerve* p, const Nerve* nerve )
{
        if ( updateNerveState( p ) )
        {
                setNerve( p, nerve );
                return true;
        }
        return false;
}

} // namespace al

namespace alNerveFunction
{

void setNerveAction( al::IUseNerve* p, const char* name )
{
        al::NerveKeeper* nk = p->getNerveKeeper();
        nk->setNerve( nk->getActionCtrl()->findNerve( name ) );
}

} // namespace alNerveFunction
