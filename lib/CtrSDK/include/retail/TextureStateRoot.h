#include <retail/GraphicsGlobals.h>
#pragma once

// Observed ARM32 views, not original SDK type names or complete allocations.
namespace retail_texture_state {
typedef unsigned int Word;
typedef unsigned char Byte;
struct Image {
    Word data;
    Word unknown04[2];
    Word width;
    Word height;
    Word format;
    Word unknown18;
    Word packedFormat;
    Word unknown20[5];
    Word mipCount;
};
struct Texture {
    Word magnification;
    Word minification;
    Word wrapS;
    Word wrapT;
    Word unknown10;
    int minimumLod;
    float border[4];
    float lodBias;
    Word unknown2c[2];
    Image image;
};
struct Unit {
    Word border;
    Word dimensions;
    Word parameters;
    Word lod;
    Word address;
};
struct Cache {
    Word enabled;
    Unit unit0;
    Word unknown18[8];
    Word format0;
    Word unknown3c[2];
    Unit unit1;
    Word format1;
    Word unknown5c[2];
    Unit unit2;
    Word format2;
};
struct ControlView {
    Byte unknown00[0xf8];
    Word textureMode[3];
    Byte textureEnabled[3];
    Byte unknown107[0x670 - 0x107];
    Cache cache;
};
}
extern "C" retail_texture_state::Word* dat_003E2E30;
extern "C" retail_texture_state::Word* dat_003E2E34;
extern "C" unsigned fn_00289C1C(unsigned address);
extern "C" void __cb_writeRegs(unsigned first, unsigned count, const unsigned* values);
extern "C" unsigned fn_003910C0(retail_texture_state::Texture* texture, int unit);
