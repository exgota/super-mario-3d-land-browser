namespace {
struct Object {
    char padding[0x18];
    int value;
};
}

extern "C" void fn_0018D5F4(Object *object) {
    object->value = 1;
}
