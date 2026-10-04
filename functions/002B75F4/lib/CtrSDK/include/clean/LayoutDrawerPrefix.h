#pragma once
#include <stddef.h>
namespace nn { namespace math { struct MTX34; } }
namespace nw { namespace lyt {
struct UniformVector4f { float component[4]; };
// Compatibility with the stopped 21503C proposal: these are general float4
// uniform registers, also used for the three rows of a text transform.
typedef UniformVector4f ObservedColor4f;
class TextBox;
class Material;
class DrawInfo;
struct Drawer {
    unsigned char unknown00[0x25];
    unsigned char colorCount;
    unsigned char unknown26[0x63e];
    float colorSelector;
    unsigned char unknown668[4];
    // Retained field spelling for the existing 21503C source; not color-only storage.
    UniformVector4f* colorBuffer;
    unsigned char unknown670[0x24];
    unsigned char textEnabled;
    unsigned char textReserved;
    unsigned char textMode;
    void SetUpMtx(const nn::math::MTX34&);
    void SetUpTextBox(const TextBox*, const Material*, const DrawInfo&);
};
static_assert(offsetof(Drawer, colorCount) == 0x25, "uniform-vector count");
static_assert(offsetof(Drawer, colorSelector) == 0x664, "color selector uniform");
static_assert(offsetof(Drawer, colorBuffer) == 0x66c, "general uniform register buffer");
static_assert(offsetof(Drawer, textEnabled) == 0x694, "text setup flags");
static_assert(sizeof(UniformVector4f) == 16, "four-float uniform register");
} }
