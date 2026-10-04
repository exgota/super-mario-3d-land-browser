#pragma once

#include <LiveActor/alLiveActor.h>

// Partial actor view used by the sensor forwarding method at 0x001BC800.
// The original class name and the intervening members remain unknown.
struct ObservedActor001BC800
{
    al::LiveActor actor;
    al::LiveActor* actor60;
    unsigned char unknown64[0x20];
    bool flag84;
};
