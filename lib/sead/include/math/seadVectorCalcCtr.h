#pragma once

#include <nn/math/math_VEC3.h>
#include <nn/math/math_MTX34.h>

namespace sead
{

template <typename T>
class Vector3CalcCtr;

// The names and native-reference signatures are present in the project map.
// Each routine writes three floats through its first argument. These are
// declarations only, not replacement implementations or matching claims.
template <>
class Vector3CalcCtr<float>
{
public:
        static void add( nn::math::VEC3& out, const nn::math::VEC3& left,
                         const nn::math::VEC3& right );
        static void sub( nn::math::VEC3& out, const nn::math::VEC3& left,
                         const nn::math::VEC3& right );
        static void multScalar( nn::math::VEC3& out, const nn::math::VEC3& value,
                                float scalar );
        static void mul( nn::math::VEC3& out, const nn::math::MTX34& matrix,
                         const nn::math::VEC3& value );
};

} // namespace sead
