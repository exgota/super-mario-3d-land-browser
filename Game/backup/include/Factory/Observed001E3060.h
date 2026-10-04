#pragma once

#include <math/seadMatrix.h>
#include <math/seadVector.h>

struct Observed001E3060
{
        void* _0;
        const sead::Matrix34f* mMatrix;
        sead::Vector3f mTranslation;
};

extern "C" void fn_001E3060( Observed001E3060* object );
