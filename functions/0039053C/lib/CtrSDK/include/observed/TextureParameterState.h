#pragma once
#include <nn/types.h>
namespace observed_texture {
// Shared parameter prefix of separately allocated2D/cube payloads.
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
};
struct NodePrefix { Parameters* payload; };
struct ManagerPrefix {
    Parameters* default2D;
    Parameters* defaultCube;
    u8 unknown08[0x808];
    NodePrefix* bound2D[3];
    NodePrefix* boundCube[3];
};
// The entire existing8-byte static owner, independently initialized106FC4/10713C.
struct ManagerGlobal { ManagerPrefix* current; u32 initialized; };
struct StatePrefix {
    u32 dirtyWords[1]; // units0..2 use only this proven word; no extra count claim
    u8 unknown04[4];
    u32 flags08;
    u8 unknown0C[0x4c];
    u32 active;
    u32 names2D[3];
    u32 namesCube[3];
};
static_assert(sizeof(ManagerGlobal)==8,"mapped manager owner");
static_assert(offsetof(ManagerPrefix,bound2D)==0x810,"2D node array");
static_assert(offsetof(ManagerPrefix,boundCube)==0x81c,"cube node array");
static_assert(offsetof(StatePrefix,active)==0x58,"active unit");
static_assert(offsetof(StatePrefix,names2D)==0x5c,"2D names");
static_assert(offsetof(StatePrefix,namesCube)==0x68,"cube names");
static_assert(offsetof(Parameters,rawParameter)==0x30,"raw byte");
}
extern observed_texture::StatePrefix* dat_003E3154;
extern observed_texture::ManagerGlobal dat_003E3180;
