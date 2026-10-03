#pragma once
#include <math/seadMatrix34CalcCtr.h>
#include <nn/types.h>
namespace observed_emitter_commands {
struct CommandTemplate {
    u32 prefix[4];
    u32 color;
    u32 suffix[5];
};
static_assert(sizeof(CommandTemplate) == 40, "independently observed template stride");
static_assert(offsetof(CommandTemplate, color) == 0x10, "replaced color field");
struct Texture {
    u32 address;
    u8 unknown04[4];
    u8 format;
    u8 unknown09[0x9f];
    u32 parameterA8, parameterAC, parameterB0;
};
struct Resource {
    u8 unknown00[4];
    u32 flags;
    u8 unknown08[0x20];
    Texture* texture;
    u8 unknown2C[0x64];
    u32 selector;
    u8 unknown94[4];
    u32 blend;
    u8 unknown9C[0x3c];
    float color[3];
    u8 unknownE4[0x50];
    u32 commandTemplate;
    u8 unknown138[0x38];
    nn::math::MTX34 baseMatrix;
    nn::math::MTX34 worldMatrix;
};
struct Owner {
    u8 unknown00[0xfc];
    nn::math::MTX34 baseMatrix;
    nn::math::MTX34 worldMatrix;
    u8 unknown15C[0x30];
    float color[3];
};
struct State {
    u8 unknown00[0xc];
    Owner* owner;
    u8 unknown10[8];
    nn::math::MTX34 worldMatrix;
    nn::math::MTX34 baseMatrix;
    u8 unknown78[0x30];
    Resource* resource;
};
static_assert(offsetof(Texture, parameterA8) == 0xa8, "texture command fields");
static_assert(offsetof(Resource, selector) == 0x90, "four-way selector");
static_assert(offsetof(Resource, color) == 0xd8, "resource color factors");
static_assert(offsetof(Resource, commandTemplate) == 0x134, "template index");
static_assert(offsetof(Resource, baseMatrix) == 0x170, "resource matrix");
static_assert(offsetof(Resource, worldMatrix) == 0x1a0, "resource matrix");
static_assert(offsetof(Owner, color) == 0x18c, "owner color factors");
static_assert(offsetof(State, resource) == 0xa8, "resource pointer");
}
extern observed_emitter_commands::Texture* dat_003EF91C;
extern u8 dat_003EF920;
extern u8 dat_003EF921;
extern u8 dat_003EF922[2];
extern const u32 dat_003EF9C8[4];
extern const u32 dat_003EF9D8[3];
extern const u32 dat_003EF9E4[2][3];
extern const u32 dat_003EF9FC[3];
extern const u32 dat_003EFA08[3];
extern const observed_emitter_commands::CommandTemplate dat_003F09B4[28];
extern u32 dat_003EF8C4;
