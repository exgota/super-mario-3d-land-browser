namespace {
struct Object {
    unsigned char padding[0xB8];
    void* value;
};
}

extern "C" void* fn_00289E14(Object* object) {
    return object->value;
}
