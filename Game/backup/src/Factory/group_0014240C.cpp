namespace {
struct Nested {
    char pad[8];
    void* value;
};

struct Object {
    char pad[0x64];
    Nested* nested;
};
}

extern "C" void fn_0026AA60(void*);
extern "C" void fn_0026A9B8(void*);
extern "C" void fn_002CEA40(void*);

extern "C" void fn_0014240C(Object* self) {
    void* value = self->nested->value;
    if (value)
        fn_0026AA60(value);
}

extern "C" void fn_00142688(Object* self) {
    void* value = self->nested->value;
    if (value)
        fn_0026A9B8(value);
}

extern "C" void fn_00142AE4(Object* self) {
    void* value = self->nested->value;
    if (value)
        fn_002CEA40(value);
}
