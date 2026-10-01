#pragma once

#include <math/seadVector.h>
#include <math/seadQuat.h>

namespace sead
{

template <typename T>
class Matrix34
{
public:
        T m[ 3 ][ 4 ];
        static const Matrix34 ident;
};

typedef Matrix34<f32> Matrix34f;

} // namespace sead
