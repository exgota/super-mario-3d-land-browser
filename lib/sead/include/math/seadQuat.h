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
        // Public API shape corroborated by open-ead/sead; retail copies all
        // four components at 0x002F90B0 through 0x002F90C0.
        void set( const Quat& other ) { *this = other; }
        static const Quat unit;
};

typedef Quat<f32> Quatf;

} // namespace sead
