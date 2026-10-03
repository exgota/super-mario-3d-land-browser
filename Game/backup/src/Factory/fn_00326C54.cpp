namespace {
struct Object {
    char padding[0x39];
    signed char value;
};
}

extern "C" int fn_00326C54(Object *self) {
    return self->value;
}
