namespace {
struct EntryObject {
    unsigned char padding[0x34];
    void* layout;
};
}

extern "C" void fn_0026C0B8(void*);
extern "C" void fn_0027E90C(void*);

namespace al {
class LayoutActor {
public:
    void kill();
};
}

extern "C" void fn_00137618(EntryObject* self) {
    fn_0026C0B8(self->layout);
    ((al::LayoutActor*)self)->kill();
}

extern "C" void fn_00139344(EntryObject* self) {
    fn_0027E90C(self->layout);
    ((al::LayoutActor*)self)->kill();
}
