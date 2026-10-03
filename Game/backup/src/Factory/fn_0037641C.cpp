namespace {
struct Object {
    unsigned char pad[0x90];
    unsigned int value;
};
}

extern "C" unsigned int fn_0037641C(Object *self) {
    return self->value;
}
