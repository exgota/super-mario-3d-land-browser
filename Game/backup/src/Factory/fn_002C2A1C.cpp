namespace {
struct UnknownObject {
    unsigned char pad[0x14];
    float value;
};
}

extern "C" void fn_002C2A1C(UnknownObject *self, float value) {
    self->value = value;
}
