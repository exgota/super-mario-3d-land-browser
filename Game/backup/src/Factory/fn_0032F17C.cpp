namespace {
struct UnknownObject {
    unsigned char pad[0x8c];
    float value;
};
}

extern "C" float fn_0032F17C(const UnknownObject* self) {
    return self->value;
}
