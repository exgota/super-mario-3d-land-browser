#pragma once

#include <Execute/alExecuteDirector.h>

namespace al
{

class LiveActor;

void registerExecutorUser( IUseExecutor* p, const char* name );
void registerExecutorFunctor( const char* name, const FunctorBase& base );
void registerExecutorFunctorDraw( const char* name, const FunctorBase& base );

} // namespace al

namespace alActorSystemFunction
{

void addToExecutorMovement( al::LiveActor* actor );
void removeFromExecutorDraw( al::LiveActor* actor );

} // namespace alActorSystemFunction
