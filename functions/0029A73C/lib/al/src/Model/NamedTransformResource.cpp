#include <Observed/SharedTransformMatrixStorage.h>
#include <math/seadVector.h>
#include <string.h>

#ifdef NON_MATCHING

namespace observed_named_transform_resource {
typedef void* (*Allocate)(void*, unsigned, int);
struct AllocatorDispatch {
    void* unknown00[2];
    Allocate allocate;
};
struct Allocator { const AllocatorDispatch* dispatch; };
inline void fillBytes(void* memory, unsigned size, unsigned char value) {
    unsigned char* cursor = static_cast<unsigned char*>(memory);
    if (size) {
        do {
            --size;
            *cursor++ = value;
        } while (size);
    }
}
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
inline void setMatrix(transform_blend::Matrix34* matrix,
                      float m00, float m01, float m02, float m03,
                      float m10, float m11, float m12, float m13,
                      float m20, float m21, float m22, float m23) {
    matrix->m[0][0] = m00; matrix->m[0][1] = m01;
    matrix->m[0][2] = m02; matrix->m[0][3] = m03;
    matrix->m[1][0] = m10; matrix->m[1][1] = m11;
    matrix->m[1][2] = m12; matrix->m[1][3] = m13;
    matrix->m[2][0] = m20; matrix->m[2][1] = m21;
    matrix->m[2][2] = m22; matrix->m[2][3] = m23;
}
static_assert(sizeof(Record) == 0xd0, "allocation and byte-clear extent");
static_assert(offsetof(Record, nameDisplacement) == 0xc, "relative name field");
static_assert(offsetof(Record, matrix) == 0x84, "matrix field");
static_assert(sizeof(transform_blend::Matrix34) == 48, "bounded shared matrix span");
static_assert(offsetof(AllocatorDispatch, allocate) == 8, "proved allocation slot");
}
extern "C" void __rt_memcpy(void*, const void*, unsigned);
extern "C" int fn_0028A998(unsigned*);

namespace observed_named_transform_resource {
inline char* copyName(Allocator* allocator, const char* name) {
    if (!name) return 0;
    unsigned originalLength = strlen(name);
    unsigned length = originalLength < 256 ? originalLength : 256;
    char* copied = static_cast<char*>(allocator->dispatch->allocate(allocator, length + 1, 4));
    __rt_memcpy(copied, name, length);
    copied[length] = 0;
    return copied;
}
inline void setRelativeName(int* displacement, const char* name) {
    if (name)
        *displacement = static_cast<int>(reinterpret_cast<unsigned>(name) -
                                        reinterpret_cast<unsigned>(displacement));
    else
        *displacement = 0;
}
}

extern "C" observed_named_transform_resource::Record* fn_0029A73C(
        observed_named_transform_resource::Allocator* allocator, const char* name) {
    using namespace observed_named_transform_resource;
    Record* record = static_cast<Record*>(allocator->dispatch->allocate(allocator, sizeof(Record), 4));
    fillBytes(record, sizeof(Record), 0);
    record->type = 0x40000422;
    record->signature = 0x4e465443;
    record->version = 0x07010000;
    record->word10 = 0;
    record->word14 = 0;
    setRelativeName(&record->nameDisplacement, copyName(allocator, name));
    record->word20 = 0;
    record->word24 = 0;
    record->word28 = 0;
    record->word2C = 0;
    record->scale = sead::Vector3f(1.0f, 1.0f, 1.0f);
    record->rotation = sead::Vector3f(0.0f, 0.0f, 0.0f);
    record->translation = sead::Vector3f(0.0f, 0.0f, 0.0f);
    if (!(dat_003F389C & 1) && fn_0028A998(&dat_003F389C)) {
        setMatrix(&dat_00430C68, 1.0f, 0.0f, 0.0f, 0.0f,
                  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    }
    record->matrix = dat_00430C68;
    record->flags |= 1;
    return record;
}

#endif
