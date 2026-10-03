namespace {
struct Object {
    signed char padding[0x1d];
    signed char value;
};
}

extern "C" int fn_00376A4C(Object* object) {
    return object->value;
}
