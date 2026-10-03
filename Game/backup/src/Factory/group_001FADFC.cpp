namespace {
struct WrapperObject {
    char pad[0x68];
    void *value;
};
}

extern "C" void *fn_001FAE04(void *);
extern "C" void *fn_001FB094(void *);
extern "C" void *fn_001FB0D8(void *);

extern "C" void *fn_001FADFC(WrapperObject *self) {
    return fn_001FAE04(self->value);
}

extern "C" void *fn_001FB08C(WrapperObject *self) {
    return fn_001FB094(self->value);
}

extern "C" void *fn_001FB0D0(WrapperObject *self) {
    return fn_001FB0D8(self->value);
}
