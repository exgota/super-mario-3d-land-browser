#pragma once

#include <nn/types.h>

namespace sead
{

template <typename T>
class Vector2
{
public:
        T x, y;
        Vector2() {}
        Vector2( T xValue, T yValue ) : x( xValue ), y( yValue ) {}
        static const Vector2 zero;
};

template <typename T>
class Vector3
{
public:
        T x, y, z;
        Vector3() {}
        Vector3( T xValue, T yValue, T zValue ) : x( xValue ), y( yValue ), z( zValue ) {}
        Vector3& operator*=( T scalar )
        {
                x *= scalar;
                y *= scalar;
                z *= scalar;
                return *this;
        }
        static const Vector3 zero;
        static const Vector3 ones;
        static const Vector3 ex;
        static const Vector3 ey;
        static const Vector3 ez;
};

typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;

} // namespace sead
