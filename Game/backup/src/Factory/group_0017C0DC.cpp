namespace al {
class LayoutActor {
public:
    void kill();
};
}

extern "C" void fn_0027E90C(void*);

namespace {
struct Object {
    unsigned char padding[0x30];
    void* resource;
};
}

extern "C" void fn_0017C0DC(Object* self) {
    fn_0027E90C(self->resource);
    reinterpret_cast<al::LayoutActor*>(self)->kill();
}

extern "C" void fn_001A2A24(Object* self) {
    fn_0027E90C(self->resource);
    reinterpret_cast<al::LayoutActor*>(self)->kill();
}
