#pragma once

#include <math/seadMatrix.h>

namespace al
{

// Observed records, not a recovered original class name.  The constructor at
// 001DF860 and insertion at 001DF8FC independently establish every field used.
struct JointAimEntry1DF860
{
        int jointIndex;
        sead::Vector2f horizontalLimits;
        sead::Vector2f verticalLimits;
        sead::Vector3f jointUp;
        sead::Vector3f jointSide;
        sead::Vector3f jointFront;
        sead::Quatf currentRotation;
        sead::Quatf previousTarget;
        bool wasAiming;
        float interpolation;
        const sead::Matrix34f* referenceMatrix;
        sead::Vector3f referenceFront;
        sead::Vector3f referenceUp;
        sead::Vector3f referenceSide;
};

// 001E04DC initializes 0x24 bytes; independent caller 0026CA54 allocates 0x24.
struct JointAimController1E04DC
{
        sead::Vector3f targetPosition;
        const sead::Matrix34f* actorMatrix;
        JointAimEntry1DF860* entries;
        int capacity;
        int first;
        int count;
        bool enabled;
        bool retainPreviousTarget;
};

} // namespace al

extern "C" bool fn_001DFB44( al::JointAimController1E04DC*, sead::Matrix34f*, int );
