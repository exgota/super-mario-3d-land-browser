#include <retail/ShaderBinary.h>
#include <string.h>

// NonMatching: full ordinary-C++ reconstruction of EU 002478D8..00248A48.
// Binary data is trusted exactly as by the retail routine; no bounds checks or
// allocation-failure cleanup have been added. See the report for valid domains.
#ifdef NON_MATCHING
#define BINARY_ALLOCATE(bytes) (dat_003E2654 ? dat_003E2654(0x10000, 0x100, 0, (bytes)) : 0)
extern "C" void fn_002478D8(int count, const unsigned* shaders, unsigned format,
                            const void* binary, int length) {
    using namespace retail_shader_binary;
    const BinaryHeader* header = static_cast<const BinaryHeader*>(binary);
    const ProgramHeader* program = reinterpret_cast<const ProgramHeader*>(
        &header->offsets[header->stageCount]);
    int failed = 1;
    Resource* resource = static_cast<Resource*>(BINARY_ALLOCATE(sizeof(Resource)));
    if (!resource) return;
    memset(resource, 0, sizeof(Resource));
    resource->code = static_cast<Word*>(BINARY_ALLOCATE(program->codeCount * 4));
    resource->codeCount = program->codeCount;
    resource->operands = static_cast<Word*>(BINARY_ALLOCATE(program->operandCount * 4));
    resource->operandCount = program->operandCount;
    if (!resource->code || !resource->operands) return;
    const Word* code = reinterpret_cast<const Word*>(
        reinterpret_cast<const Byte*>(program) + program->codeOffset);
    for (Word i = 0; i < program->codeCount; ++i) resource->code[i] = code[i];
    const OperandDescriptor* operands = reinterpret_cast<const OperandDescriptor*>(
        reinterpret_cast<const Byte*>(program) + program->operandOffset);
    for (Word i = 0; i < program->operandCount; ++i, ++operands) resource->operands[i] = operands->lower;
    resource->stages = static_cast<Stage*>(BINARY_ALLOCATE(header->stageCount * sizeof(Stage)));
    resource->stageCount = header->stageCount;
    if (resource->stages) {
        fn_0028D1F0(resource->stages, header->stageCount * sizeof(Stage));
        const Word* stageOffsets = header->offsets;
        Word stageIndex;
        for (stageIndex = 0; stageIndex < header->stageCount; ++stageIndex) {
            const StageHeader* stage = reinterpret_cast<const StageHeader*>(
                static_cast<const Byte*>(binary) + stageOffsets[stageIndex]);
#define BINARY_STAGE resource->stages[stageIndex]
            BINARY_STAGE.type = stage->type;
            BINARY_STAGE.mergeOutputs = stage->flags & 1;
            BINARY_STAGE.geometryMode = stage->geometryMode;
            BINARY_STAGE.geometryParam3 = stage->geometryParam3;
            BINARY_STAGE.geometryParam4 = stage->geometryParam4;
            BINARY_STAGE.geometryParam5 = stage->geometryParam5;
            BINARY_STAGE.inputMask = stage->inputMask;
            BINARY_STAGE.outputMask = stage->outputMask;
            BINARY_STAGE.entryPoint = stage->entryPoint;
            BINARY_STAGE.endPoint = stage->endPoint;
            const BinaryConstant* constants = reinterpret_cast<const BinaryConstant*>(
                reinterpret_cast<const Byte*>(stage) + stage->constantOffset);
            for (Word i = 0; i < stage->constantCount; ++i)
                if (constants[i].type == 2) ++BINARY_STAGE.constantCount;
            if (BINARY_STAGE.constantCount) {
                BINARY_STAGE.constants = static_cast<Constant*>(BINARY_ALLOCATE(BINARY_STAGE.constantCount * sizeof(Constant)));
                if (!BINARY_STAGE.constants) break;
                BINARY_STAGE.constantCount = 0;
            }
            for (Word i = 0; i < stage->constantCount; ++i) {
                const BinaryConstant& c = constants[i];
                switch (c.type) {
                case 0:
                    if (c.value[0]) BINARY_STAGE.boolConstants |= 1u << c.index;
                    break;
                case 1:
                    if (c.index < 4) BINARY_STAGE.integerConstants[c.index] = c.value[0];
                    break;
                case 2:
                    BINARY_STAGE.constants[BINARY_STAGE.constantCount].index = c.index;
                    BINARY_STAGE.constants[BINARY_STAGE.constantCount].packed[0] = (c.value[3] << 8) | (c.value[2] >> 16);
                    BINARY_STAGE.constants[BINARY_STAGE.constantCount].packed[1] = (c.value[2] << 16) | (c.value[1] >> 8);
                    BINARY_STAGE.constants[BINARY_STAGE.constantCount].packed[2] = c.value[0] | (c.value[1] << 24);
                    ++BINARY_STAGE.constantCount;
                    break;
                }
            }
            BINARY_STAGE.inputCount = 0;
            for (Word i = 0; i < 16; ++i)
                if (stage->inputMask & (1u << i)) ++BINARY_STAGE.inputCount;
            BINARY_STAGE.outputCount = 0;
            for (Word i = 0; i < 16; ++i)
                if (stage->outputMask & (1u << i)) ++BINARY_STAGE.outputCount;
            const BinaryOutput* outputs = reinterpret_cast<const BinaryOutput*>(
                reinterpret_cast<const Byte*>(stage) + stage->outputOffset);
            Word outputMask = stage->outputMask;
            for (Word i = 0; i < stage->outputCount; ++i)
                if (outputs[i].semantic == 9) outputMask &= ~(1u << outputs[i].index);
            for (Word i = 0; i < 7; ++i) BINARY_STAGE.outputs[i] = 0x1f1f1f1f;
            Word outputIndex;
            for (outputIndex = 0; outputIndex < stage->outputCount; ++outputIndex) {
                Word packedIndex = 0, selected = 0;
                int component = 0;
                switch (outputs[outputIndex].semantic) {
                case 1: component = 4; break;
                case 2: component = 8; break;
                case 3: component = 12; break;
                case 4: component = 16; break;
                case 5: component = 14; break;
                case 6: component = 22; break;
                case 8: component = 18; break;
                case 9: continue;
                }
                for (Word i = 0; i != outputs[outputIndex].index; ++i)
                    if ((outputMask >> i) & 1) ++packedIndex;
                for (int i = 0; i < 4; ++i) {
                    if (outputs[outputIndex].mask & (1u << i)) {
                        ++selected;
                        BINARY_STAGE.outputs[packedIndex] &= ~(0xffu << (i * 8));
                        BINARY_STAGE.outputs[packedIndex] |= static_cast<Word>(component) << (i * 8);
                        switch (component) {
                        case 2: BINARY_STAGE.outputFlags |= 1; break;
                        case 4: case 18: BINARY_STAGE.outputFlags |= 0x1000000; break;
                        case 8: BINARY_STAGE.outputFlags |= 2; break;
                        case 12: BINARY_STAGE.outputFlags |= 0x100; break;
                        case 14: BINARY_STAGE.outputFlags |= 0x200; break;
                        case 16: BINARY_STAGE.outputFlags |= 0x10000; break;
                        case 22: BINARY_STAGE.outputFlags |= 0x400; break;
                        }
                        ++component;
                    }
                    switch (outputs[outputIndex].semantic) {
                    case 3: case 5: case 6:
                        if (selected == 2) i = 4;
                        break;
                    case 4:
                        if (selected == 1) i = 4;
                        break;
                    case 8:
                        if (selected == 3) i = 4;
                        break;
                    }
                }
            }
            if (outputIndex != stage->outputCount) break;
            BINARY_STAGE.names = static_cast<char*>(BINARY_ALLOCATE(stage->nameBytes));
            if (!BINARY_STAGE.names) break;
            __rt_memcpy(BINARY_STAGE.names, reinterpret_cast<const Byte*>(stage) + stage->nameOffset, stage->nameBytes);
            BINARY_STAGE.nameBytes = stage->nameBytes;
            const BinaryVariable* variables = reinterpret_cast<const BinaryVariable*>(
                reinterpret_cast<const Byte*>(stage) + stage->variableOffset);
            for (Word i = 0; i < stage->variableCount; ++i)
                if (variables[i].first >= 16) ++BINARY_STAGE.uniformCount;
            if (BINARY_STAGE.uniformCount) {
                BINARY_STAGE.uniforms = static_cast<Uniform*>(BINARY_ALLOCATE(BINARY_STAGE.uniformCount * sizeof(Uniform)));
                if (!BINARY_STAGE.uniforms) break;
                BINARY_STAGE.uniformCount = 0;
            }
            for (Word i = 0; i < stage->variableCount; ++i) {
                const BinaryVariable& v = variables[i];
                char* name = BINARY_STAGE.names + v.nameOffset;
                int dot = -1, components = 0;
                for (int j = 0; name[j]; ++j) {
                    if (name[j] == '.') dot = j;
                    else if (dot != -1) {
                        if (name[j] == 'x' || name[j] == 'y' || name[j] == 'z' || name[j] == 'w') ++components;
                        else { dot = -1; components = 0; }
                    }
                }
                if (!components) components = 4;
                if (v.last < 16) {
                    switch (v.last - v.first) {
                    case 0:
                        switch (components) {
                        case 1: BINARY_STAGE.attributes[v.first].type = 0x1406; break;
                        case 2: BINARY_STAGE.attributes[v.first].type = 0x8b50; break;
                        case 3: BINARY_STAGE.attributes[v.first].type = 0x8b51; break;
                        case 4: BINARY_STAGE.attributes[v.first].type = 0x8b52; break;
                        }
                        break;
                    case 1: BINARY_STAGE.attributes[v.first].type = 0x8b5a; break;
                    case 2: BINARY_STAGE.attributes[v.first].type = 0x8b5b; break;
                    case 3: BINARY_STAGE.attributes[v.first].type = 0x8b5c; break;
                    }
                    BINARY_STAGE.attributes[v.first].nameOffset = v.nameOffset;
                    if (dot != -1) name[dot] = 0;
                } else if (v.last < 0x70) {
                    int span = v.last - v.first + 1;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].registerCount = span;
                    switch (components) {
                    case 1: BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].type = 0x1406; break;
                    case 2: BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].type = (span & 1) ? 0x8b50 : 0x8b5a; break;
                    case 3: BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].type = (span % 3) ? 0x8b51 : 0x8b5b; break;
                    case 4: BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].type = (span & 3) ? 0x8b52 : 0x8b5c; break;
                    }
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].firstRegister = v.first - 16;
                    if (dot != -1) {
                        switch (name[dot + 1]) {
                        default: case 'x': BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].component = 0; break;
                        case 'y': BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].component = 1; break;
                        case 'z': BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].component = 2; break;
                        case 'w': BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].component = 3; break;
                        }
                        name[dot] = 0;
                    } else BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].component = 0;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].nameOffset = v.nameOffset;
                    ++BINARY_STAGE.uniformCount;
                } else if (v.last < 0x78) {
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].type = 0x8b54;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].firstRegister = v.first - 0x70;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].component = 0;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].nameOffset = v.nameOffset;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].registerCount = v.last - v.first + 1;
                    ++BINARY_STAGE.uniformCount;
                } else {
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].type = 0x8b56;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].firstRegister = v.first - 0x78;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].component = 0;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].nameOffset = v.nameOffset;
                    BINARY_STAGE.uniforms[BINARY_STAGE.uniformCount].registerCount = v.last - v.first + 1;
                    ++BINARY_STAGE.uniformCount;
                }
            }
#undef BINARY_STAGE
        }
        if (stageIndex == header->stageCount) failed = 0;
    }
    if (failed) return;
#define BINARY_CONTEXT (*reinterpret_cast<Context*>(dat_003E2E40.current))
    for (int i = 0; i < count; ++i) {
        Word handle = shaders[i];
        Shader* shader = BINARY_CONTEXT.shaders[handle & 511];
        if (shader) {
            do {
                if (shader->handle == handle) break;
                shader = shader->next;
            } while (shader);
        }
        if (shader->resource) {
            --shader->resource->references;
            if (!shader->resource->references) fn_0020F690(shader->resource);
        }
        shader->resource = resource;
        shader->index = i;
    }
    if (failed) return;
    resource->references = count;
    resource->previous = 0;
    resource->next = BINARY_CONTEXT.resources;
    if (BINARY_CONTEXT.resources) BINARY_CONTEXT.resources->previous = resource;
    BINARY_CONTEXT.resources = resource;
#undef BINARY_CONTEXT
}
#undef BINARY_ALLOCATE
#endif
