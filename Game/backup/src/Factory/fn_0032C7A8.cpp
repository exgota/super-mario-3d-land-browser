namespace {
struct ValueAtC {
    unsigned char padding[0xC];
    float value;
};
}

extern "C" float fn_0032C7A8(const ValueAtC *self) {
    return self->value;
}
