#pragma once
#include <LiveActor/alLiveActor.h>
namespace FlowerInit { class Param; }
class BoomerangFlower : public al::LiveActor {
    unsigned int mUnknown60;
    FlowerInit::Param* mAppearParam;
    void* mItemControl;
public:
    virtual void init(const al::ActorInitInfo& info);
};
static_assert(sizeof(BoomerangFlower) == 0x6c, "Observed BoomerangFlower field extent");
