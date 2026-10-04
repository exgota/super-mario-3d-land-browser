namespace {
struct Inner {
    unsigned char pad[0x44];
    float value;
};
struct Object {
    unsigned char pad[0x800];
    Inner inner;
};
}

extern "C" void fn_0025F7B8(Object *self, float value) {
    self->inner.value = value;
}
