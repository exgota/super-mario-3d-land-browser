namespace {
struct Object {
    unsigned char padding[0x68];
    unsigned int value;
};
}

extern "C" void fn_002D6DF8(Object* self, unsigned int value) {
    self->value = value;
}
