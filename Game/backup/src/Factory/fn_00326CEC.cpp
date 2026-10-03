namespace {
struct Object {
    unsigned char pad[0x28];
    float value;
};
}

extern "C" float fn_00326CEC(const Object* self) {
    return self->value;
}
