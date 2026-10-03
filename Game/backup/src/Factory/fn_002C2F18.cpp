namespace {
struct Object {
    char pad[0x10];
    float value;
};
}

extern "C" void fn_002C2F18(Object* self, float value) {
    self->value = value;
}
