namespace {
struct Object {
    unsigned char padding[0x18];
    float value;
};
}

extern "C" void fn_002C28D0(Object* self, float value) {
    self->value = value;
}
