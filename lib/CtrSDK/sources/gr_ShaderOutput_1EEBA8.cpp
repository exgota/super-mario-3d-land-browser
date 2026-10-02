// Clean reconstruction from the EU executable. Public identities are unknown.
// This is a low-level DVLE output-map / PICA command serializer; see the report.
#include <stddef.h>

namespace ShaderOutput1EEBA8 {
struct Output {
    u16 type;
    u16 reg;
    u16 mask;
    u16 reserved;
};
struct Program {
    u32 magic;
    u16 version;
    u8 kind;
    u8 merge;
    u32 entry;
    u32 end;
    u32 inputMask;
    u8 mode;
    u8 fixedStart;
    u8 variableCount;
    u8 fixedCount;
    u8 unknown18[0x10];
    u32 outputOffset;
    u32 outputCount;
};
struct Context {
    int vertex;
    int geometry;
    u32 programCount;
    const Program* programs[32];
};
static_assert_(sizeof(Output) == 8);
static_assert_(offsetof(Program, merge) == 7);
static_assert_(offsetof(Program, mode) == 0x14);
static_assert_(offsetof(Program, outputOffset) == 0x28);
static_assert_(offsetof(Program, outputCount) == 0x2c);
static_assert_(offsetof(Context, programs) == 0xc);
static inline const Output* outputs(const Program* program) {
    return reinterpret_cast<const Output*>(reinterpret_cast<const u8*>(program) + program->outputOffset);
}
// The real external 0x00284680 copies four halfwords in address order.
extern "C" Output* fn_00284680(Output*, const Output*);

#ifdef NON_MATCHING
extern "C" u32* fn_001EEBA8(const Context* context, u32* command, int vertex, int geometry) {
    bool hasGeometry = context->geometry >= 0;
    const Program* selected = context->programs[hasGeometry ? geometry : vertex];
    int count = 0;
    u32 mode = 0;
    u32 clock = 0;
    u32 outputCount = 0;
    u32 outputMask = 0;
    Output merged[64];
    if (hasGeometry && selected->merge) {
        const Output* selectedOutputs = outputs(selected);
        const Program* vertexProgram = context->programs[vertex];
        const Output* vertexOutputs = outputs(vertexProgram);
        u32 selectedUsed = 0;
        u32 vertexUsed = 0;
        for (u32 i = 0; i < selected->outputCount; ++i) {
            if (selectedOutputs[i].type < 9 && selectedOutputs[i].type != 7) {
                for (u32 j = 0; j < vertexProgram->outputCount; ++j) {
                    // Retail tests vertexOutputs[i], then compares against [j].
                    if (vertexOutputs[i].type < 9 && vertexOutputs[i].type != 7 &&
                        selectedOutputs[i].type == vertexOutputs[j].type) {
                        merged[count].type = selectedOutputs[i].type;
                        merged[count].reg = count;
                        merged[count].mask = selectedOutputs[i].mask;
                        selectedUsed |= 1u << i;
                        vertexUsed |= 1u << j;
                        ++count;
                    }
                }
            }
        }
        for (u32 i = 0; i < selected->outputCount; ++i) {
            if (!(selectedUsed & (1u << i)) && selectedOutputs[i].type < 9 && selectedOutputs[i].type != 7) {
                merged[count].type = selectedOutputs[i].type;
                merged[count].reg = count;
                merged[count].mask = selectedOutputs[i].mask;
                ++count;
            }
        }
        for (u32 i = 0; i < vertexProgram->outputCount; ++i) {
            if (!(vertexUsed & (1u << i)) && vertexOutputs[i].type < 9 && vertexOutputs[i].type != 7) {
                merged[count].type = vertexOutputs[i].type;
                merged[count].reg = count;
                merged[count].mask = vertexOutputs[i].mask;
                ++count;
            }
        }
    } else {
        const Output* source = outputs(selected);
        if (selected->outputCount) {
            Output* destination = merged;
            if (selected->outputCount & 1) {
                fn_00284680(destination++, source++);
            }
            Output pending;
            fn_00284680(&pending, source);
            for (u32 pairs = selected->outputCount >> 1; pairs != 0; --pairs) {
                Output next;
                fn_00284680(&next, source + 1);
                fn_00284680(destination, &pending);
                source += 2;
                fn_00284680(&pending, source);
                fn_00284680(destination + 1, &next);
                destination += 2;
            }
        }
        count = selected->outputCount;
    }
    u32 rasterMap[7];
    for (int reg = 0; reg < 7; ++reg) {
        rasterMap[reg] = 0x1f1f1f1f;
        for (int i = 0; i < count; ++i) {
            u32 component = 0;
            for (int lane = 0; merged[i].reg == reg && lane < 4; ++lane) {
                if (merged[i].mask & (1u << lane)) {
                    u32 value = 0x1f;
                    switch (merged[i].type) {
                    case 0: value = component++; if (component == 2) clock |= 1; break;
                    case 1: value = 4 + component++; clock |= 0x1000000; break;
                    case 2: value = 8 + component++; clock |= 2; break;
                    case 3: if (component < 2) value = 12 + component++; mode = 1; clock |= 0x100; break;
                    case 4: value = 16; mode = 1; clock |= 0x30000; break;
                    case 5: if (component < 2) value = 14 + component++; mode = 1; clock |= 0x200; break;
                    case 6: if (component < 2) value = 22 + component++; mode = 1; clock |= 0x400; break;
                    case 8: if (component < 3) value = 18 + component++; clock |= 0x1000000; break;
                    }
                    rasterMap[reg] = (rasterMap[reg] & ~(0xffu << (8 * lane))) | (value << (8 * lane));
                }
            }
        }
        if (count > 0 && rasterMap[reg] != 0x1f1f1f1f) {
            outputMask |= 1u << reg;
            ++outputCount;
        }
    }
    if (hasGeometry) {
        u32 vertexCount = 0;
        u32 vertexMask = 0;
        u32 vertexMap[16];
        const Program* vertexProgram = context->programs[vertex];
        const Output* source = outputs(vertexProgram);
        for (int reg = 0; reg < 16; ++reg) {
            vertexMap[reg] = 0x1f1f1f1f;
            for (u32 i = 0; i < vertexProgram->outputCount; ++i) {
                u32 component = 0;
                for (int lane = 0; source[i].reg == reg && lane < 4; ++lane) {
                    if (source[i].mask & (1u << lane)) {
                        u32 value = 0x1f;
                        switch (source[i].type) {
                        case 0: value = component++; break;
                        case 1: value = 4 + component++; break;
                        case 2: value = 8 + component++; break;
                        case 3: if (component < 2) value = 12 + component++; break;
                        case 4: value = 16; break;
                        case 5: if (component < 2) value = 14 + component++; break;
                        case 6: if (component < 2) value = 22 + component++; break;
                        case 8: if (component < 3) value = 18 + component++; break;
                        case 9: value = 0xff; break;
                        }
                        vertexMap[reg] = (vertexMap[reg] & ~(0xffu << (8 * lane))) | (value << (8 * lane));
                    }
                }
            }
            if (vertexProgram->outputCount && vertexMap[reg] != 0x1f1f1f1f) {
                vertexMask |= 1u << reg;
                ++vertexCount;
            }
        }
        u32 geometryMode = context->programs[geometry]->mode;
        *command++ = geometryMode == 1 ? 0x80000000 : 0;
        *command++ = 0x000a0229;
        *command++ = 0;
        *command++ = 0x00030253;
        *command++ = (geometryMode ? 0x100 : 0) | (vertexCount - 1) | 0x08000000;
        *command++ = 0x000b0289;
        *command++ = context->programs[geometry]->entry | 0x7fff0000;
        *command++ = 0x000f028a;
        *command++ = outputMask;
        *command++ = 0x000f028d;
        *command++ = context->programs[vertex]->entry | 0x7fff0000;
        *command++ = 0x000f02ba;
        *command++ = vertexMask;
        *command++ = 0x000f02bd;
        *command++ = vertexCount - 1;
        *command++ = 0x000f0251;
        *command++ = 0x76543210;
        *command++ = 0x000f028b;
        *command++ = 0xfedcba98;
        *command++ = 0x000f028c;
        if (geometryMode == 1) {
            if (context->programs[geometry]->fixedCount) {
                *command++ = context->programs[geometry]->fixedCount - 1;
                *command++ = 0x00010254;
            }
        } else if (geometryMode == 2) {
            geometryMode = 0x01000002 | (context->programs[geometry]->fixedStart << 16) |
                           (0xfffff000u + (vertexCount << 12)) |
                           (0xffffff00u + (context->programs[geometry]->fixedCount << 8));
        }
        *command++ = geometryMode;
        *command++ = 0x000f0252;
        *command++ = vertexCount - 1;
        *command++ = 0x000f024a;
    } else {
        *command++ = 0;
        *command++ = 0x00080229;
        *command++ = 0;
        *command++ = 0x00010253;
        *command++ = 0xa0000000;
        *command++ = 0x000b0289;
        *command++ = context->programs[vertex]->entry | 0x7fff0000;
        *command++ = 0x000f02ba;
        *command++ = outputMask;
        *command++ = 0x000f02bd;
        *command++ = outputCount - 1;
        *command++ = 0x000f0251;
        *command++ = 0;
        *command++ = 0x000f0252;
        *command++ = outputCount - 1;
        *command++ = 0x000f024a;
    }
    *command++ = outputCount - 1;
    *command++ = 0x0001025e;
    *command++ = outputCount;
    *command++ = 0x000f004f;
    int written = 0;
    for (int i = 0; i < 7; ++i) {
        if (rasterMap[i] != 0x1f1f1f1f) {
            *command++ = rasterMap[i];
            *command++ = 0x000f0050 + written++;
        }
    }
    for (int i = written; i < 7; ++i) {
        *command++ = rasterMap[i];
        *command++ = 0x000f0050 + i;
    }
    *command++ = mode;
    *command++ = 0x000f0064;
    *command++ = clock;
    *command++ = 0x000f006f;
    if (hasGeometry) {
        *command++ = 0;
        *command++ = 0x0008025e;
    }
    return command;
}
#endif
}
