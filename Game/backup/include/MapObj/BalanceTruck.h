#pragma once

#include <MapObj/alMapObjActor.h>
#include <math/seadQuat.h>

// Constructor 0x00135420 installs the BalanceTruck vtable at 0x003C6BDC.
// Unused member names below retain their retail offsets until their roles are known.
class BalanceTruck : public al::MapObjActor
{
        void* mUnknown60;
        void* mUnknown64;
        void* mUnknown68;
        sead::Quatf mInitialRotation;
        sead::Vector3f mUnknown7C;
        float mMovePower;
        float mAnimationPower;
        int mDirection;
        int mPreviousDirection;
        int mUnknown98;
        float mUnknown9C;
        float mUnknownA0;
        float mUnknownA4;
        float mUnknownA8;
        float mUnknownAC;
        int mHeldFrames;

public:
        // Descriptive name for retail 0x0026D6C0.
        void updateMovePower();
};

static_assert( sizeof( BalanceTruck ) == 0xB4, "BalanceTruck retail size" );
