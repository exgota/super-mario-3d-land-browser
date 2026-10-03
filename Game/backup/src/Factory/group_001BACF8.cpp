namespace {
struct ByteFieldC {
    unsigned char pad[0xC];
    unsigned char value;
};
}

extern "C" void fn_001BACF8(ByteFieldC *self) {
    self->value = 0;
}

extern "C" void fn_002D287C(ByteFieldC *self) {
    self->value = 0;
}
