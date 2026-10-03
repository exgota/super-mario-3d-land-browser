namespace {
struct Object {
    unsigned char pad0[0x64];
    unsigned int value64;
    unsigned char pad68[0x18];
    unsigned int value80;
};
}

extern "C" void fn_0011E9EC(Object *self) {
    self->value64 = self->value80;
}
