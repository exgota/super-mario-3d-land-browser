namespace {
struct Object {
    char pad[0x70];
    int value;
};
extern "C" void fn_0026AA60(int);
extern "C" void fn_0026A9B8(int);
}

extern "C" int fn_00153830(Object* self) {
    int value = self->value;
    if (value != 0) {
        fn_0026AA60(value);
        return 1;
    }
    return 0;
}

extern "C" int fn_0015384C(Object* self) {
    int value = self->value;
    if (value != 0) {
        fn_0026A9B8(value);
        return 1;
    }
    return 0;
}
