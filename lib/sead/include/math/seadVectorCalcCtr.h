#pragma once

#include <nn/math/math_Vector3.h>

namespace sead
{

template <typename T>
class Vector3CalcCtr;

template <>
class Vector3CalcCtr<float>
{
public:
    static float normalize(nn::math::VEC3& vector);
    static void sub(nn::math::VEC3& output,
                    const nn::math::VEC3& left,
                    const nn::math::VEC3& right);
    static void multScalarAdd(nn::math::VEC3& output, float scalar,
                              const nn::math::VEC3& vector,
                              const nn::math::VEC3& addend);
    static void add(nn::math::VEC3& output,
                    const nn::math::VEC3& left,
                    const nn::math::VEC3& right);
    static void multScalar(nn::math::VEC3& output,
                           const nn::math::VEC3& vector,
                           float scalar);
};

} // namespace sead
