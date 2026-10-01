#include <LiveActor/alLiveActor.h>
#include <LiveActor/alLiveActorFlag.h>

namespace al
{

LiveActorFlag::LiveActorFlag()
    : isDead( true ), isClipped( false ), isInvalidClipping( true ), isDrawClipping( false ), flag5( false ),
      isHideModel( false ), isOffCollide( true ), flag8( false ), isValidMaterialCode( false )
{
}

} // namespace al

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
