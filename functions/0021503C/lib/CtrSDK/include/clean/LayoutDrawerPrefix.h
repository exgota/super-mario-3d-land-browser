#pragma once
#include <stddef.h>
namespace nn { namespace math { struct MTX34; } }
namespace nw { namespace lyt {
struct ObservedColor4f { float component[4]; };
// Observed receiver prefix, not a claim about total Drawer allocation size.
struct Drawer {
    unsigned char unknown00[0x25];
    unsigned char colorCount;
    unsigned char unknown26[0x63e];
    float colorSelector;
    unsigned char unknown668[4];
    ObservedColor4f* colorBuffer;
    void SetUpMtx(const nn::math::MTX34&);
};
static_assert(offsetof(Drawer, colorCount) == 0x25, "byte color counter");
static_assert(offsetof(Drawer, colorSelector) == 0x664, "color selector uniform");
static_assert(offsetof(Drawer, colorBuffer) == 0x66c, "color buffer pointer");
static_assert(sizeof(ObservedColor4f) == 16, "four float channels");
} }
