#include <Observed/SharedTransformMatrixStorage.h>
#include <math/seadVector.h>
#include <string.h>

namespace observed_fog_resource {
typedef void* (*Allocate)(void*, unsigned, int);
struct AllocatorDispatch {
    void* unknown00[2];
    Allocate allocate;
};
struct Allocator { const AllocatorDispatch* dispatch; };
struct Parameters {
    unsigned flags;
    unsigned unknown04, unknown08;
    unsigned bufferSize;
    int bufferDisplacement;
};
struct Auxiliary { unsigned words[4]; };
struct Record {
    unsigned type;
    unsigned signature;
    unsigned version;
    int nameDisplacement;
    unsigned word10, word14, flags, word1C;
    unsigned word20, word24, word28, word2C;
    sead::Vector3f scale;
    sead::Vector3f rotation;
    sead::Vector3f translation;
    unsigned char unknown54[0x30];
    transform_blend::Matrix34 matrix;
    unsigned char unknownB4[0x10];
    int parametersDisplacement;
    int auxiliaryDisplacement;
};
static_assert(sizeof(Parameters) == 0x14, "parameter allocation");
static_assert(sizeof(Auxiliary) == 0x10, "auxiliary allocation");
static_assert(offsetof(Record, parametersDisplacement) == 0xc4, "parameter link");
static_assert(offsetof(Record, auxiliaryDisplacement) == 0xc8, "auxiliary link");
static_assert(sizeof(Record) == 0xcc, "allocation and byte-clear extent");
static_assert(offsetof(Record, nameDisplacement) == 0xc, "relative name field");
static_assert(offsetof(Record, matrix) == 0x84, "matrix field");
static_assert(sizeof(transform_blend::Matrix34) == 48, "bounded shared matrix span");
static_assert(offsetof(AllocatorDispatch, allocate) == 8, "proved allocation slot");
}
extern "C" void __rt_memcpy(void*, const void*, unsigned);
extern "C" int fn_0028A998(unsigned*);

extern "C" observed_fog_resource::Record* fn_002A819C(
        observed_fog_resource::Allocator* allocator, const char* name) {
    using namespace observed_fog_resource;
    Record* record = static_cast<Record*>(allocator->dispatch->allocate(allocator, sizeof(Record), 4));
    unsigned char* bytes = reinterpret_cast<unsigned char*>(record);
    for (unsigned i = 0; i < sizeof(Record); ++i) bytes[i] = 0;
    record->type = 0x40000042;
    record->signature = 0x474f4643;
    record->version = 0x06000000;
    record->word10 = 0;
    record->word14 = 0;
    char* copiedName = 0;
    if (name) {
        unsigned length = strlen(name);
        if (length > 256) length = 256;
        copiedName = static_cast<char*>(allocator->dispatch->allocate(allocator, length + 1, 4));
        __rt_memcpy(copiedName, name, length);
        copiedName[length] = 0;
    }
    record->nameDisplacement = copiedName
        ? static_cast<int>(reinterpret_cast<unsigned>(copiedName) -
                           reinterpret_cast<unsigned>(&record->nameDisplacement)) : 0;
    record->word20 = 0;
    record->word24 = 0;
    record->word28 = 0;
    record->word2C = 0;
    record->scale = sead::Vector3f(1.0f, 1.0f, 1.0f);
    record->rotation = sead::Vector3f(0.0f, 0.0f, 0.0f);
    record->translation = sead::Vector3f(0.0f, 0.0f, 0.0f);
    if (!(dat_003F389C & 1) && fn_0028A998(&dat_003F389C)) {
        for (unsigned row = 0; row < 3; ++row)
            for (unsigned column = 0; column < 4; ++column)
                dat_00430C68.m[row][column] = row == column ? 1.0f : 0.0f;
    }
    record->matrix = dat_00430C68;
    record->flags |= 1;
    Parameters* parameters = static_cast<Parameters*>(allocator->dispatch->allocate(allocator, sizeof(Parameters), 4));
    unsigned char* parameterBytes = reinterpret_cast<unsigned char*>(parameters);
    for (unsigned i = 0; i < sizeof(Parameters); ++i) parameterBytes[i] = 0;
    Auxiliary* auxiliary = static_cast<Auxiliary*>(allocator->dispatch->allocate(allocator, sizeof(Auxiliary), 4));
    unsigned char* auxiliaryBytes = reinterpret_cast<unsigned char*>(auxiliary);
    for (unsigned i = 0; i < sizeof(Auxiliary); ++i) auxiliaryBytes[i] = 0;
    record->auxiliaryDisplacement = auxiliary
        ? static_cast<int>(reinterpret_cast<unsigned>(auxiliary) -
                           reinterpret_cast<unsigned>(&record->auxiliaryDisplacement)) : 0;
    record->parametersDisplacement = parameters
        ? static_cast<int>(reinterpret_cast<unsigned>(parameters) -
                           reinterpret_cast<unsigned>(&record->parametersDisplacement)) : 0;
    parameters->flags = 0x80000000;
    unsigned* buffer = static_cast<unsigned*>(allocator->dispatch->allocate(allocator, 0x210, 4));
    if (buffer)
        for (unsigned i = 0; i < 0x210 / sizeof(unsigned); ++i) buffer[i] = 0;
    parameters->bufferSize = 0x210;
    parameters->bufferDisplacement = buffer
        ? static_cast<int>(reinterpret_cast<unsigned>(buffer) -
                           reinterpret_cast<unsigned>(&parameters->bufferDisplacement)) : 0;
    return record;
}
