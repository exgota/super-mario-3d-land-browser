namespace {
struct Object {
    unsigned char padding[0x5c];
    float value;
};
}

extern "C" float fn_0032EBDC(const Object* self) {
    return self->value;
}
