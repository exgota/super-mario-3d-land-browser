namespace {
struct WrapperObject {
    unsigned char pad[0x40];
    void** member;
};
}

extern "C" void* fn_00260E98(void*);
extern "C" void* fn_0024FE58(void*);

extern "C" void* fn_00271390(WrapperObject* self) {
    return fn_00260E98(self->member[1]);
}

extern "C" void* fn_002799E0(WrapperObject* self) {
    return fn_0024FE58(self->member[1]);
}
