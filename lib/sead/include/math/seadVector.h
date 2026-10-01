#pragma once

#include <nn/types.h>
#include <math/seadVectorCalcCtr.h>

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

// Retail vector operations accept nn::math::VEC3 storage. Public inheritance
// is this reconstruction's type-safe storage bridge, not a claim about the
// original source spelling. The binary establishes x/y/z at +0/+4/+8.
template <>
class Vector3<float> : public nn::math::VEC3
{
public:
        Vector3() {}
        Vector3( float xValue, float yValue, float zValue )
        {
                x = xValue;
                y = yValue;
                z = zValue;
        }
        Vector3& operator*=( float scalar )
        {
                x *= scalar;
                y *= scalar;
                z *= scalar;
                return *this;
        }
        Vector3 operator+( const Vector3& other ) const
        {
                Vector3 result;
                Vector3CalcCtr<float>::add( result, *this, other );
                return result;
        }
        Vector3 operator-( const Vector3& other ) const
        {
                Vector3 result;
                Vector3CalcCtr<float>::sub( result, *this, other );
                return result;
        }
        Vector3 operator*( float scalar ) const
        {
                Vector3 result;
                Vector3CalcCtr<float>::multScalar( result, *this, scalar );
                return result;
        }
        static const Vector3 zero;
        static const Vector3 ones;
        static const Vector3 ex;
        static const Vector3 ey;
        static const Vector3 ez;
};

typedef char Vector3FloatSize[ sizeof( Vector3<float> ) == 12 ? 1 : -1 ];

typedef Vector2<f32> Vector2f;
typedef Vector3<f32> Vector3f;

} // namespace sead
