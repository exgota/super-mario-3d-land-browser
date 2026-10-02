// Retail particle binding factory 002A451C..002A5008.
// NonMatching clean-room reconstruction from the owner's EU executable.
// Neutral root/object names: original class and compiler ownership are unproven.
#ifdef NON_MATCHING

#include <retail/ParticleShape.h>
#include <retail/ResourceParticleFamily.h>

extern "C" void __rt_memclr(void*, unsigned);
extern "C" void* nnnstdMemCpy(void*, const void*, unsigned);
extern "C" const unsigned dat_003D8384[5];
extern "C" const float dat_003BA150[4];
extern "C" void fn_002A4434(void*);

namespace particle2a451c {
struct Allocator;
struct AllocatorDispatch {
    unsigned unknown[2];
    void* (*allocate)(Allocator*, unsigned, int);
    void (*release)(Allocator*, void*);
};
struct Allocator {
    AllocatorDispatch* dispatch;
    void* allocate(unsigned size) { return dispatch->allocate(this, size, 32); }
    void release(void* p) { dispatch->release(this, p); }
};
struct Attribute {
    unsigned type;
    int index;
    unsigned unknown08[2];
    int valuesOffset;
    float* values() { return valuesOffset ? (float*)((unsigned char*)&valuesOffset + valuesOffset) : 0; }
};
struct Resource {
    int count;
    int attributeCount;
    int attributesOffset;
    int* attributes() const { return attributesOffset ? (int*)((unsigned char*)&attributesOffset + attributesOffset) : 0; }
};
struct Descriptor {
    int index;
    unsigned char stream;
    unsigned char unknown05[3];
    unsigned char* data;
};
struct Pair { unsigned char* first; unsigned char* second; };
struct Bindings {
    const unsigned* dispatch;
    Allocator* allocator;
    Descriptor descriptors[16];
    int capacity;
    int count;
    unsigned char swap;
    unsigned char enabled[16];
    unsigned char unknownE1[3];
    Pair buffers[16];
    unsigned strides[16];
    unsigned short first, last;
    const Resource* resource;
    nw::gfx::ParticleShape* shape;
    void* owner;
    Allocator* streamAllocator;
    unsigned char* streamAllocation;
    unsigned unknown1BC;

    void initialize(Allocator* heap, const Resource* res, Allocator* streamHeap, unsigned char* stream) {
        allocator = heap;
        dispatch = dat_003D8384;
        swap = 0;
        first = 0;
        last = 0;
        resource = res;
        shape = 0;
        owner = 0;
        streamAllocator = streamHeap;
        streamAllocation = stream;
        unknown1BC = 0;
        for (int i = 0; i < 16; ++i) {
            descriptors[i].index = -1;
            descriptors[i].data = 0;
            strides[i] = 0;
            enabled[i] = 0;
            buffers[i].first = 0;
            buffers[i].second = 0;
        }
    }
    unsigned char* addStorage(int index, unsigned size, unsigned char** cursor) {
        unsigned char* p = (unsigned char*)(((unsigned)*cursor + 7) & ~7U);
        *cursor = p + size;
        __rt_memclr(p, size);
        descriptors[index].index = index;
        descriptors[index].stream = 1;
        descriptors[index].data = p;
        return p;
    }
};
static_assert_(sizeof(Bindings) == 0x1c0);
static_assert_(sizeof(nw::gfx::ParticleShape) == 0x200);

inline Attribute* resolve(int* p) {
    return *p ? (Attribute*)((unsigned char*)p + *p) : 0;
}
inline unsigned align8(unsigned x) { return (x + 7) & ~7U; }
inline unsigned align4(unsigned x) { return (x + 3) & ~3U; }
}

extern "C" void* fn_002A451C(void* owner, const void* resource, void* allocator, void* streamAllocator, void* shape) {
    using namespace particle2a451c;
    using nw::gfx::ParticleShape;
    const Resource* res = (const Resource*)resource;
    Allocator* heap = (Allocator*)allocator;
    Allocator* streamHeap = (Allocator*)streamAllocator;
    ParticleShape* particle = (ParticleShape*)shape;
    unsigned char present[16] = {0};
    int count = res->count;
    unsigned indexSize = 0x18 + count * 2;
    unsigned wordSize = 0x28 + count * 4;
    int size = align8(align4(0x1c0 + indexSize)) + wordSize;
    int streamSize = 0;
    present[13] = present[14] = present[15] = 1;
    int* p = res->attributes();
    int* end = p + res->attributeCount;
    for (; p != end; ++p) {
        int index = resolve(p)->index;
        if (resolve(p)->type == 0x40000000) {
            switch (index) {
            case 10: case 11: size = align4(size) + 4; break;
            case 4: case 7: size = ParticleShape::AddVertexParamSize(0x1406, 1, size); break;
            case 0: case 1: case 2: case 3: case 8: size = ParticleShape::AddVertexParamSize(0x1406, 3, size); break;
            case 5: case 6: size = ParticleShape::AddVertexParamSize(0x1406, 2, size); break;
            }
        } else {
            switch (index) {
            case 10: case 11: size = align8(size) + wordSize; break;
            case 12: size = align8(size) + 0x68 + count * 12; break;
            case 4: case 7: streamSize = ParticleShape::AddVertexStreamSize(0x1406, 1, count, streamSize); break;
            case 0: case 1: case 2: case 3: case 8: streamSize = ParticleShape::AddVertexStreamSize(0x1406, 3, count, streamSize); break;
            case 5: case 6: streamSize = ParticleShape::AddVertexStreamSize(0x1406, 2, count, streamSize); break;
            }
        }
        present[index] = 1;
    }
    for (int i = 0; i < 16; ++i) {
        if (!present[i]) {
            switch (i) {
            case 4: case 7: size = ParticleShape::AddVertexParamSize(0x1406, 1, size); break;
            case 0: case 1: case 2: case 3: case 8: size = ParticleShape::AddVertexParamSize(0x1406, 3, size); break;
            case 5: case 6: size = ParticleShape::AddVertexParamSize(0x1406, 2, size); break;
            }
            present[i] = 0;
        }
    }
    unsigned char* stream = 0;
    if (streamSize > 0) {
        stream = (unsigned char*)streamHeap->allocate(streamSize);
        if (!stream) return 0;
    }
    unsigned char* storage = (unsigned char*)heap->allocate(size);
    if (!storage) {
        if (stream) streamHeap->release(stream);
        return 0;
    }
    Bindings* result = (Bindings*)storage;
    result->initialize(heap, res, streamHeap, stream);
    storage = (unsigned char*)align4((unsigned)storage + 0x1c0);
    result->capacity = count;
    result->shape = particle;
    result->enabled[13] = 1;
    result->buffers[13].first = particle->buffers[particle->swap != 0];
    result->strides[13] = 2;
    result->buffers[13].second = particle->buffers[particle->swap == 0];
    present[13] = 1;
    result->addStorage(14, indexSize, &storage);
    result->enabled[14] = 1;
    result->buffers[14].first = result->descriptors[14].data;
    result->strides[14] = 2;
    result->buffers[14].second = result->descriptors[14].data;
    present[14] = 1;
    result->addStorage(15, wordSize, &storage);
    result->enabled[15] = 1;
    result->strides[15] = 2;
    result->buffers[15].first = result->descriptors[15].data;
    result->buffers[15].second = result->descriptors[15].data;
    present[15] = 1;
    p = res->attributes();
    end = p + res->attributeCount;
    for (; p != end; ++p) {
        int index = resolve(p)->index;
        if (resolve(p)->type == 0x40000000) {
            Attribute* attribute = resolve(p);
            unsigned char* value = 0;
            switch (index) {
            case 10: case 11:
                value = (unsigned char*)align4((unsigned)storage);
                storage = value + 4;
                nnnstdMemCpy(value, attribute->values(), 4);
                result->descriptors[index].index = index;
                result->descriptors[index].stream = 0;
                result->descriptors[index].data = value;
                *(float*)value = *attribute->values();
                break;
            case 4: case 7: value = particle->AddVertexParam(index, 0x1406, 1, attribute->values(), &storage)->buffers[0]; break;
            case 0: case 1: case 2: case 3: case 8: value = particle->AddVertexParam(index, 0x1406, 3, attribute->values(), &storage)->buffers[0]; break;
            case 5: case 6: value = particle->AddVertexParam(index, 0x1406, 2, attribute->values(), &storage)->buffers[0]; break;
            }
            present[index] = 1;
            result->enabled[index] = 0;
            result->buffers[index].first = value;
            result->buffers[index].second = value;
            result->strides[index] = 0;
        } else {
            switch (index) {
            case 10: case 11:
                result->addStorage(index, wordSize, &storage);
                result->buffers[index].first = result->descriptors[index].data;
                result->buffers[index].second = result->descriptors[index].data;
                result->strides[index] = 4;
                break;
            case 12:
                result->addStorage(index, 0x68 + count * 12, &storage);
                result->buffers[12].first = result->descriptors[12].data;
                result->strides[12] = 12;
                result->buffers[12].second = result->descriptors[12].data;
                break;
            case 4: case 7: {
                nw::gfx::ParticleVertexAttribute* a = particle->AddVertexStream(index, 0x1406, 1, count, &stream);
                result->buffers[index].first = a->buffers[0]; result->buffers[index].second = a->buffers[1]; result->strides[index] = 4; break;
            }
            case 0: case 1: case 2: case 3: case 8: {
                nw::gfx::ParticleVertexAttribute* a = particle->AddVertexStream(index, 0x1406, 3, count, &stream);
                result->buffers[index].first = a->buffers[0]; result->buffers[index].second = a->buffers[1]; result->strides[index] = 12; break;
            }
            case 5: case 6: {
                nw::gfx::ParticleVertexAttribute* a = particle->AddVertexStream(index, 0x1406, 2, count, &stream);
                result->buffers[index].first = a->buffers[0]; result->buffers[index].second = a->buffers[1]; result->strides[index] = 8; break;
            }
            }
            result->enabled[index] = 1;
            present[index] = 1;
        }
    }
    for (int i = 0; i < 16; ++i) {
        if (!present[i]) {
            float zeros[4] = {0, 0, 0, 0};
            float ones[4] = {dat_003BA150[0], dat_003BA150[1], dat_003BA150[2], dat_003BA150[3]};
            unsigned char* value = 0;
            switch (i) {
            case 4: value = particle->AddVertexParam(i, 0x1406, 1, ones, &storage)->buffers[0]; break;
            case 7: value = particle->AddVertexParam(i, 0x1406, 1, zeros, &storage)->buffers[0]; break;
            case 0: case 2: value = particle->AddVertexParam(i, 0x1406, 3, zeros, &storage)->buffers[0]; break;
            case 1: case 3: case 8: value = particle->AddVertexParam(i, 0x1406, 3, ones, &storage)->buffers[0]; break;
            case 5: value = particle->AddVertexParam(i, 0x1406, 2, zeros, &storage)->buffers[0]; break;
            case 6: value = particle->AddVertexParam(i, 0x1406, 2, ones, &storage)->buffers[0]; break;
            }
            present[i] = 1;
            result->enabled[i] = 0;
            result->buffers[i].first = value;
            result->buffers[i].second = value;
            result->strides[i] = 0;
        }
    }
    fn_002A4434(result);
    if (owner) {
        void** first = (void**)((unsigned char*)owner + 0x4c);
        if (!*first) *first = result;
        result->owner = owner;
    }
    return result;
}
#endif
