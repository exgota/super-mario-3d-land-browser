#pragma once
#include <LiveActor/alLiveActor.h>
namespace FlowerInit { class Param; }
class SuperLeafSpecial : public al::LiveActor {
    unsigned int mUnknown60;
    FlowerInit::Param* mAppearParam;
    void* mItemControl;
public:
    virtual void init(const al::ActorInitInfo& info);
};
static_assert(sizeof(SuperLeafSpecial) == 0x6c, "Observed SuperLeafSpecial field extent");
