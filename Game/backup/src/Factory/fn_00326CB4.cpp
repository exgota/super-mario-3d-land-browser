namespace {
struct UnknownObject {
    unsigned char pad[0x3e];
    signed char value;
};
}

extern "C" signed char fn_00326CB4(const UnknownObject *object) {
    return object->value;
}
