namespace {
struct Object {
    unsigned char padding[0x34];
    unsigned char field;
};
}

extern "C" void fn_0013DF8C(void* object) {
    static_cast<Object*>(object)->field = 1;
}
