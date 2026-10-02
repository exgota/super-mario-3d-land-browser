#pragma once

// Private declarations for the effect-set update proposal. The existing map
// supplies the external symbol's spelling. The EU body at 0x0027CB9C supplies
// these storage layouts and argument roles; no original class identity is
// asserted for the local effect field views which derive from these bases.
namespace nn
{
namespace math
{
struct VEC3
{
        float x;
        float y;
        float z;
};

// Keep this definition token-identical to the existing clean definition in
// Math/seadMatrixConversion.cpp. The observed fields are row-major floats.
struct MTX34
{
        float m[ 3 ][ 4 ];
};
} // namespace math
} // namespace nn

namespace sead
{
template <typename T>
class Vector3CalcCtr;

template <>
class Vector3CalcCtr<float>
{
public:
        static void mul( nn::math::VEC3& output, const nn::math::MTX34& matrix,
                         const nn::math::VEC3& input );
};
} // namespace sead
