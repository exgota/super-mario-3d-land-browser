namespace {
struct Object {
    unsigned char padding[0x14];
    int value;
};
}

extern "C" void fn_001F9B48(Object* self) {
    self->value = 0;
}

extern "C" void fn_002BD86C(Object* self) {
    self->value = 0;
}
