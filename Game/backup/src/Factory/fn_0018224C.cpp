namespace {
struct Object {
    unsigned char padding[0x17];
    unsigned char flag;
};
}

extern "C" void fn_0018224C(Object *object) {
    object->flag = 1;
}
