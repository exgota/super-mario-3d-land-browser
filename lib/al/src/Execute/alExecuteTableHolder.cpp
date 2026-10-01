#include <Execute/alExecuteRequestKeeper.h>
#include <Execute/alExecuteTableHolder.h>
#include <LiveActor/alActorExecuteInfo.h>
#include <LiveActor/alLiveActor.h>
#include <LiveActor/alLiveActorKit.h>

namespace al
{

void registerExecutorUser( IUseExecutor* p, const char* name )
{
        al::getLiveActorKit()->getExecuteDirector()->registerUser( p, name );
}

void registerExecutorFunctor( const char* name, const FunctorBase& base )
{
        al::getLiveActorKit()->getExecuteDirector()->registerFunctor( base, name );
}

void registerExecutorFunctorDraw( const char* name, const FunctorBase& base )
{
        al::getLiveActorKit()->getExecuteDirector()->registerFunctorDraw( base, name );
}

} // namespace al

namespace alActorSystemFunction
{

void addToExecutorMovement( al::LiveActor* actor )
{
        actor->getActorExecuteInfo()->getRequestKeeper()->request( actor, 0 );
}

} // namespace alActorSystemFunction
