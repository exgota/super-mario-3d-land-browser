namespace {
struct UnknownObject {
    unsigned char padding[0x10];
    float value;
};
}

extern "C" float fn_00376510(const UnknownObject* object) {
    return object->value;
}
