// Layout evidence: retail EU fn_0020ACAC and its three direct float wrappers.
// Descriptive names only; original library and public API names are unproven.
#pragma once
#include <nn/types.h>

namespace retail_float_state {

struct LocationRecord {
    u32 unknown0;
    u32 unknown4;
    u32 packed;
};

struct LightState {
    u32 unknown0;
    float color[4][4];               // +04, +14, +24, +34
    float vector97[4];               // +44
    float vector105[3];              // +54
    u32 unknown60;
    float scalar161;                 // +64
    float scalar169;                 // +68
    u32 unknown6c;
};

struct State {
    u8 unknown0[0x1c];
    LocationRecord* locations;       // +01c
    u8 unknown20[0x0c];
    u32 (*floatRegisters0)[4];       // +02c
    u8 unknown30[0x184];
    u32 floatDirty0[3];              // +1b4
    u32 (*floatRegisters1)[4];       // +1c0
    u8 unknown1c4[0x184];
    u32 floatDirty1[3];              // +348
    u8 unknown354[0xa2];
    u8 byteEnables[190];             // +3f6
    u32 packedRegisters[189];        // +4b4
    u32 packedDirty[6];              // +7a8
    u8 unknown7c0[0x1ac];
    u8 multiplyFourthColor;          // +96c
    u8 unknown96d[0x23];
    float globalColor[4];            // +990
    LightState lights[8];            // +9a0, stride70
    float materialColor[4][4];       // +d20, +d30, +d40, +d50
    float colorOffset[4];            // +d60
    u8 unknownd70[0x20];
    float scalar19;                  // +d90
    float vector21[3];               // +d94
    float vector22[3];               // +da0
    u8 unknowndac[0x10];
    float scalar32;                  // +dbc
    float scalar31;                  // +dc0
    float scalar1;                   // +dc4
    float scalar2;                   // +dc8
    float scalar33;                  // +dcc
    float vector35[4];               // +dd0
    float scalar38;                  // +de0
    u32 unknownde4;
    float vector294[3];              // +de8
    u8 unknowndf4[0x10];
    float vector39[3];               // +e04
    float vector40[4];               // +e10
    float scalar41;                  // +e20
    float scalar42;                  // +e24
    float scalar44;                  // +e28
    float vector282[6][4];           // +e2c, stride10
    float vector288[4];              // +e8c
};

struct Context {
    State* state;
    u8 unknown4[0x1008];
    u32 complementedShadow[189];     // +100c
};

struct ContextSlots {
    u32 unknown0[2];
    Context* current;                // +8
    u32 unknownc;
};

struct Control {
    u32 dirty;
    u8 unknown4[8];
    u8 forceShadow;                  // +00c
    u8 unknownd[0x37];
    float bias;                      // +044
    u8 unknown48[4];
    float range0;                    // +04c
    float range1;                    // +050
    u8 biasEnabled;                  // +054
    u8 unknown55[0x567];
    u32 format;                      // +5bc
};

#ifdef __arm__
static_assert_(sizeof(LocationRecord) == 12);
static_assert_(sizeof(LightState) == 0x70);
static_assert_(offsetof(State, locations) == 0x1c);
static_assert_(offsetof(State, floatDirty0) == 0x1b4);
static_assert_(offsetof(State, floatRegisters1) == 0x1c0);
static_assert_(offsetof(State, floatDirty1) == 0x348);
static_assert_(offsetof(State, byteEnables) == 0x3f6);
static_assert_(offsetof(State, packedRegisters) == 0x4b4);
static_assert_(offsetof(State, packedDirty) == 0x7a8);
static_assert_(offsetof(State, lights) == 0x9a0);
static_assert_(offsetof(State, materialColor) == 0xd20);
static_assert_(offsetof(State, scalar44) == 0xe28);
static_assert_(offsetof(State, vector288) == 0xe8c);
static_assert_(offsetof(Context, complementedShadow) == 0x100c);
static_assert_(sizeof(ContextSlots) == 16);
static_assert_(offsetof(Control, bias) == 0x44);
static_assert_(offsetof(Control, format) == 0x5bc);
#endif

} // namespace retail_float_state

extern "C" retail_float_state::ContextSlots dat_003E2E40;
extern "C" retail_float_state::Control* dat_003E3154;
extern "C" void fn_0020ACAC(u32 location, const float* values, int width,
                           int count, int transpose, int matrix);
