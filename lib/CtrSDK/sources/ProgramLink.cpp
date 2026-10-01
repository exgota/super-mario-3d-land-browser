// NonMatching proposal for EU 00245D50..002476CC. See the branch report.
// Valid object/descriptor inputs are assumed, as in retail. In particular,
// non-builtin uniform ranges must lie within96 registers, attribute bindings
// within12 slots, and merged output lists must fit7 words. Allocation failure
// for a nonempty register bank reaches the retail zero-fill with a null pointer.
// This source deliberately does not manufacture recovery for those states.
#include <retail/ProgramLink.h>
#ifdef NON_MATCHING
namespace retail_program_link {
static inline Context* currentContext()
{ return reinterpret_cast<Context*>(dat_003E2E40.current); }
static inline Word& word(void* object, unsigned offset)
{ return *reinterpret_cast<Word*>(static_cast<Byte*>(object) + offset); }
static inline float& scalar(Program* object, unsigned offset)
{ return *reinterpret_cast<float*>(reinterpret_cast<Byte*>(object) + offset); }
static inline Byte& byte(void* object, unsigned offset)
{ return static_cast<Byte*>(object)[offset]; }
static inline void* allocate(unsigned size)
{ return dat_003E2654 ? dat_003E2654(0x10000, 0x100, 0, size) : 0; }
static inline void release(void* allocation)
{ if (allocation && dat_003E2658.callback) dat_003E2658.callback(0x10000, 0x100, 0, allocation); }
static inline void collectRegisters(Stage* stage, Word* indices, Word& count)
{
    Word used[3] = {0, 0, 0};
    for (Word i = 0; i < stage->uniformCount; ++i) {
        Uniform* uniform = stage->uniforms + i;
        if (uniform->type == 0x8b54 || uniform->type == 0x8b56) continue;
        for (Word j = 0; j < uniform->registerCount; ++j) {
            int reg = uniform->firstRegister + j;
            used[reg >> 5] |= 1u << (reg & 31);
        }
    }
    for (Word block = 0; block < 3; ++block) {
        Word bits = used[block];
        for (Word bit = 0; bits && bit < 32; ++bit, bits >>= 1)
            if (bits & 1) indices[count++] = bit + block * 32;
    }
}
static inline void makeLocations(Program* program, Stage* stage, Word handle,
                                Word start, Word* indices, Word count, bool secondary)
{
    for (Word i = 0; i < stage->uniformCount; ++i) {
        Uniform* uniform = stage->uniforms + i;
        Word packed = ((uniform->qualifier << 11) & 0x1800) |
                      ((uniform->registerCount << 17) & 0xfe0000) |
                      (secondary ? 0x400 : 0);
        switch (uniform->type) {
        case 0x8b50: packed = (packed & ~0x6000u) | 0x2000; break;
        case 0x8b51: packed = (packed & ~0x6000u) | 0x4000; break;
        case 0x8b52: packed |= 0x6000; break;
        case 0x8b54: packed = (packed & ~0xff006000u) | 0x14000 |
                              (static_cast<Word>(uniform->firstRegister) << 24); break;
        case 0x8b56: packed = (packed & ~0xff000000u) | 0x10000 |
                              (static_cast<Word>(uniform->firstRegister) << 24); break;
        case 0x8b5a: packed = (packed & ~0x6000u) | 0xa000; break;
        case 0x8b5b: packed = (packed & ~0x6000u) | 0xc000; break;
        case 0x8b5c: packed |= 0xe000; break;
        }
        if (!(packed & 0x10000)) {
            for (Word dense = 0; dense < count; ++dense) {
                if (indices[dense] == static_cast<Word>(uniform->firstRegister)) {
                    packed = (packed & ~0xff000000u) | (dense << 24);
                    break;
                }
            }
        }
        Location* location = program->locations + start + i;
        location->packed = packed;
        location->nameOffset = uniform->nameOffset;
        location->token = ((handle << 19) & ~0x3ff80u) | (((start + i) << 7) & 0x3ffff);
    }
}
static inline void cache(Program* program, Word index, Word value,
                         Word mask = ~0u, Byte enable = 15)
{
    if ((program->packedRegisters[index] & mask) != value) {
        program->packedRegisters[index] = (program->packedRegisters[index] & ~mask) | value;
        program->packedDirty[index >> 5] |= 1u << (index & 31);
    }
    if (mask == ~0u) program->byteEnables[index] = enable;
    else program->byteEnables[index] |= enable;
}
static inline void initializeSettings(Program* program)
{
    fn_0028D1F0(program->settings, 0x530);
    byte(program, 0x96c) = 1;
    for (unsigned i = 0; i < 6; ++i) word(program, 0x974 + i * 4) = ~0u;
    word(program, 0x98c) = 0x61;
    scalar(program, 0x990) = scalar(program, 0x994) = scalar(program, 0x998) = 0.2f;
    scalar(program, 0x99c) = 1.0f;
    for (unsigned i = 0; i < 8; ++i) scalar(program, 0x9b4 + i * 4) = 1.0f;
    for (unsigned i = 0; i < 8; ++i) {
        scalar(program, 0x9ec + i * 0x70) = 1.0f;
        scalar(program, 0x9fc + i * 0x70) = -1.0f;
        word(program, 0xa00 + i * 0x70) = ~0u;
        word(program, 0xa0c + i * 0x70) = ~0u;
        scalar(program, 0xa08 + i * 0x70) = 1.0f;
    }
    for (unsigned i = 0; i < 3; ++i) {
        scalar(program, 0xd20 + i * 4) = 0.2f;
        scalar(program, 0xd30 + i * 4) = 0.8f;
    }
    for (unsigned i = 0; i < 5; ++i) scalar(program, 0xd2c + i * 16) = 1.0f;
    for (unsigned i = 0; i < 7; ++i) word(program, 0xd70 + i * 4) = ~0u;
    scalar(program, 0xd90) = 0.5f;
    word(program, 0xdb8) = 0x6030;
    scalar(program, 0xdbc) = 1.0f;
    word(program, 0xde4) = ~0u;
    byte(program, 0xdf4) = 1;
    for (unsigned i = 0; i < 3; ++i) word(program, 0xdf8 + i * 4) = ~0u;
    scalar(program, 0xe24) = scalar(program, 0xe28) = 1.0f;
    scalar(program, 0xe20) = 10.0f;
}
}

extern "C" void fn_00245D50(unsigned handle)
{
    using namespace retail_program_link;
    Program* program = currentContext()->buckets[handle & 511];
    while (program && program->handle != handle) program = program->next;
    program->linked = 0;
    if (program->blocked || !program->primary || !program->attached ||
        !program->primary->resource ||
        (program->secondary && program->secondary->resource != program->primary->resource))
        return;
    Stage* primary = program->primary->resource->stages + program->primary->index;
    Stage* secondary = program->secondary ?
        program->secondary->resource->stages + program->secondary->index : 0;
    Word primaryIndices[96], secondaryIndices[96];
    Word primaryCount = 0, secondaryCount = 0;
    Word (*primaryRegisters)[4] = 0;
    Word (*secondaryRegisters)[4] = 0;
    Location* locations = 0;
    collectRegisters(primary, primaryIndices, primaryCount);
    Word locationCount = primary->uniformCount;
    Word secondaryStart = 0;
    if (primaryCount) {
        primaryRegisters = static_cast<Word (*)[4]>(allocate(primaryCount * 16));
        fn_0028D1F0(primaryRegisters, primaryCount * 16);
    }
    if (program->secondary) {
        collectRegisters(secondary, secondaryIndices, secondaryCount);
        secondaryStart = locationCount;
        locationCount += secondary->uniformCount;
        if (secondaryCount) {
            secondaryRegisters = static_cast<Word (*)[4]>(allocate(secondaryCount * 16));
            fn_0028D1F0(secondaryRegisters, secondaryCount * 16);
        }
    }
    Word builtinStart = locationCount;
    if (locationCount <= 0x800) {
        locationCount += 297;
        locations = static_cast<Location*>(allocate(locationCount * 12));
        if (locations) fn_0028D1F0(locations, locationCount * 12);
    }
    if (!locations) {
        release(secondaryRegisters);
        release(primaryRegisters);
        return;
    }
    release(program->locations);
    program->locations = 0;
    release(program->primaryRegisters);
    program->primaryRegisters = 0;
    program->primaryRegisterCount = 0;
    for (Word i = 0; i < 3; ++i) program->primaryDirty[i] = 0;
    release(program->secondaryRegisters);
    program->secondaryRegisters = 0;
    program->secondaryRegisterCount = 0;
    for (Word i = 0; i < 3; ++i) program->secondaryDirty[i] = 0;
    for (Word i = 0; i < 12; ++i) program->attributes[i].source = ~0u;
    program->primaryRegisterCount = primaryCount;
    for (Word i = 0; i < primaryCount; ++i) program->primaryRegisterIndices[i] = primaryIndices[i];
    program->primaryRegisters = primaryRegisters;
    program->secondaryRegisterCount = secondaryCount;
    for (Word i = 0; i < secondaryCount; ++i) program->secondaryRegisterIndices[i] = secondaryIndices[i];
    program->secondaryRegisters = secondaryRegisters;
    program->secondaryLocationStart = secondaryStart;
    program->builtinLocationStart = builtinStart;
    program->locationCount = locationCount;
    program->locations = locations;
    makeLocations(program, primary, handle, 0, program->primaryRegisterIndices,
                  program->primaryRegisterCount, false);
    if (program->secondary)
        makeLocations(program, secondary, handle, program->secondaryLocationStart,
                      program->secondaryRegisterIndices, program->secondaryRegisterCount, true);
    for (Word i = program->builtinLocationStart; i < program->locationCount; ++i) {
        Word id = i - program->builtinLocationStart;
        Word token = (static_cast<u16>(id) << 2) | (handle << 19) | 0x40000;
        switch (dat_00420160[(token >> 2) & 0xffff].type) {
        case 0x8b50: case 0x8b53: token = (token & ~3u) | 1; break;
        case 0x8b51: case 0x8b54: token = (token & ~3u) | 2; break;
        case 0x8b52: case 0x8b55: token |= 3; break;
        default: token &= ~3u; break;
        }
        program->locations[i].token = token;
        program->locations[i].nameOffset = id;
        program->locations[i].packed = 0;
    }
    Word usedAttributes = 0;
    for (Binding* binding = program->bindings; binding; binding = binding->next) {
        for (Word i = 0; i < 16; ++i) {
            if (primary->attributes[i].type &&
                !strcmp(primary->names + primary->attributes[i].nameOffset, binding->name)) {
                usedAttributes |= 1u << i;
                program->attributes[binding->destination].source = i;
                program->attributes[binding->destination].type = primary->attributes[i].type;
                program->attributes[binding->destination].nameOffset = primary->attributes[i].nameOffset;
                break;
            }
        }
    }
    for (Word i = 0; i < 16; ++i) {
        if (!primary->attributes[i].type || (usedAttributes & (1u << i))) continue;
        for (Word slot = 0; slot < 12; ++slot) {
            if (program->attributes[slot].source == ~0u) {
                usedAttributes |= 1u << i;
                program->attributes[slot].source = i;
                program->attributes[slot].type = primary->attributes[i].type;
                program->attributes[slot].nameOffset = primary->attributes[i].nameOffset;
                break;
            }
        }
    }
    Word outputs[7];
    Word secondaryMask = 0, secondaryOutputCount = 0;
    if (program->secondary) {
        if (secondary->mergeOutputs) {
            Word count = 0, usedPrimary = 0, usedSecondary = 0;
            for (Word i = 0; i < 7 && secondary->outputs[i] != 0x1f1f1f1f; ++i) {
                for (Word j = 0; j < 7; ++j) {
                    if (secondary->outputs[i] == primary->outputs[j]) {
                        outputs[count++] = secondary->outputs[i];
                        usedSecondary |= 1u << i;
                        usedPrimary |= 1u << j;
                        break;
                    }
                }
            }
            for (Word i = 0; i < 7 && secondary->outputs[i] != 0x1f1f1f1f; ++i) {
                if (!(usedSecondary & (1u << i))) {
                    outputs[count++] = secondary->outputs[i];
                    if (count == 7) break;
                }
            }
            for (Word i = 0; i < 7 && primary->outputs[i] != 0x1f1f1f1f; ++i) {
                if (!(usedPrimary & (1u << i))) {
                    outputs[count++] = primary->outputs[i];
                    if (count == 7) break;
                }
            }
            for (Word i = count; i < 7; ++i) outputs[i] = 0x1f1f1f1f;
            secondaryOutputCount = count;
            Word bits = secondary->outputMask, extent = 0;
            while (count && bits) {
                if (bits & 1) --count;
                bits >>= 1;
                ++extent;
            }
            secondaryMask = secondary->outputMask & ((1u << extent) - 1);
            program->outputFlags = secondary->outputFlags | primary->outputFlags;
        } else {
            for (Word i = 0; i < 7; ++i) outputs[i] = secondary->outputs[i];
            secondaryMask = secondary->outputMask;
            secondaryOutputCount = secondary->count10;
            program->outputFlags = secondary->outputFlags;
        }
    } else {
        for (Word i = 0; i < 7; ++i) outputs[i] = primary->outputs[i];
        program->outputFlags = primary->outputFlags;
    }
    __aeabi_memcpy4(program->packedRegisters, currentContext()->defaultRegisters, 0x2f4);
    __rt_memcpy(program->byteEnables, currentContext()->defaultByteEnables, 0xbd);
    initializeSettings(program);
    cache(program, 15, primary->word14 | 0x7fff0000);
    cache(program, 9, primary->words1C[0] | 0x7fff0000);
    for (Word i = 1; i < 5; ++i) cache(program, 9 + i, primary->words1C[i]);
    cache(program, 16, primary->outputMask);
    cache(program, 14, (primary->countC - 1) & 255, 255, 1);
    if (program->secondary) {
        cache(program, 6, (primary->count10 - 1) & 255, 255, 1);
        cache(program, 7, secondary->word14 | 0x7fff0000);
        cache(program, 1, secondary->words1C[0] | 0x7fff0000);
        for (Word i = 1; i < 5; ++i) cache(program, 1 + i, secondary->words1C[i]);
        cache(program, 8, secondaryMask);
        cache(program, 25, secondaryOutputCount);
    } else {
        cache(program, 25, primary->count10);
    }
    for (Word i = 0; i < 7; ++i) cache(program, 26 + i, outputs[i]);
    cache(program, 40, program->outputFlags);
    cache(program, 38, (program->outputFlags & 0x10700) != 0);
    if (program->secondary) {
        switch (secondary->geometryMode) {
        case 0:
            cache(program, 0, 0, 0xff000000, 8);
            cache(program, 18, 0);
            cache(program, 6, 0x08000000, 0xff00ff00, 10);
            break;
        case 1:
            cache(program, 0, 0x80000000, 0xff000000, 8);
            cache(program, 18, 1);
            cache(program, 6, 0x08000100, 0xff00ff00, 10);
            cache(program, 19, static_cast<Word>(secondary->geometryParam4) - 1);
            break;
        case 2:
            cache(program, 0, 0, 0xff000000, 8);
            cache(program, 18, (0xfffff000 + (primary->count10 << 12)) |
                  ((static_cast<Word>(secondary->geometryParam5) << 8) - 0x100) |
                  (static_cast<Word>(secondary->geometryParam3) << 16) | 0x1000002);
            cache(program, 6, 0x08000100, 0xff00ff00, 10);
            break;
        }
    } else {
        cache(program, 0, 0, 0xff000000, 8);
        cache(program, 18, 0);
    }
    cache(program, 21, primary->count10 - 1);
    cache(program, 22, ((program->secondary ? secondaryOutputCount : primary->count10) - 1) & 255, 255, 1);
    cache(program, 17, primary->count10 - 1);
    cache(program, 20, primary->countC - 1);
    cache(program, 14, 0xa0000000, 0xff00ff00, 10);
    program->linked = 1;
    program->changedAtLink = program->changed;
    program->changed = 0;
    program->hasSecondary = program->secondary != 0;
    if (program->secondary) program->secondaryStageIndex = program->secondary->index;
    program->primaryStageIndex = program->primary->index;
    if (currentContext()->current == program) {
        Byte* control = reinterpret_cast<Byte*>(dat_003E3154);
        if (program->linkedResource != program->primary->resource) word(control, 0) |= 0x100000;
        word(control, 0) |= 0x1e08000;
        for (Word i = 0; i < 3; ++i) program->primaryDirty[i] = ~0u;
        if (program->secondary)
            for (Word i = 0; i < 3; ++i) program->secondaryDirty[i] = ~0u;
        for (Word i = 0; i < 6; ++i) program->packedDirty[i] = ~0u;
        word(control, 0) |= 0x200013c;
        for (Word i = 0; i < 3; ++i) {
            if (word(control, 0xf8 + i * 4) != word(program, 0xdac + i * 4)) {
                byte(control, 0x104 + i) = 0;
                word(control, 0xf8 + i * 4) = 0;
                word(control, 0) |= 0x400u << i;
                if (i == 0) byte(control, 0x107) = 0;
            }
        }
        program->changedAtLink = 0;
        byte(control, 0x1b) = program->hasSecondary;
    }
    program->linkedResource = program->primary->resource;
}
#endif
