namespace {
struct ByteField {
    signed char padding[0x10];
    signed char value;
};
}

extern "C" signed char fn_00326E50(const ByteField *self) {
    return self->value;
}

extern "C" signed char fn_0032B6F0(const ByteField *self) {
    return self->value;
}

extern "C" signed char fn_00333C64(const ByteField *self) {
    return self->value;
}
