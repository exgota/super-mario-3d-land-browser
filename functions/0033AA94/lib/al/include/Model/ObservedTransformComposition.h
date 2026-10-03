#pragma once
#include <nn/math/math_MatrixStorage.h>
#include <nn/math/math_Vector3.h>
namespace nn { namespace math { namespace ARMv6 {
void MTX34ToMTX33Asm(MTX33*, const MTX34*);
void VEC3TransformAsm(VEC3*, const MTX33*, const VEC3*);
}}}
namespace observed_transform_composition {
struct Transform {
    nn::math::MTX34 matrix;
    nn::math::VEC3 scale;
    u32 flags;
};
static_assert(sizeof(Transform) == 64, "observed transform record stride");
static_assert(offsetof(Transform, scale) == 0x30, "scale component");
static_assert(offsetof(Transform, flags) == 0x3c, "transform flags");
}
extern "C" void fn_00291470(nn::math::MTX34*, const nn::math::MTX34*);
extern "C" void fn_00216360(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::VEC3*);
extern "C" void fn_00224AD0(nn::math::MTX34*, const nn::math::MTX34*, const nn::math::MTX34*);
