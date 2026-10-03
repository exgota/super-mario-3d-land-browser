namespace {
struct Object {
    unsigned char padding[0x85];
    unsigned char value;
};
}

extern "C" void fn_0023F928(void *object) {
    static_cast<Object *>(object)->value = 1;
}
