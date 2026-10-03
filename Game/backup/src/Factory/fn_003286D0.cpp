namespace {
struct Object {
    unsigned char pad[0x7c];
    signed char value;
};
}

extern "C" signed char fn_003286D0(const Object* object) {
    return object->value;
}
