#include <new>

namespace {
struct Auxiliary {
    unsigned char storage[8];
};

struct Owner {
    unsigned char padding[0x58];
    Auxiliary* auxiliary;
};

extern "C" Auxiliary* fn_002715E8(Auxiliary*);
}

extern "C" void fn_001297DC(Owner* self) {
    Auxiliary* auxiliary = new Auxiliary;
    if (auxiliary)
        auxiliary = fn_002715E8(auxiliary);
    self->auxiliary = auxiliary;
}
