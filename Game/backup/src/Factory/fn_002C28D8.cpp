namespace {
struct Object {
    unsigned char padding[0x4D];
    unsigned char value;
};
}

extern "C" void fn_002C28D8(Object *object, unsigned char value) {
    object->value = value;
}
