#include <Observed/SharedTransformMatrixStorage.h>
#include <math/seadVector.h>
#include <string.h>

namespace observed_named_transform_resource {
typedef void* (*Allocate)(void*, unsigned, int);
struct AllocatorDispatch {
    void* unknown00[2];
    Allocate allocate;
};
struct Allocator { const AllocatorDispatch* dispatch; };
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
    unsigned char unknownB4[0x1c];
};
static_assert(sizeof(Record) == 0xd0, "allocation and byte-clear extent");
static_assert(offsetof(Record, nameDisplacement) == 0xc, "relative name field");
static_assert(offsetof(Record, matrix) == 0x84, "matrix field");
static_assert(sizeof(transform_blend::Matrix34) == 48, "bounded shared matrix span");
static_assert(offsetof(AllocatorDispatch, allocate) == 8, "proved allocation slot");
}
extern "C" void __rt_memcpy(void*, const void*, unsigned);
extern "C" int fn_0028A998(unsigned*);

extern "C" observed_named_transform_resource::Record* fn_0029A73C(
        observed_named_transform_resource::Allocator* allocator, const char* name) {
    using namespace observed_named_transform_resource;
    Record* record = static_cast<Record*>(allocator->dispatch->allocate(allocator, sizeof(Record), 4));
    unsigned char* bytes = reinterpret_cast<unsigned char*>(record);
    unsigned remaining = sizeof(Record);
    do {
        --remaining;
        *bytes++ = 0;
    } while (remaining);
    record->type = 0x40000422;
    record->signature = 0x4e465443;
    record->version = 0x07010000;
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
    return record;
}
