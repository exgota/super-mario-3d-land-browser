namespace {
struct TargetObject {
    unsigned char padding[0xa8];
    int value;
};
}

extern "C" void fn_0011E230(TargetObject* object, int value) {
    object->value = value;
}

extern "C" void fn_0030AE3C(TargetObject* object, int value) {
    object->value = value;
}
