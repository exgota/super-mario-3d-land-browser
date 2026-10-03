#pragma once
#include <LiveActor/alLiveActor.h>
class EnemyStateBlowDown;
class EnemyStateHipDropDown;
class Tenten : public al::LiveActor {
    float parameter60;
    bool option7;
    bool flag65;
    unsigned char pad66[2];
    EnemyStateBlowDown* blowDown;
    EnemyStateHipDropDown* hipDropDown;
    bool option5;
public:
    virtual void init(const al::ActorInitInfo& info);
};
static_assert(sizeof(Tenten)==0x74, "Tenten observed constructor extent");
