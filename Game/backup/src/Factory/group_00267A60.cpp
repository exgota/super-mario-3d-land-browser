namespace {
struct Object267A {
    unsigned char pad[0x60];
    void* value;
};
}

extern "C" void* fn_00267A68(void*);
extern "C" void* fn_00268BE4(void*);

extern "C" void* fn_00267A60(Object267A* self) {
    return fn_00267A68(self->value);
}

extern "C" void* fn_00268BDC(Object267A* self) {
    return fn_00268BE4(self->value);
}
