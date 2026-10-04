#ifndef OBSERVED_TEXTURE_PARAMETER_STATE_H
#define OBSERVED_TEXTURE_PARAMETER_STATE_H
#include <nn/types.h>

namespace observed_texture {
// A field component, not a claimed inherited base or original SDK class.
struct Parameters {
    u32 magnification;
    u32 minification;
    u32 wrapS;
    u32 wrapT;
    u32 unknown10;
    s32 minimumLod;
    float border[4];
    float lodBias;
    u32 unknown2C;
    u8 rawParameter;

    static float clampBorder(int value) {
        if (value <= 0) return 0.0f;
        if (value > 1) return 1.0f;
        return static_cast<float>(value);
    }
};

// Cleanup independently advances by 0x44 for each of six cube images.
struct ImagePlaneStorage { u8 unknown[0x44]; };

template<unsigned ImageCount>
struct TextureStorage {
    Parameters parameters;
    ImagePlaneStorage images[ImageCount];

    bool setIntegerParameter(int parameter, const int* values) {
        switch (parameter) {
        case 0x2801: parameters.minification = values[0]; break;
        case 0x2800: parameters.magnification = values[0]; break;
        case 0x2802: parameters.wrapS = values[0]; break;
        case 0x2803: parameters.wrapT = values[0]; break;
        case 0x8501: parameters.lodBias = static_cast<float>(values[0]); break;
        case 0x1004:
            parameters.border[0] = Parameters::clampBorder(values[0]);
            parameters.border[1] = Parameters::clampBorder(values[1]);
            parameters.border[2] = Parameters::clampBorder(values[2]);
            parameters.border[3] = Parameters::clampBorder(values[3]);
            break;
        case 0x813A: parameters.minimumLod = values[0]; break;
        case 0x8191: parameters.rawParameter = static_cast<u8>(values[0]); break;
        default: return false;
        }
        return true;
    }
};

typedef TextureStorage<1> Texture2D;
typedef TextureStorage<6> TextureCube;
template<class Payload> struct NodePrefix { Payload* payload; };
struct ManagerPrefix {
    Texture2D* default2D;
    TextureCube* defaultCube;
    u8 unknown08[0x808];
    NodePrefix<Texture2D>* bound2D[3];
    NodePrefix<TextureCube>* boundCube[3];
};
struct ManagerGlobal { ManagerPrefix* current; u32 initialized; };
struct StatePrefix {
    u32 dirtyWords[1]; // Only this word is used by independently valid units 0..2.
    u8 unknown04[4];
    u32 flags08;
    u8 unknown0C[0x4c];
    u32 active;
    u32 names2D[3];
    u32 namesCube[3];
};
static_assert(sizeof(Parameters) == 0x34, "parameter component before image storage");
static_assert(sizeof(Texture2D) == 0x78, "independent 2D allocation and clear");
static_assert(sizeof(TextureCube) == 0x1cc, "independent cube allocation and clear");
static_assert(sizeof(ManagerGlobal) == 8, "mapped manager owner");
static_assert(offsetof(ManagerPrefix, bound2D) == 0x810, "2D node array");
static_assert(offsetof(ManagerPrefix, boundCube) == 0x81c, "cube node array");
static_assert(offsetof(StatePrefix, active) == 0x58, "active unit");
static_assert(offsetof(StatePrefix, names2D) == 0x5c, "2D names");
static_assert(offsetof(StatePrefix, namesCube) == 0x68, "cube names");
static_assert(offsetof(Parameters, rawParameter) == 0x30, "raw byte");
}
extern observed_texture::StatePrefix* dat_003E3154;
extern observed_texture::ManagerGlobal dat_003E3180;

#endif
