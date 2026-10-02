#include <retail/CflMeshResource.h>

#ifdef NON_MATCHING
// NonMatching: clean-room whole-root proposal for 0x00280798..0x002812F0.
// Imports retain existing map-row addresses; inline helpers have no retail identity.
using namespace retail_cfl_mesh;
extern "C" {
int fn_00283D1C(void*, int category, int index);
void* fn_002848F4(unsigned bytes, int alignment);
void fn_002847BC(void*);
typedef void* (*Allocate)(unsigned, unsigned, void*, unsigned);
void nngxGetAllocator(Allocate*, void*);
void __rt_memcpy(void*, const void*, unsigned);
void fn_00296048(void*, unsigned);
void fn_00281DDC(const void*, void*, unsigned);
void nngxSplitDrawCmdlist();
int glGetError();
void fn_00284894();
void fn_00284868();
void fn_002847E4();
extern const Vec3 dat_003A8334;
extern const ComponentCounts dat_003A8340;
extern AxisMapping dat_003EF07C;
}
static_assert_(sizeof(Mesh) == 0x74);
static_assert_(offsetof(Resource, anchors) == 0x4e4);
static_assert_(offsetof(Resource, transforms) == 0x508);
static_assert_(offsetof(Resource, memoryKind) == 0x67c);
static_assert_(offsetof(Resource, flags) == 0x68c);

static inline void copyFloatBytes(float* out, const unsigned char* input, int count)
{
    for (int i = 0; i < count; ++i) {
        const unsigned char* element = input + 4 * i;
        union { float f; unsigned char bytes[4]; } value;
        value.bytes[0] = element[0]; value.bytes[1] = element[1];
        value.bytes[2] = element[2]; value.bytes[3] = element[3];
        out[i] = value.f;
    }
}
static inline void swizzle(short* value)
{
    short x = dat_003EF07C.negateX ? -value[0] : value[0];
    short y = dat_003EF07C.negateY ? -value[1] : value[1];
    short z = dat_003EF07C.negateZ ? -value[2] : value[2];
    value[dat_003EF07C.outputX] = x;
    value[dat_003EF07C.outputY] = y;
    value[dat_003EF07C.outputZ] = z;
}
static inline void syncDraw()
{
    nngxSplitDrawCmdlist(); glGetError();
    fn_00284894(); fn_00284868(); fn_002847E4(); glGetError();
}
static inline void initBuffer(Buffer* buffer, void* source, unsigned bytes, unsigned kind)
{
    buffer->owner = buffer;
    buffer->owned = 1;
    buffer->memoryKind = kind;
    Allocate allocate;
    nngxGetAllocator(&allocate, 0);
    buffer->data = allocate(kind, 0x102, buffer, bytes);
    if (source) {
        if (kind == 0x10000) {
            __rt_memcpy(buffer->data, source, bytes);
        } else {
            fn_00296048(source, bytes);
            fn_00281DDC(source, buffer->data, bytes);
        }
    }
    if (kind == 0x10000) fn_00296048(buffer->data, bytes);
}
extern "C" void fn_00280798(Resource* resource, int category, int index,
    const Vec3* translation, float scaleX, float scaleY,
    int mirrorX, const unsigned char* occupancy)
{
    Mesh* mesh = &resource->meshes[category];
    int size = fn_00283D1C(0, category, index);
    unsigned char* loaded = 0;
    if (size > 0) {
        loaded = static_cast<unsigned char*>(fn_002848F4(size, 32));
        fn_00283D1C(loaded, category, index);
    }
    if (!loaded) { mesh->segmentCount = 0; return; }
    unsigned char* data = loaded;
    if (category == 2) {
        copyFloatBytes(&resource->anchors[0].x, data, 3); data += 12;
        copyFloatBytes(&resource->anchors[1].x, data, 3); data += 12;
        copyFloatBytes(&resource->anchors[2].x, data, 3); data += 12;
        Vec3* transforms = resource->transforms;
        transforms[0] = resource->anchors[0];
        transforms[2] = dat_003A8334;
        transforms[4] = dat_003A8334;
        transforms[6] = dat_003A8334;
        transforms[1] = dat_003A8334;
        transforms[3] = dat_003A8334;
        transforms[5] = dat_003A8334;
    }
    if (category == 5) {
        copyFloatBytes(&resource->transforms[1].x, data, 18); data += 72;
    }
    unsigned short* header = reinterpret_cast<unsigned short*>(data);
    int positionCount = header[0], normalCount = header[1], texCoordCount = header[2];
    int wideIndices = (header[3] & 0x8000) >> 15;
    mesh->segmentCount = header[3] & ~0x8000;
    mesh->indexType = wideIndices ? 0x1403 : 0x1401;
    data += 8;
    if (mesh->segmentCount > 0) {
        float scaleZ = (scaleX + scaleY) * 0.5f;
        if ((category == 8 || category == 7) && (resource->flags & 8) && scaleZ > 1.1f)
            scaleZ = 1.1f;
        float offsetX = translation ? translation->x * 256.0f : 0.0f;
        float offsetY = translation ? translation->y * 256.0f : 0.0f;
        float offsetZ = translation ? translation->z * 256.0f : 0.0f;
        int counts[3] = { positionCount, normalCount, texCoordCount };
        ComponentCounts componentCounts = dat_003A8340;
        int* components = componentCounts.value;
        unsigned char* streams[3] = {0, 0, 0};
        unsigned offsets[3] = {0, 0, 0};
        mesh->componentType = 0x1402;
        mesh->stride = 0;
        for (int i = 0; i < 3; ++i) {
            if (counts[i] >= 2) {
                offsets[i] = mesh->stride;
                streams[i] = data;
                mesh->stride += components[i] * 2;
            }
        }
        int maxCount = positionCount > normalCount ? positionCount : normalCount;
        if (texCoordCount > maxCount) maxCount = texCoordCount;
        unsigned char* elements = data + maxCount * mesh->stride;
        for (int i = 0; i < 3; ++i) {
            if (counts[i] == 1) { streams[i] = elements; elements += components[i] * 2; }
        }
        short* values = reinterpret_cast<short*>(streams[0]) + offsets[0] / 2;
        for (int i = 0; i < positionCount; ++i) {
            if (mirrorX) values[0] = -values[0];
            values[0] = static_cast<short>(values[0] * scaleX + offsetX);
            values[1] = static_cast<short>(values[1] * scaleY + offsetY);
            values[2] = static_cast<short>(values[2] * scaleZ + offsetZ);
            swizzle(values);
            values += mesh->stride / 2;
        }
        values = reinterpret_cast<short*>(streams[1]) + offsets[1] / 2;
        for (int i = 0; i < normalCount; ++i) {
            if (mirrorX) values[0] = -values[0];
            swizzle(values);
            values += mesh->stride / 2;
        }
        for (int i = 0; i < 3; ++i) {
            if (mesh->componentType == 0x1406) offsets[i] *= 2;
            Attribute* attribute = &mesh->attributes[i];
            int count = components[i];
            attribute->components = count;
            if (counts[i] >= 2) attribute->offset = offsets[i];
            else {
                attribute->offset = -1;
                attribute->constant[0] = attribute->constant[1] = attribute->constant[2] = 0.0f;
                if (counts[i] == 1) {
                    const unsigned char* bytes = streams[i];
                    for (int j = 0; j < count; ++j) {
                        union { short s; unsigned char bytes[2]; } value;
                        value.bytes[0] = bytes[0]; value.bytes[1] = bytes[1];
                        attribute->constant[j] = value.s;
                        bytes += 2;
                    }
                }
            }
        }
        mesh->vertices.owner = 0;
        if (mesh->stride != 0) {
            if (mesh->componentType == 0x1406) {
                mesh->stride *= 2;
                float* converted = static_cast<float*>(fn_002848F4(mesh->stride * maxCount, 4));
                short* source = reinterpret_cast<short*>(data);
                int valuesCount = mesh->stride * maxCount / 4;
                for (int i = 0; i < valuesCount; ++i) converted[i] = source[i];
                data = reinterpret_cast<unsigned char*>(converted);
            }
            initBuffer(&mesh->vertices, data, mesh->stride * maxCount, resource->memoryKind);
            if (resource->memoryKind != 0x10000) syncDraw();
            if (mesh->componentType == 0x1406) fn_002847BC(data);
        }
        int total = 0;
        for (int i = 0; i < mesh->segmentCount; ++i) {
            unsigned short* source = reinterpret_cast<unsigned short*>(elements);
            mesh->segment[i].mode = source[0];
            unsigned short count = source[1];
            mesh->segment[i].count = count;
            mesh->segment[i].offset = total;
            total += count;
            elements += 4;
        }
        if (category == 6 && mesh->segmentCount == 1 && occupancy && !(resource->flags & 0x10000000)) {
            int indexBytes = wideIndices ? 2 : 1;
            unsigned count = mesh->segment[0].count;
            unsigned char* bounds = elements + count * indexBytes;
            unsigned char* read = elements;
            unsigned char* write = elements;
            int triangleCount = count / 3;
            total = 0;
            for (int triangle = 0; triangle < triangleCount; ++triangle) {
                unsigned char horizontal = bounds[triangle * 2];
                unsigned char vertical = bounds[triangle * 2 + 1];
                for (int y = vertical >> 4; y <= (vertical & 15); ++y) {
                    for (int x = horizontal >> 4; x <= (horizontal & 15); ++x) {
                        if (occupancy[y * 16 + x]) goto keepTriangle;
                    }
                }
                goto nextTriangle;
            keepTriangle:
                if (write != read) __rt_memcpy(write, read, indexBytes * 3);
                total += 3;
                write += indexBytes * 3;
            nextTriangle:
                read += indexBytes * 3;
            }
            if (!total) total = 3;
            mesh->segment[0].count = total;
        }
        initBuffer(&mesh->indices, elements, total * (wideIndices ? 2 : 1), resource->memoryKind);
        if (resource->memoryKind != 0x10000) syncDraw();
    }
    fn_002847BC(loaded);
}
#endif
