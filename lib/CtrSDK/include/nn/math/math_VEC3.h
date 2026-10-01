#pragma once

#include <nn/types.h>
#include <stddef.h>

namespace nn
{
namespace math
{

// Clean storage declaration from the EU vector add/subtract/scale routines
// at 0x0027CB48, 0x0027CB64 and 0x0027CC64. No SDK source was used.
struct VEC3
{
        f32 x;
        f32 y;
        f32 z;
};

typedef char VEC3Size[ sizeof( VEC3 ) == 12 ? 1 : -1 ];
typedef char VEC3XOffset[ offsetof( VEC3, x ) == 0 ? 1 : -1 ];
typedef char VEC3YOffset[ offsetof( VEC3, y ) == 4 ? 1 : -1 ];
typedef char VEC3ZOffset[ offsetof( VEC3, z ) == 8 ? 1 : -1 ];

} // namespace math
} // namespace nn
