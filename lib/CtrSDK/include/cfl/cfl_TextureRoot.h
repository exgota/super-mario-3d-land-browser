#ifndef CFL_TEXTURE_ROOT_H
#define CFL_TEXTURE_ROOT_H

// Clean-room views of fields read/written by 0x001119EC. Descriptive names
// are inferred roles, not recovered SDK symbols. Unused fields stay opaque.
namespace cfl_re {
struct Texture {
    void* owner;                  // 00: allocation identity; zero means absent
    int width, height;            // 04, 08
    int storageWidth, storageHeight; // 0C, 10
    int levels;                   // 14
    int field18, field1c;
    const void* source;           // 20
    int format;                   // 24
    void* pixels;                 // 28
    unsigned memoryKind;          // 2C
    int ownsPixels;               // 30
};
struct Quad {
    float x, y, width, height, angle;
    int mirror;
    Texture texture;
};
struct CharInfo { int word[0x120 / 4]; };
struct Resource {
    unsigned char prefix[0x67c];
    unsigned textureMemoryKind;
    unsigned char gap[12];
    unsigned flags;               // 68C
};
struct Model {
    unsigned char prefix[0x6c];
    Resource* resource;
    Texture* expression[16];
};
struct ExpressionAdjust { int firstType, secondType, firstRotation, secondRotation, secondY; };
}
extern "C" void fn_001119EC(unsigned char* occupancy, const cfl_re::CharInfo*, cfl_re::Model*, int resolution);
#endif
