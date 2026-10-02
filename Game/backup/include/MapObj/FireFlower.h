#pragma once
#include <LiveActor/alLiveActor.h>
namespace FlowerInit { class Param; }
class FireFlower : public al::LiveActor {
    unsigned int mUnknown60;
    FlowerInit::Param* mAppearParam;
    void* mItemControl;
public:
    virtual void init(const al::ActorInitInfo& info);
};
static_assert(sizeof(FireFlower) == 0x6c, "Observed FireFlower field extent");
