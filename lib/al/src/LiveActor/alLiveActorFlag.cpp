#include <LiveActor/alLiveActor.h>

extern "C" signed char readLiveActorModelHiddenFlag( const al::LiveActor* actor )
{
        return reinterpret_cast<const signed char&>( actor->getLiveActorFlag().isHideModel );
}

extern "C" signed char readLiveActorAuxiliaryFlag( const al::LiveActor* actor )
{
        return reinterpret_cast<const signed char&>( actor->getLiveActorFlag().flag8 );
}

extern "C" signed char readLiveActorClippingFlag( const al::LiveActor* actor )
{
        return reinterpret_cast<const signed char&>( actor->getLiveActorFlag().isClipped );
}
