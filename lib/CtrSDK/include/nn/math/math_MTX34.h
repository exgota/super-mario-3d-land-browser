#pragma once

#include <nn/types.h>
#include <stddef.h>

namespace nn
{
namespace math
{

// Clean storage declaration from the named affine-vector routine at
// 0x0027CB9C and the independent matrix copier at 0x00291470. These bodies
// establish three rows of four floats, not the original field spelling.
struct MTX34
{
        f32 mElements[ 3 ][ 4 ];
};

typedef char MTX34Size[ sizeof( MTX34 ) == 48 ? 1 : -1 ];
typedef char MTX34ElementsOffset[ offsetof( MTX34, mElements ) == 0 ? 1 : -1 ];

} // namespace math
} // namespace nn
