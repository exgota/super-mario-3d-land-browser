#pragma once

#include <LiveActor/alLiveActor.h>
#include <math/seadVector.h>

// Observed prefix of an actor that arranges child actors vertically.
// The original class identity and the remaining fields are unresolved.
class Observed00322040 : public al::LiveActor
{
public:
    al::LiveActor** mChildren;             // 0x60
    unsigned char mUnresolved64[0x10];
    int mChildCount;                       // 0x74
    unsigned char mUnresolved78[0x40];
    sead::Vector3f mSavedTranslation;       // 0xb8
};

extern "C" bool fn_002173E0(const al::LiveActor* actor);
extern "C" const sead::Vector3f& fn_0027D530(const al::LiveActor* actor);
extern "C" void fn_00322040(Observed00322040* actor);
