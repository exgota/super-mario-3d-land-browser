namespace {
typedef unsigned int u32;
typedef unsigned char u8;
}

extern "C" void fn_001DD5C0(void* p) {
    u32* words = static_cast<u32*>(p);
    words[0] = 0;
    words[1] = 0;
    static_cast<u8*>(p)[8] = 0;
}

extern "C" void fn_002911D4(void* p) {
    u32* words = static_cast<u32*>(p);
    words[0] = 0;
    words[1] = 0;
    static_cast<u8*>(p)[8] = 0;
}

namespace sead {
class HeapMgr {
public:
    unsigned int field_0;
    unsigned int field_4;
    unsigned char field_8;
    HeapMgr();
};

HeapMgr::HeapMgr() : field_0(0), field_4(0), field_8(0) {}
}
