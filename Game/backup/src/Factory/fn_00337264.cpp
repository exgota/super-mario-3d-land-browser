namespace {
struct Fn00337264Object {
    unsigned char padding[0x20];
    void *value;
};
}

extern "C" void *fn_0033726C(void *);

extern "C" void *fn_00337264(Fn00337264Object *self) {
    return fn_0033726C(self->value);
}
