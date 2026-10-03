namespace {
struct UnknownObject {
    signed char padding[0x27];
    signed char value;
};
}

extern "C" signed char fn_00375F08(UnknownObject* self) {
    return self->value;
}
