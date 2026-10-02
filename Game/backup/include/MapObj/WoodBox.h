#pragma once
#include <MapObj/alMapObjActor.h>

// Constructor 314778 and initializer 314660 establish this actor prefix.
class WoodBox : public al::MapObjActor {
    unsigned char mUnused5D[3];
    al::LiveActor* mBreakActor;
    al::LiveActor* mTraceActor;
    void* mItemKeeper;
    bool mWater;
    bool mCreateTrace;
    unsigned char mUnused6E[2];
    sead::Vector3f mInitialTranslation;

    void finishBreak(unsigned msg);
public:
    virtual bool receiveMsg(u32 msg,al::HitSensor* other,al::HitSensor* me);
};
static_assert(sizeof(WoodBox)==0x7C, "WoodBox observed constructor extent");
