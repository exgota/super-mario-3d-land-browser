#pragma once
#include <nn/types.h>

// Clean-room layout prefixes recovered from EU 00245D50 and independent
// bind/attach/validate callers. Names describe use, not an original SDK API.
namespace retail_program_link {
typedef unsigned char Byte;
typedef unsigned int Word;
struct Uniform {
    Word type;
    int firstRegister;
    Word qualifier;                 // component suffix offset 0..3
    Word nameOffset;
    Word registerCount;
};
struct Attribute { Word type; Word nameOffset; };
struct Stage {
    Byte unknown0;                  // shader-stage type, observed by binary constructor
    Byte mergeOutputs;
    Byte geometryMode;
    Byte geometryParam3;
    Byte geometryParam4;
    Byte geometryParam5;
    u16 inputMask;
    u16 outputMask;
    u16 unknowna;
    Word countC;                    // inputMask popcount, binary-constructor evidence
    Word count10;                   // outputMask popcount, before link composition
    Word word14;                    // entry point
    Word unknown18;                 // end offset
    Word words1C[5];                // one boolean mask and four packed integer words
    Word* constants;
    Word constantCount;
    Word outputs[7];
    Word outputFlags;
    Uniform* uniforms;
    Word uniformCount;
    Attribute attributes[16];
    char* names;
    Word unknowne4;                 // string-table size
};
struct Resource { Word* code; Word codeCount; Word* auxiliary; Word auxiliaryCount; Stage* stages; };
struct StageReference { Resource* resource; Word index; };
struct Binding { char* name; Word destination; Binding* next; };
struct Location { Word token; Word nameOffset; Word packed; };
struct BoundAttribute { Word type; Word source; Word nameOffset; };
struct Program {
    Program* next;
    Word handle;
    Word attached;
    StageReference* primary;
    StageReference* secondary;
    Byte changed;
    Byte blocked;
    Byte linked;
    Byte changedAtLink;
    Binding* bindings;
    Location* locations;
    Word locationCount;
    Word secondaryLocationStart;
    Word builtinLocationStart;
    Word (*primaryRegisters)[4];
    Word primaryRegisterIndices[96];
    Word primaryRegisterCount;
    Word primaryDirty[3];
    Word (*secondaryRegisters)[4];
    Word secondaryRegisterIndices[96];
    Word secondaryRegisterCount;
    Word secondaryDirty[3];
    BoundAttribute attributes[12];
    Resource* linkedResource;
    Word primaryStageIndex;
    Word secondaryStageIndex;
    Word outputFlags;
    Byte hasSecondary;
    Byte unknown3f5;
    Byte byteEnables[190];
    Word packedRegisters[189];
    Word packedDirty[6];
    Byte unknown7c0[0x1ac];
    Byte settings[0x530];
};
struct Context {
    Program* current;
    Word unknown4;
    Program* buckets[512];
    Byte unknown808[0xaf8];
    Word defaultRegisters[189];
    Byte unknown15f4[0xbd];
    Byte defaultByteEnables[189];
};
struct Slots { Word unknown0[2]; Context* current; Word unknownC; };
struct Builtin { Word unknown0; Word type; Word unknown8; };
struct ReleaseSlot { void (*callback)(Word, Word, Word, void*); Word unknown4[3]; };

#ifdef __arm__
static_assert_(sizeof(Uniform) == 20);
static_assert_(sizeof(Stage) == 0xe8);
static_assert_(offsetof(Stage, uniforms) == 0x58);
static_assert_(offsetof(Program, primaryRegisterCount) == 0x1b0);
static_assert_(offsetof(Program, secondaryRegisterCount) == 0x344);
static_assert_(offsetof(Program, attributes) == 0x354);
static_assert_(offsetof(Program, linkedResource) == 0x3e4);
static_assert_(offsetof(Program, byteEnables) == 0x3f6);
static_assert_(offsetof(Program, packedRegisters) == 0x4b4);
static_assert_(offsetof(Program, packedDirty) == 0x7a8);
static_assert_(offsetof(Program, settings) == 0x96c);
static_assert_(sizeof(Program) == 0xe9c);
static_assert_(offsetof(Context, defaultRegisters) == 0x1300);
static_assert_(offsetof(Context, defaultByteEnables) == 0x16b1);
#endif
}
extern "C" {
// Keep the shader-validator declaration contract; cast the observed current slot.
extern unsigned dat_003E2E40[4];
extern void* (*dat_003E2654)(unsigned, unsigned, unsigned, unsigned);
extern retail_program_link::ReleaseSlot dat_003E2658;
extern retail_program_link::Byte* dat_003E3154;
// 297 three-word entries: original initializer 001064E4 populates IDs0..296
// from 003A2F7C; the next independent table starts at00420F4C. No definition.
extern retail_program_link::Builtin dat_00420160[297];
void fn_0028D1F0(void*, unsigned);
void __aeabi_memcpy4(void*, const void*, unsigned);
void __rt_memcpy(void*, const void*, unsigned);
int strcmp(const char*, const char*);
void fn_00245D50(unsigned handle);
}
