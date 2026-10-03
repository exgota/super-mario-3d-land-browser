#pragma once

#include <MapObj/alMapObjActor.h>
#include <math/seadQuat.h>

class EnemyStateBlowDown;

// Plant is an inferred class name from the archive and item-prefix strings.
// Constructor 0x002F92C4 independently establishes the MapObjActor base.
class Plant : public al::MapObjActor
{
        sead::Quatf mInitialQuat;                  // 0x60
        al::LiveActor* mTrace;                     // 0x70
        EnemyStateBlowDown* mBlowDownState;         // 0x74
        int mItemType;                             // 0x78
        int mStateValue;                           // 0x7C

public:
        virtual void init( const al::ActorInitInfo& info );
};
