namespace {
struct Holder {
    unsigned char padding[0x60];
    void* target;
};
}

extern "C" void* fn_00268BE4(void*);
extern "C" void* fn_00267A68(void*);
extern "C" void* fn_00199390(void*);

extern "C" void* fn_00146A64(Holder* self) {
    return fn_00268BE4(self->target);
}

extern "C" void* fn_00263A84(Holder* self) {
    return fn_00267A68(self->target);
}

extern "C" void* fn_00309AC0(Holder* self) {
    return fn_00199390(self->target);
}

extern "C" void* fn_0031A490(Holder* self) {
    return fn_00268BE4(self->target);
}
