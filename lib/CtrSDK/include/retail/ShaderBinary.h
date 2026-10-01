#include <retail/GraphicsGlobals.h>
#pragma once
#include <nn/types.h>

// EU 002478D8 constructs these records; 0020F690 releases them and 00245D50
// consumes the stage records. Names describe observed use, not SDK provenance.
namespace retail_shader_binary {
typedef unsigned Word;
typedef unsigned char Byte;
struct Constant { Word index; Word packed[3]; };
struct Uniform { Word type; int firstRegister; Word component; Word nameOffset; int registerCount; };
struct Attribute { Word type; Word nameOffset; };
struct Stage {
    Byte type, mergeOutputs, geometryMode, geometryParam3, geometryParam4, geometryParam5;
    u16 inputMask, outputMask, unknownA;
    Word inputCount, outputCount, entryPoint, endPoint;
    Word boolConstants, integerConstants[4];
    Constant* constants;
    Word constantCount;
    Word outputs[7];
    Word outputFlags;
    Uniform* uniforms;
    Word uniformCount;
    Attribute attributes[16];
    char* names;
    Word nameBytes;
};
struct Resource {
    Word* code; Word codeCount;
    Word* operands; Word operandCount;
    Stage* stages; Word stageCount;
    int references;
    Resource* previous;
    Resource* next;
};
struct Shader {
    Resource* resource; Word index; Word handle; Word type;
    Word unknown10, unknown14;
    Shader* next;
};
struct Context {
    Word unknown0[0x808 / 4];
    Shader* shaders[512];
    Resource* resources;
};
struct Slots { Word unknown0, unknown4; Context* current; Word unknownC; };
struct BinaryHeader { Word magic, stageCount; Word offsets[1]; };
struct ProgramHeader {
    Word magic, version;
    Word codeOffset, codeCount;
    Word operandOffset, operandCount;
};
struct StageHeader {
    Word magic;
    u16 version;
    Byte type, flags;
    Word entryPoint, endPoint;
    u16 inputMask, outputMask;
    Byte geometryMode, geometryParam3, geometryParam4, geometryParam5;
    Word constantOffset, constantCount;
    Word labelOffset, labelCount;
    Word outputOffset, outputCount;
    Word variableOffset, variableCount;
    Word nameOffset, nameBytes;
};
struct OperandDescriptor { Word lower, upper; };
struct BinaryConstant { u16 type, index; Word value[4]; };
struct BinaryOutput { u16 semantic, index, mask, unknown6; };
struct BinaryVariable { Word nameOffset; u16 first, last; };
#ifdef __arm__
static_assert_(sizeof(Stage) == 0xe8);
static_assert_(sizeof(Resource) == 0x24);
static_assert_(sizeof(Shader) == 0x1c);
static_assert_(sizeof(StageHeader) == 0x40);
static_assert_(offsetof(Stage, constants) == 0x30);
static_assert_(offsetof(Stage, uniforms) == 0x58);
static_assert_(offsetof(Stage, names) == 0xe0);
static_assert_(offsetof(Context, shaders) == 0x808);
static_assert_(offsetof(Context, resources) == 0x1008);
#endif
}
extern "C" {
extern void* (*dat_003E2654)(unsigned, unsigned, unsigned, unsigned);
void fn_0028D1F0(void*, unsigned);
void __rt_memcpy(void*, const void*, unsigned);
void fn_0020F690(retail_shader_binary::Resource*);
void fn_002478D8(int count, const unsigned* shaders, unsigned format,
                 const void* binary, int length);
}
