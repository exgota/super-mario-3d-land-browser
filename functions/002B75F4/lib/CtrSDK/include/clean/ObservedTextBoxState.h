#pragma once
#include <clean/LayoutDrawerPrefix.h>
namespace nw { namespace lyt {
// Reconstructed storage contracts, not recovered nominal color/writer classes.
struct ObservedRgba8 {
    unsigned char r, g, b, a;
    ObservedRgba8(const ObservedRgba8& other)
        : r(other.r), g(other.g), b(other.b), a(other.a) {}
    ObservedRgba8& operator=(const ObservedRgba8& other) {
        r = other.r; g = other.g; b = other.b; a = other.a;
        return *this;
    }
};
struct ObservedTextCommands {
    unsigned char unknown00[0x14];
    unsigned commandCount;
};
struct ObservedWriter {
    ObservedRgba8 materialFirst, materialSecond;
    ObservedRgba8 cornerColors[4];
    ObservedRgba8 textFirst, textSecond;
    unsigned char colorMode;
    unsigned char unknown21[0x23];
    ObservedTextCommands* commands;
    unsigned char unknown48;
    unsigned char alpha;
};
struct ObservedRenderContext {
    unsigned char unknown00[0xf0];
    ObservedWriter writer;
};
class DrawInfo {
public:
    unsigned char unknown00[0x80];
    ObservedRenderContext* context;
};
class Material {
public:
    unsigned char unknown00[0x10];
    ObservedRgba8 firstColor, secondColor;
};
class TextBox {
public:
    unsigned char unknown00[0xb5];
    unsigned char alpha;
    unsigned char unknownB6[0x22];
    ObservedRgba8 firstColor, secondColor;
    unsigned char unknownE0[0x20];
    Material* material;
    ObservedTextCommands* commands;
};
static_assert(sizeof(ObservedRgba8) == 4, "RGBA8 channel storage");
static_assert(offsetof(ObservedWriter, textFirst) == 0x18, "text color pair");
static_assert(offsetof(ObservedWriter, colorMode) == 0x20, "color mode byte");
static_assert(offsetof(ObservedWriter, commands) == 0x44, "command storage");
static_assert(offsetof(ObservedWriter, alpha) == 0x49, "global alpha");
static_assert(offsetof(TextBox, firstColor) == 0xd8, "text colors");
static_assert(offsetof(TextBox, commands) == 0x104, "text command object");
} }
