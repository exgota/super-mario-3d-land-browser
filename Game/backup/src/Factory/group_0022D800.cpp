namespace {
struct DispatchVTable {
    void* reserved[3];
    void (*invoke)(void*, void*);
};
struct DispatchObject {
    DispatchVTable* vtable;
};
}

extern "C" DispatchObject* dat_003EF834;
extern "C" DispatchObject* dat_003E2AE4;

extern "C" void fn_0022D81C(void*, void*);
extern "C" void fn_00251400(void*, void*);
extern "C" void fn_00252180(void*, void*);

extern "C" void fn_0022D800(void* self) {
    dat_003EF834->vtable->invoke(dat_003EF834, self);
}

extern "C" void fn_002513E4(void* self) {
    dat_003E2AE4->vtable->invoke(dat_003E2AE4, self);
}

extern "C" void fn_00252164(void* self) {
    dat_003E2AE4->vtable->invoke(dat_003E2AE4, self);
}
