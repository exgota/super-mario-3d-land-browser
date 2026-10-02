namespace {
struct Object {
    unsigned char padding[0xBC];
    float value;
};
}

extern "C" void fn_002BD5A4(Object* object, float value) {
    object->value = value;
}
