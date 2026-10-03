namespace {
struct Object {
    unsigned char padding[0x28];
    void* value;
};
}

extern "C" void* fn_003274F8(Object* self) {
    return self->value;
}
