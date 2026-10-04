#include <Factory/Observed001E3060.h>

extern "C" void fn_001E3060( Observed001E3060* object )
{
        const sead::Matrix34f* matrix = object->mMatrix;
        object->mTranslation.x = matrix->m[0][3];
        object->mTranslation.y = matrix->m[1][3];
        object->mTranslation.z = matrix->m[2][3];
}
