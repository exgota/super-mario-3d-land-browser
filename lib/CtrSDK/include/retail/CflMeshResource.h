#ifndef RETAIL_CFL_MESH_RESOURCE_H
#define RETAIL_CFL_MESH_RESOURCE_H

// Field views recovered from EU 0x00280798 and its caller 0x00115B1C.
// Descriptive names; no original SDK class or internal API name is asserted.
namespace retail_cfl_mesh {
struct Vec3 { float x, y, z; };
struct Buffer {
    void* owner;
    void* data;
    unsigned memoryKind;
    int owned;
};
struct Attribute { int components; int offset; float constant[3]; };
struct Segment { unsigned short mode, count, offset, unused; };
struct Mesh {
    unsigned componentType;
    Buffer vertices;
    unsigned stride;
    Attribute attributes[3];
    Buffer indices;
    unsigned indexType;
    int segmentCount;
    Segment segment[1];
};
struct Resource {
    Mesh meshes[9];
    unsigned char textureFields[0xd0];
    Vec3 anchors[3];
    Vec3 transforms[7];
    unsigned char characterInfo[0x120];
    unsigned memoryKind;
    unsigned char unknown680[12];
    unsigned flags;
};
struct ComponentCounts { int value[3]; };
struct AxisMapping {
    unsigned char negateY, negateZ, negateX;
    unsigned char outputY, outputZ, outputX;
    unsigned char padding[2];
};
}
extern "C" void fn_00280798(retail_cfl_mesh::Resource*, int category, int index,
    const retail_cfl_mesh::Vec3* translation, float scaleX, float scaleY,
    int mirrorX, const unsigned char* occupancy);
#endif
