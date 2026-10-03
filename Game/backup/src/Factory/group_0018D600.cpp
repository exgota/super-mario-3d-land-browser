namespace {
struct Object {
    unsigned char pad[0x18];
    int value;
};
}

extern "C" void fn_0018D600(Object *object) {
    object->value = 0;
}

extern "C" void fn_00229990(Object *object) {
    object->value = 0;
}
