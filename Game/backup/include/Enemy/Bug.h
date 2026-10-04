#pragma once

#include <MapObj/alMapObjActor.h>

class EnemyStateBlowDown;
class EnemyStateHipDropDown;
struct BugActionParameters;

class Bug : public al::MapObjActor
{
private:
        sead::Vector3f _60; // 0x60; initialized to zero by the constructor
        int _6C;              // 0x6c; initialized to 300 by the constructor
        sead::Vector3f mStartTrans;    // 0x70
        BugActionParameters* mActionParameters;  // 0x7c
        EnemyStateBlowDown* mBlowDownState;       // 0x80
        EnemyStateHipDropDown* mHipDropDownState; // 0x84

public:
        virtual void init( const al::ActorInitInfo& info );
};

static_assert( sizeof( Bug ) == 0x88, "" );
