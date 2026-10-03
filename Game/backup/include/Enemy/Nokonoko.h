#pragma once

#include <MapObj/alMapObjActor.h>

class EnemyStateBlowDown;
class NokonokoKickShell;
class NokonokoJointControl;

class Nokonoko;
extern "C" bool fn_00315C6C(Nokonoko*, u32, al::HitSensor*, al::HitSensor*);

// Registry-family spelling retained from the held initializer header;
// it is a descriptive reconstruction, not a recovered C++ symbol identity.
class Nokonoko : public al::MapObjActor
{
        friend bool fn_00315C6C(Nokonoko*, u32, al::HitSensor*, al::HitSensor*);

        EnemyStateBlowDown* mBlowDownState; // 0x60
        NokonokoKickShell* mKickShell;      // 0x64
        float _68;
        sead::Vector3f mRailDirection;     // 0x6c
        sead::Vector3f mFrontDirection;    // 0x78
        NokonokoJointControl* mHeadControl; // 0x84

public:
        virtual void init( const al::ActorInitInfo& info );
};

static_assert( sizeof( Nokonoko ) == 0x88, "" );
