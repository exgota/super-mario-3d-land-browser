namespace {
struct Fn00326C78Object {
    char padding[0x3a];
    signed char value;
};
}

extern "C" signed char fn_00326C78(Fn00326C78Object* self) {
    return self->value;
}
