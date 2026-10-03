namespace {
struct ByteView {
    unsigned char padding[0x2d];
    signed char value;
};
}

extern "C" int fn_003764A0(const ByteView* object) {
    return object->value;
}
