#pragma once

#include <MapObj/alMapObjActor.h>
#include <math/seadVector.h>

class EnemyStateBlowDown;
class PackunFlowerTransform;
class PackunFlowerTrace;

// Init-only view; constructor 0x0013D8CC establishes these offsets and the
// MapObjActor base. The complete virtual implementation remains unrecovered.
class PackunFlower : public al::MapObjActor
{
        PackunFlowerTransform* mTransform; // 0x60
        PackunFlowerTrace* mTrace;         // 0x64
        sead::Vector3f mHomeTrans;         // 0x68
        float mUnknown74;
        bool mIsTrace;                    // 0x78
        EnemyStateBlowDown* mBlowDown;     // 0x7C
        float mDistance;                  // 0x80
        bool mIsDokan;                    // 0x84

public:
        virtual void init( const al::ActorInitInfo& info );
};

static_assert( sizeof( PackunFlower ) == 0x88, "PackunFlower retail size" );
