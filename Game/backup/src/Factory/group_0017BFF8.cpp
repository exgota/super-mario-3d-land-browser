namespace {
struct Owner {
    unsigned char pad[0x194];
    void *value;
};
}

extern "C" void *fn_0017C000(void *);
extern "C" void *fn_0017C0A4(void *);

extern "C" void *fn_0017BFF8(Owner *self) {
    return fn_0017C000(self->value);
}

extern "C" void *fn_0017C09C(Owner *self) {
    return fn_0017C0A4(self->value);
}
