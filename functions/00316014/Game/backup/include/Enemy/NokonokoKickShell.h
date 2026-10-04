#pragma once

#include <MapObj/alMapObjActor.h>

// Descriptive name for the KickKoura registry family. The independent creator
// and constructor establish this complete 0xc8 MapObjActor-derived object;
// this spelling does not claim recovery of an original C++ class symbol.
class NokonokoKickShell : public al::MapObjActor
{
    sead::Matrix34f mBaseMatrix; // 0x60
    sead::Quatf mRotation;      // 0x90
    sead::Vector3f mDirection;  // 0xa0
    float _ac;
    float _b0;
    u32 _b4;
    u32 _b8;
    u32 _bc;
    u32 _c0;
    u32 _c4;

public:
    NokonokoKickShell(const sead::SafeString& name);
    virtual void init(const al::ActorInitInfo& info);
    virtual void attackSensor(al::HitSensor* me, al::HitSensor* other);
    virtual bool receiveMsg(u32 message, al::HitSensor* other, al::HitSensor* me);
    virtual void control();
};

static_assert(sizeof(NokonokoKickShell) == 0xc8, "");
