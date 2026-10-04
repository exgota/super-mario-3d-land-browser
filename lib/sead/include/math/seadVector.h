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
        Vector3& operator=( const Vector3& other )
        {
                x = other.x;
                y = other.y;
                z = other.z;
                return *this;
        }
        void set( const Vector3& other )
        {
                x = other.x;
                y = other.y;
                z = other.z;
        }
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

template <>
inline Vector3<float>& Vector3<float>::operator=( const Vector3<float>& other )
{
        reinterpret_cast<nn::math::VEC3&>( *this ) =
            reinterpret_cast<const nn::math::VEC3&>( other );
        return *this;
}

// Float subtraction follows the CTR operation storage in the pinned sead API.
inline Vector3f operator-( const Vector3f& left, const Vector3f& right )
{
        Vector3f result;
        Vector3CalcCtr<float>::sub(
            reinterpret_cast<nn::math::VEC3&>( result ),
            reinterpret_cast<const nn::math::VEC3&>( left ),
            reinterpret_cast<const nn::math::VEC3&>( right ) );
        return result;
}

} // namespace sead
