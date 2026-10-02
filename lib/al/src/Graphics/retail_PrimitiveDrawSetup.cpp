#include <Observed/PrimitiveDrawSetup.h>

// NonMatching. Actor/791 is the configured build carrier, not proof of original module.
#ifdef NON_MATCHING
extern "C" {
void* fn_002913E4(unsigned, void*, int);
void* fn_00220CE4(void*);
void fn_00220BD0(void*, const void*, int, int);
bool fn_00220A6C(void*, effect2ea9e8::Attribute*, const char*);
unsigned* fn_00220828(void*, unsigned*);
void* fn_00293088(void*);
extern void* dat_003E23B8;
void fn_00220F04(void*, void*, void*, unsigned, unsigned);
void* __aeabi_vec_ctor_nocookie_nodtor(void*, void (*)(void*), unsigned, unsigned);
void fn_002E2274(void*);
void fn_002E1BA0(void*, unsigned short*);
void fn_002E1868(void*, unsigned short*);
void _ZN4sead17PrimitiveDrawUtil15setSphereVertexEPNS0_6VertexEPtii(void*, unsigned short*, int, int);
void _ZN4sead17PrimitiveDrawUtil13setDiskVertexEPNS0_6VertexEPti(void*, unsigned short*, int);
void _ZN4sead17PrimitiveDrawUtil17setCylinderVertexEPNS0_6VertexEPti(void*, unsigned short*, int);
void _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(void*, void*, const void*, unsigned, unsigned);
void _ZN4sead19PrimitiveDrawMgrCtr15loadLineVertex_EPNS_4HeapE(void*, void*);
void* _ZnajPN4sead4HeapEi(unsigned, void*, int);
unsigned nngxGetPhysicalAddr(const void*);
void fn_001EFA24(void*);
void fn_001EFA74(void*);
void _ZdlPv(void*);
}
namespace primitive_draw_setup {
inline Word& word(Byte* p, unsigned offset) { return *reinterpret_cast<Word*>(p + offset); }
inline unsigned*& commands(Byte* p, unsigned offset) { return *reinterpret_cast<unsigned**>(p + offset); }
inline unsigned short* indices(Byte* p, unsigned offset) { return *reinterpret_cast<unsigned short**>(p + offset); }
inline effect2ea9e8::Attribute* attribute(Byte* p, unsigned offset) {
    return reinterpret_cast<effect2ea9e8::Attribute*>(p + offset);
}
inline void* currentHeap(void* heap) { return heap ? heap : fn_00293088(dat_003E23B8); }
inline unsigned largestBlock(void* heap) {
    typedef unsigned (*Call)(void*, int);
    return reinterpret_cast<Call>((*reinterpret_cast<void***>(heap))[14])(heap, 4);
}
inline unsigned* allocateBlock(void* heap, unsigned size) {
    typedef unsigned* (*Call)(void*, unsigned, int);
    return reinterpret_cast<Call>((*reinterpret_cast<void***>(heap))[5])(heap, size, 4);
}
inline unsigned* resizeBlock(void* heap, unsigned* p, unsigned size) {
    typedef unsigned* (*Call)(void*, void*, unsigned);
    return reinterpret_cast<Call>((*reinterpret_cast<void***>(heap))[8])(heap, p, size);
}
inline void constructVertices(Vertex* p, unsigned count) {
    __aeabi_vec_ctor_nocookie_nodtor(p, fn_002E2274, sizeof(Vertex), count);
}
}

extern "C" void fn_002E0D2C(void* receiver, void* heap, const void* shaderBinary) {
    using namespace primitive_draw_setup;
    Byte* self = static_cast<Byte*>(receiver);
    void* shader = fn_002913E4(0x128E8, heap, -32);
    if (shader) shader = fn_00220CE4(shader);
    fn_00220BD0(shader, shaderBinary, 0, -1);
    fn_00220A6C(shader, attribute(self, 0x0C), "wvp");
    fn_00220A6C(shader, attribute(self, 0x14), "user");
    fn_00220A6C(shader, attribute(self, 0x1C), "color0");
    fn_00220A6C(shader, attribute(self, 0x24), "color1");
    fn_00220A6C(shader, attribute(self, 0x2C), "uv_src");
    fn_00220A6C(shader, attribute(self, 0x34), "uv_size");
    fn_00220A6C(shader, attribute(self, 0x3C), "Vertex");
    fn_00220A6C(shader, attribute(self, 0x44), "TexCoord0");
    fn_00220A6C(shader, attribute(self, 0x4C), "ColorRate");
    void* allocator = currentHeap(heap);
    commands(self, 4) = allocateBlock(allocator, largestBlock(allocator));
    unsigned* end = fn_00220828(shader, commands(self, 4));
    allocator = currentHeap(heap);
    word(self, 8) = end - commands(self, 4);
    commands(self, 4) = resizeBlock(allocator, commands(self, 4), word(self, 8) * 4);
    fn_00220BD0(shader, shaderBinary, 0, 1);
    fn_00220A6C(shader, attribute(self, 0x5C), "wvp");
    fn_00220A6C(shader, attribute(self, 0x64), "user");
    fn_00220A6C(shader, attribute(self, 0x6C), "color0");
    fn_00220A6C(shader, attribute(self, 0x74), "color1");
    fn_00220A6C(shader, attribute(self, 0x7C), "uv_src");
    fn_00220A6C(shader, attribute(self, 0x84), "uv_size");
    fn_00220A6C(shader, attribute(self, 0x8C), "Vertex");
    fn_00220A6C(shader, attribute(self, 0x94), "TexCoord0");
    fn_00220A6C(shader, attribute(self, 0x9C), "ColorRate");
    fn_00220A6C(shader, attribute(self, 0xA4), "dmp_Line.width");
    allocator = currentHeap(heap);
    commands(self, 0x54) = allocateBlock(allocator, largestBlock(allocator));
    end = fn_00220828(shader, commands(self, 0x54));
    allocator = currentHeap(heap);
    word(self, 0x58) = end - commands(self, 0x54);
    commands(self, 0x54) = resizeBlock(allocator, commands(self, 0x54), word(self, 0x58) * 4);
    fn_00220F04(self, heap, self + 0x32C, 4, 6);
    {
        Vertex vertices[4]; constructVertices(vertices, 4);
        fn_002E1BA0(vertices, indices(self, 0xABC));
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x32C, vertices, 4, 6);
    }
    _ZN4sead19PrimitiveDrawMgrCtr15loadLineVertex_EPNS_4HeapE(self, heap);
    IndexBuffer* outline = reinterpret_cast<IndexBuffer*>(self + 0x38E0);
    outline->data = static_cast<unsigned short*>(_ZnajPN4sead4HeapEi(10, heap, 4));
    outline->data[0] = 0; outline->data[1] = 1; outline->data[2] = 3; outline->data[3] = 2; outline->data[4] = 0;
    outline->physical = nngxGetPhysicalAddr(outline->data); outline->count = 5; outline->format = 0;
    fn_00220F04(self, heap, self + 0x1284, 8, 36);
    {
        Vertex vertices[8]; constructVertices(vertices, 8);
        fn_002E1868(vertices, indices(self, 0x1A14));
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x1284, vertices, 8, 36);
    }
    fn_00220F04(self, heap, self + 0x1A30, 34, 192);
    {
        Vertex vertices[130]; constructVertices(vertices, 130);
        _ZN4sead17PrimitiveDrawUtil15setSphereVertexEPNS0_6VertexEPtii(vertices, indices(self, 0x21C0), 8, 4);
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x1A30, vertices, 34, 192);
    }
    fn_00220F04(self, heap, self + 0x21DC, 130, 768);
    {
        Vertex vertices[130]; constructVertices(vertices, 130);
        _ZN4sead17PrimitiveDrawUtil15setSphereVertexEPNS0_6VertexEPtii(vertices, indices(self, 0x296C), 16, 8);
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x21DC, vertices, 130, 768);
    }
    fn_00220F04(self, heap, self + 0x2988, 17, 48);
    {
        Vertex vertices[33]; constructVertices(vertices, 33);
        _ZN4sead17PrimitiveDrawUtil13setDiskVertexEPNS0_6VertexEPti(vertices, indices(self, 0x3118), 16);
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x2988, vertices, 17, 48);
    }
    fn_00220F04(self, heap, self + 0x3134, 33, 96);
    {
        Vertex vertices[33]; constructVertices(vertices, 33);
        _ZN4sead17PrimitiveDrawUtil13setDiskVertexEPNS0_6VertexEPti(vertices, indices(self, 0x38C4), 32);
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x3134, vertices, 33, 96);
    }
    IndexBuffer* ring16 = reinterpret_cast<IndexBuffer*>(self + 0x38F0);
    ring16->data = static_cast<unsigned short*>(_ZnajPN4sead4HeapEi(34, heap, 4));
    for (int i = 0; i < 16; ++i) ring16->data[i] = i;
    ring16->data[16] = 0;
    ring16->physical = nngxGetPhysicalAddr(ring16->data); ring16->count = 17; ring16->format = 0;
    IndexBuffer* ring32 = reinterpret_cast<IndexBuffer*>(self + 0x3900);
    ring32->data = static_cast<unsigned short*>(_ZnajPN4sead4HeapEi(66, heap, 4));
    for (int i = 0; i < 32; ++i) ring32->data[i] = i;
    ring32->data[32] = 0;
    ring32->physical = nngxGetPhysicalAddr(ring32->data); ring32->count = 33; ring32->format = 0;
    fn_00220F04(self, heap, self + 0x3910, 34, 192);
    {
        Vertex vertices[66]; constructVertices(vertices, 66);
        _ZN4sead17PrimitiveDrawUtil17setCylinderVertexEPNS0_6VertexEPti(vertices, indices(self, 0x40A0), 16);
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x3910, vertices, 34, 192);
    }
    fn_00220F04(self, heap, self + 0x40BC, 66, 384);
    {
        Vertex vertices[66]; constructVertices(vertices, 66);
        _ZN4sead17PrimitiveDrawUtil17setCylinderVertexEPNS0_6VertexEPti(vertices, indices(self, 0x484C), 32);
        _ZN4sead19PrimitiveDrawMgrCtr16copyAndSetupVtx_EPNS0_5ShapeEPKNS_17PrimitiveDrawUtil6VertexEjj(self, self + 0x40BC, vertices, 66, 384);
    }
    self[0xC8] = 5; self[0x160] = 3; self[0x15F] = 1; self[0x170] = 0;
    fn_001EFA24(self + 0x1FC);
    fn_001EFA74(self + 0x290);
    _ZdlPv(shader);
}
#endif
