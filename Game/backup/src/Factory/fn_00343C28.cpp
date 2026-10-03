namespace {
struct Object {
    unsigned char padding[0x10];
    unsigned int value;
};
}

extern "C" unsigned int fn_00343C28(Object* object) {
    return object->value;
}
