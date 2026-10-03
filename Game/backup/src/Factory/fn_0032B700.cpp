namespace {
struct Fn0032B700Object {
    unsigned char padding[0x31];
    signed char value;
};
}

extern "C" signed char fn_0032B700(const Fn0032B700Object* object) {
    return object->value;
}
