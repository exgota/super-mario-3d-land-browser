namespace {
struct Object {
    unsigned char pad[0x2c];
    float value;
};
}

extern "C" float fn_00326CF4(const Object* object) {
    return object->value;
}
