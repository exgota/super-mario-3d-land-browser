namespace {
struct Object {
    unsigned char padding[0xB8];
    signed char value;
};
}

extern "C" signed char fn_00327B34(Object *object) {
    return object->value;
}
