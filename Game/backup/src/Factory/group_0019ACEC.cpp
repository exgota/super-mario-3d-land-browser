namespace {
struct Field64 {
    unsigned char pad[0x64];
    unsigned int value;
};
}

extern "C" void fn_0019ACEC(Field64 *self, unsigned int value) {
    self->value = value;
}

extern "C" void fn_002D65FC(Field64 *self, unsigned int value) {
    self->value = value;
}

extern "C" void fn_00314C28(Field64 *self, unsigned int value) {
    self->value = value;
}
