#include <retail/GraphicsGlobals.h>
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
    u8 enabled;                      // +00, integer IDs57..64
    u8 unknown1[3];
    float color[4][4];               // +04, +14, +24, +34
    float vector97[4];               // +44
    float vector105[3];              // +54
    u32 enum145;                    // +60, integer IDs145..152
    float scalar161;                 // +64
    float scalar169;                 // +68
    u32 enum185;                    // +6c, integer IDs185..192
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
    u8 aggregateEnables[3];         // +96d, integer IDs226..228
    u8 flag223;                     // +970
    u8 unknown971[3];
    u32 enum214[6];                 // +974, integer IDs214..219
    u32 enum224;                    // +98c, table-selected value
    float globalColor[4];            // +990
    LightState lights[8];            // +9a0, stride70
    float materialColor[4][4];       // +d20, +d30, +d40, +d50
    float colorOffset[4];            // +d60
    u32 enum23[7];                  // +d70, integer IDs23..29
    u32 enum6;                      // +d8c
    float scalar19;                  // +d90
    float vector21[3];               // +d94
    float vector22[3];               // +da0
    u32 enum3[3];                   // +dac, integer IDs3..5
    u32 enum30;                     // +db8
    float scalar32;                  // +dbc
    float scalar31;                  // +dc0
    float scalar1;                   // +dc4
    float scalar2;                   // +dc8
    float scalar33;                  // +dcc
    float vector35[4];               // +dd0
    float scalar38;                  // +de0
    u32 enum296;                    // +de4
    float vector294[3];              // +de8
    u8 flag43;                      // +df4
    u8 unknowndf5[3];
    u32 enum47[3];                  // +df8, integer IDs47..49
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
    u8 unknown55[0xa3];
    u32 textureEnums[3];            // +0f8, integer IDs3..5
    u8 textureEnabled[3];           // +104
    u8 cubeEnabled;                 // +107
    u32 unknown108;
    u32 enum214Cache[6];            // +10c
    u32 light145Cache[8];           // +124
    u32 light185Cache[8];           // +144
    u32 enum23Cache[7];             // +164
    u32 enum296Cache;               // +180
    u32 enum47Cache[3];             // +184
    u8 unknown190[0x42c];
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
static_assert_(offsetof(State, enum214) == 0x974);
static_assert_(offsetof(State, enum23) == 0xd70);
static_assert_(offsetof(State, enum3) == 0xdac);
static_assert_(offsetof(State, enum47) == 0xdf8);
static_assert_(offsetof(Control, textureEnums) == 0xf8);
static_assert_(offsetof(Control, light145Cache) == 0x124);
static_assert_(offsetof(Control, enum47Cache) == 0x184);
static_assert_(offsetof(Control, bias) == 0x44);
static_assert_(offsetof(Control, format) == 0x5bc);
#endif

} // namespace retail_float_state

extern "C" void fn_0020ACAC(u32 location, const float* values, int width,
                           int count, int transpose, int matrix);
