namespace {
struct DispatchTable {
    void *slot0;
    void (*slot1)(void *);
};
struct DispatchObject {
    DispatchTable *vtable;
};
struct Wrapper {
    unsigned char pad[0x10];
    DispatchObject *target;
};
}

extern "C" void fn_001972DC(Wrapper *self) {
    self->target->vtable->slot1(self->target);
}

extern "C" void fn_0019D7B4(Wrapper *self) {
    self->target->vtable->slot1(self->target);
}
