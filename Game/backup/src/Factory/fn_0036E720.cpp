namespace {
struct Object {
    char pad[0x2c0];
    float value;
};
}

extern "C" float fn_0036E720(Object* self) {
    return self->value;
}
