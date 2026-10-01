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
        void makeST( const Vector3<T>& scale, const Vector3<T>& translation );
};

typedef Matrix34<f32> Matrix34f;

} // namespace sead
