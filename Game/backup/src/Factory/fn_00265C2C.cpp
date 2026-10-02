namespace {
struct Object {
    unsigned char padding[0x3c];
    void* field_3c;
};
}

extern "C" void* fn_001493BC();

extern "C" void* fn_00265C2C(Object* self) {
    void* value = self->field_3c;
    if (value != 0)
        return value;
    return fn_001493BC();
}
