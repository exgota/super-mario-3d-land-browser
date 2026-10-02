namespace {
struct Object {
    unsigned char padding[0x98];
    signed char value;
};
}

extern "C" signed char fn_002514BC(Object* object) {
    return object->value;
}
