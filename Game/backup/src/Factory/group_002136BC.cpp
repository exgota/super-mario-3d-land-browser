namespace {
struct Owner {
    void* reserved[14];
    void* target;
};
}

extern "C" void fn_0015A62C(void*);
extern "C" void fn_00328884(void*);
extern "C" void fn_003288A0(void*);

extern "C" void fn_002136BC(Owner* self) {
    if (self->target)
        fn_0015A62C(self->target);
}

extern "C" void fn_00213F2C(Owner* self) {
    if (self->target)
        fn_00328884(self->target);
}

extern "C" void fn_0032C8DC(Owner* self) {
    if (self->target)
        fn_003288A0(self->target);
}
