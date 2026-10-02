namespace {
struct Object {
    unsigned char padding[12];
};
}

extern "C" void fn_0026FBF0(void*);

namespace al {
class LiveActor {
public:
    void kill();
};
}

extern "C" void fn_0013D63C(Object* self) {
    fn_0026FBF0(reinterpret_cast<unsigned char*>(self) + 0xC);
    reinterpret_cast<al::LiveActor*>(self)->kill();
}

extern "C" void fn_002F840C(Object* self) {
    fn_0026FBF0(reinterpret_cast<unsigned char*>(self) + 0xC);
    reinterpret_cast<al::LiveActor*>(self)->kill();
}
