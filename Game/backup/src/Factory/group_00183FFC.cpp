namespace {
struct ThunkObject {
    char pad[0x10];
    void* target;
};
}

extern "C" void fn_0025CB78(void*);
extern "C" void fn_0025BC6C(void*);

extern "C" void fn_00183FFC(ThunkObject* self) {
    fn_0025CB78(self->target);
}

extern "C" void fn_001A5E38(ThunkObject* self) {
    fn_0025BC6C(self->target);
}

namespace nn { namespace os {
class LightEvent {
public:
    void Signal();
};
}}

extern "C" void fn_00392A68(ThunkObject* self) {
    static_cast<nn::os::LightEvent*>(self->target)->Signal();
}
