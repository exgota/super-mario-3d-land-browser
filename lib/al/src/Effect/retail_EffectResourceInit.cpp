// Complete retail root 002EA9E8..002EB290, reconstructed from the EU dump.
// NonMatching. These private names describe observed storage, not original ABI names.
// The caller allocates 0x6077C bytes; the unused regions are not recovered layouts.
// Actor/791 is a provisional build carrier, not a proven original compiler identity.
#ifdef NON_MATCHING
namespace effect2ea9e8 {
typedef unsigned char Byte;
typedef unsigned Word;
struct Attribute { Byte stage, kind, begin, end; const char* name; };
struct LoadArray { Word count; Byte flags[12]; Word offsets[12]; int formats[12]; };
struct Constant { Byte enabled; Byte reserved[3]; float values[4]; };
struct Vertex { Word count; Word commands[84]; Byte enabled[12]; LoadArray arrays[12]; Constant constants[12]; };
struct Vec3 { float x, y, z; };
struct Geometry { Vec3 position[32]; Vec3 texture[32]; Byte indices[48]; };
struct Settings { Byte unknown[8]; Byte memoryKind; };
typedef void* (*Allocate)(unsigned, unsigned, unsigned, unsigned);
typedef void (*Deallocate)(void*);
static_assert_(sizeof(Vertex) == 0x790);
static_assert_(sizeof(Geometry) == 0x330);
}
extern "C" {
void* fn_002F0110(void*);
void* __aeabi_vec_ctor_nocookie_nodtor(void*, void (*)(void*), unsigned, unsigned);
void _ZN2nn2gr3CTR6Vertex9LoadArrayC2Ev(void*);
void _ZN2nn2gr3CTR6Vertex9AttrConstC2Ev(void*);
void fn_0039AAE8(void*);
void fn_0028D1F0(void*, unsigned);
void fn_0028D18C(void*);
void* fn_00220CE4(void*);
void* _ZN4sead7HeapMgr15setCurrentHeap_EPNS_4HeapE(void*, void*);
extern void* dat_003E23B8;
extern unsigned dat_003EF8C4;
extern const unsigned char dat_003AC800[];
extern const unsigned char dat_003ACC00[];
extern const unsigned char dat_003ACE80[];
void fn_00220BD0(void*, const void*, int, int);
bool fn_00220A6C(void*, effect2ea9e8::Attribute*, const char*);
void fn_002EFF28(void*, void*, const void*, int, bool);
void* fn_002913E4(unsigned, void*, int);
void fn_00296048(void*, unsigned);
void nngxGetAllocator(effect2ea9e8::Allocate*, effect2ea9e8::Deallocate*);
void fn_00281DDC(void*, void*, unsigned);
unsigned nngxGetPhysicalAddr(const void*);
void fn_00220DC0(void*, effect2ea9e8::Attribute*, unsigned, int);
unsigned* fn_001EEAD0(unsigned*);
void fn_0028E758(void*);
void fn_001EFA74(void*);
unsigned* fn_002204FC(void*, unsigned*);
unsigned* fn_00220828(void*, unsigned*);
unsigned* fn_0022151C(void*, unsigned*);
void __aeabi_memcpy4(void*, const void*, unsigned);
void fn_0028D3BC(void*);
unsigned* fn_0028A5EC(void*, unsigned*, bool);
unsigned* fn_00223CA4(void*, unsigned*, bool);
unsigned* fn_00337948(void*, unsigned*, bool);
unsigned* fn_003378CC(void*, unsigned*, bool);
unsigned* fn_0028A4A0(void*, unsigned*, bool);
void* fn_0028E280(unsigned);
}
namespace effect2ea9e8 {
inline Word& word(Byte* p, unsigned off) { return *reinterpret_cast<Word*>(p + off); }
inline Attribute* attribute(Byte* p, unsigned off) { return reinterpret_cast<Attribute*>(p + off); }
inline void initAttribute(Attribute* p, unsigned kind) {
    p->stage = 0; p->kind = kind; p->begin = 255; p->end = 255; p->name = 0;
}
inline void initVertex(Vertex* p) {
    __aeabi_vec_ctor_nocookie_nodtor(p->arrays, _ZN2nn2gr3CTR6Vertex9LoadArrayC2Ev, sizeof(LoadArray), 12);
    __aeabi_vec_ctor_nocookie_nodtor(p->constants, _ZN2nn2gr3CTR6Vertex9AttrConstC2Ev, sizeof(Constant), 12);
    p->count = 0;
    fn_0028D1F0(p->commands, sizeof(p->commands));
    for (unsigned i = 0; i < 12; ++i) {
        p->enabled[i] = 0;
        fn_0028D18C(&p->arrays[i]);
        p->constants[i].enabled = 0;
        p->constants[i].values[0] = 0.0f;
        p->constants[i].values[1] = 0.0f;
        p->constants[i].values[2] = 0.0f;
        p->constants[i].values[3] = 0.0f;
    }
}
inline void initIndices(Byte* p) { word(p, 0) = 0; word(p, 4) = 0; p[8] = 0; }
}
extern "C" void* fn_002EA9E8(void* storage, void* heap, int, void* owner,
                              const effect2ea9e8::Settings* settings) {
    using namespace effect2ea9e8;
    Byte* p = static_cast<Byte*>(fn_002F0110(storage));
    initVertex(reinterpret_cast<Vertex*>(p + 0x39AAC));
    initIndices(p + 0x3A23C);
    fn_00220CE4(p + 0x3A24C);
    initAttribute(attribute(p, 0x4CB34), 2);
    initAttribute(attribute(p, 0x4CB3C), 2);
    initAttribute(attribute(p, 0x4CB48), 1);
    initAttribute(attribute(p, 0x4CB50), 1);
    initAttribute(attribute(p, 0x4CB58), 1);
    fn_00220CE4(p + 0x4CB60);
    initVertex(reinterpret_cast<Vertex*>(p + 0x5F464));
    initIndices(p + 0x5FBF4);
    word(p, 0x4CB44) = reinterpret_cast<Word>(owner);
    void* oldHeap = reinterpret_cast<void*>(1);
    if (heap) oldHeap = _ZN4sead7HeapMgr15setCurrentHeap_EPNS_4HeapE(dat_003E23B8, heap);
    word(p, 0x39AA8) = reinterpret_cast<Word>(heap);
    dat_003EF8C4 = 5;
    fn_00220BD0(p + 0x3A24C, dat_003AC800, 0, -1);
    fn_00220A6C(p + 0x3A24C, attribute(p, 0x4CB34), "proj");
    fn_00220A6C(p + 0x3A24C, attribute(p, 0x4CB3C), "view");
    fn_00220BD0(p + 0x4CB60, dat_003ACC00, 0, -1);
    fn_00220A6C(p + 0x4CB60, attribute(p, 0x4CB48), "Position");
    fn_00220A6C(p + 0x4CB60, attribute(p, 0x4CB50), "TexCoord0");
    fn_00220A6C(p + 0x4CB60, attribute(p, 0x4CB58), "Dir");
    word(p, 0x5F44C) = 0; word(p, 0x5F448) = 0;
    word(p, 0x5F458) = 0; word(p, 0x5F45C) = 0;
    word(p, 0x5F460) = 0; word(p, 0x5F454) = 0;
    fn_002EFF28(p, p, dat_003ACE80, 0, true);
    Attribute input;
    initAttribute(&input, 1);
    Geometry* geometry = static_cast<Geometry*>(fn_002913E4(sizeof(Geometry), heap, 128));
    if (geometry) {
        __aeabi_vec_ctor_nocookie_nodtor(geometry->position, fn_0039AAE8, sizeof(Vec3), 32);
        __aeabi_vec_ctor_nocookie_nodtor(geometry->texture, fn_0039AAE8, sizeof(Vec3), 32);
    }
    for (int i = 0; i < 8; ++i) {
        Vec3* pos = geometry->position + 4 * i;
        pos[0].x = -0.5f; pos[0].y = 0.5f; pos[0].z = 0.0f;
        pos[1].x = 0.5f; pos[1].y = 0.5f; pos[1].z = 0.0f;
        pos[2].x = -0.5f; pos[2].y = -0.5f; pos[2].z = 0.0f;
        pos[3].x = 0.5f; pos[3].y = -0.5f; pos[3].z = 0.0f;
        Vec3* tex = geometry->texture + 4 * i;
        tex[0].x = 0.0f; tex[0].y = 0.0f; tex[0].z = float(i * 9);
        tex[1].x = 1.0f; tex[1].y = 0.0f; tex[1].z = float(i * 9);
        tex[2].x = 0.0f; tex[2].y = 1.0f; tex[2].z = float(i * 9);
        tex[3].x = 1.0f; tex[3].y = 1.0f; tex[3].z = float(i * 9);
        Byte* index = geometry->indices + i * 6;
        index[0] = i * 4 + 2; index[1] = i * 4 + 1; index[2] = i * 4;
        index[3] = i * 4 + 2; index[4] = i * 4 + 3; index[5] = i * 4 + 1;
    }
    unsigned kind = settings->memoryKind;
    fn_00296048(geometry, sizeof(Geometry));
    if (kind == 1 || kind == 2) {
        Allocate allocate;
        Deallocate deallocate;
        nngxGetAllocator(&allocate, &deallocate);
        Geometry* transferred = static_cast<Geometry*>(allocate(kind == 1 ? 0x20000 : 0x30000, 0x102, 0, sizeof(Geometry)));
        fn_00281DDC(geometry, transferred, sizeof(Geometry));
        geometry = transferred;
    }
    word(p, 0x60770) = reinterpret_cast<Word>(geometry);
    fn_00220A6C(p + 0x3A24C, &input, "Position");
    Vertex* vertex = reinterpret_cast<Vertex*>(p + 0x39AAC);
    fn_00220DC0(vertex, &input, nngxGetPhysicalAddr(geometry), 11);
    fn_00220A6C(p + 0x3A24C, &input, "TexCoord0");
    fn_00220DC0(vertex, &input, nngxGetPhysicalAddr(geometry->texture), 11);
    word(p, 0x3A23C) = nngxGetPhysicalAddr(geometry->indices);
    word(p, 0x3A240) = 48;
    p[0x3A244] = 1;
    Word* commands = fn_001EEAD0(reinterpret_cast<Word*>(p + 0x5FC00));
    Word combiner[0x94 / 4];
    fn_0028E758(combiner);
    fn_001EFA74(combiner);
    commands = fn_002204FC(combiner, commands);
    *commands++ = 1; *commands++ = 0xF0245;
    commands = fn_00220828(p + 0x3A24C, commands);
    if (vertex->count == 0) {
        Word* end = fn_0022151C(vertex, vertex->commands);
        vertex->count = end - vertex->commands;
    }
    __aeabi_memcpy4(commands, vertex->commands, vertex->count * sizeof(Word));
    commands += vertex->count;
    Word renderStorage[0x84 / 4];
    Byte* render = reinterpret_cast<Byte*>(renderStorage);
    fn_0028D3BC(render);
    render[0] = 1; render[5] = 7; render[1] = 0; render[4] = 7;
    render[3] = 6; render[2] = 0; render[6] = 1;
    commands = fn_0028A5EC(render, commands, false);
    render[0x54] = 1; render[0x55] = 0;
    commands = fn_00223CA4(render + 0x54, commands, false);
    render[0x30] = 1; render[0x32] = 6;
    commands = fn_00337948(render + 0x30, commands, false);
    render[0x5C] = 0;
    commands = fn_003378CC(render + 0x5C, commands, false);
    commands = fn_0028A4A0(render + 0x80, commands, true);
    *commands++ = 0; *commands++ = 0xF0081;
    Word physicalBase = nngxGetPhysicalAddr(fn_0028E280(0x20000));
    *commands++ = (word(p, 0x3A23C) - ((physicalBase >> 3) << 3)) | (p[0x3A244] ? 0 : 0x80000000);
    *commands++ = 0xF0227;
    *commands++ = 0; *commands++ = 0xF0231;
    word(p, 0x60560) = commands - reinterpret_cast<Word*>(p + 0x5FC00);
    Word* reset = reinterpret_cast<Word*>(p + 0x60564);
    fn_0028D3BC(render);
    render[0] = 0;
    reset = fn_0028A5EC(render, reset, true);
    reset = fn_0028A4A0(render + 0x80, reset, true);
    *reset++ = 0; *reset++ = 0x500E0;
    word(p, 0x60764) = reset - reinterpret_cast<Word*>(p + 0x60564);
    if (oldHeap != reinterpret_cast<void*>(1))
        _ZN4sead7HeapMgr15setCurrentHeap_EPNS_4HeapE(dat_003E23B8, oldHeap);
    return p;
}
#endif
