#pragma once
#include <LiveActor/alLiveActor.h>
namespace observed_grass {
class Receiver : public al::LiveActor {
public:
    int mode;
    int releasedCount;
    int totalCount;
    unsigned char triggered;
    unsigned char hasAccessory;
    unsigned char unknown6E[2];
    al::LiveActor* accessory;
    al::LiveActor* reactionActor;
};
}
