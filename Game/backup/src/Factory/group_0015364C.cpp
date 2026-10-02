namespace {
struct Context {
    unsigned char pad[0x6C];
    void* object;
};
}

extern "C" void fn_0026AA60(void*);
extern "C" void fn_0026A9B8(void*);

extern "C" int fn_0015364C(Context* self) {
    if (self->object) {
        fn_0026AA60(self->object);
        return 1;
    }
    return 0;
}

extern "C" int fn_00153814(Context* self) {
    if (self->object) {
        fn_0026A9B8(self->object);
        return 1;
    }
    return 0;
}
