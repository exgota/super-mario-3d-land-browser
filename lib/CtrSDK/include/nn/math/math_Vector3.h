#pragma once

#include <nn/types.h>

namespace nn
{
namespace math
{

// Three-float operation storage reconstructed from the original EU helpers.
struct VEC3
{
    f32 x;
    f32 y;
    f32 z;
};

typedef char Vector3StorageSizeCheck[sizeof(VEC3) == 12 ? 1 : -1];
typedef char Vector3ComponentOffsetCheck[
    offsetof(VEC3, x) == 0 && offsetof(VEC3, y) == 4 &&
    offsetof(VEC3, z) == 8 ? 1 : -1];

} // namespace math
} // namespace nn
