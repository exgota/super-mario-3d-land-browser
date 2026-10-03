namespace {
struct Fn001D18D0Object {
    unsigned char padding[0x18];
    unsigned char *data;
};
}

extern "C" unsigned char *fn_001D18D0(Fn001D18D0Object *object) {
    return object->data + 12;
}
