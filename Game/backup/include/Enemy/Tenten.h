#pragma once
#include <MapObj/alMapObjActor.h>
class EnemyStateBlowDown;
class EnemyStateHipDropDown;
// Init-only view. Constructor 0x0030A37C calls MapObjActor at 0x00280428.
// The observed 0x74-byte prefix does not prove the complete allocation extent.
class Tenten : public al::MapObjActor {
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
