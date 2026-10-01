#pragma once

#include <math/seadVector.h>

namespace sead
{

template <typename T>
class Quat
{
public:
        T x, y, z, w;
        Quat() {}
        Quat( T xValue, T yValue, T zValue, T wValue )
            : x( xValue ), y( yValue ), z( zValue ), w( wValue ) {}
        static const Quat unit;
};

typedef Quat<f32> Quatf;

} // namespace sead
