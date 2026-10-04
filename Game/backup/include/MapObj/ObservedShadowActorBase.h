#pragma once

#include <LiveActor/alLiveActor.h>

// Observed interface of the LiveActor-derived base constructed at 0x001D21FC.
// The registry's ShadowObj inherits its four additional primary virtual slots.
// Additional object members and the original class name remain unknown.
class ObservedShadowActorBase : public al::LiveActor
{
public:
    virtual void v24(void* arg1, void* arg2);
    virtual void v25(void* arg1, void* arg2);
    virtual void v26();
    virtual void v27();
};
