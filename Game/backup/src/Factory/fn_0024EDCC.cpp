namespace {
struct Inner0024EDCC {
    unsigned char pad[0xC];
    void* value;
};

struct Outer0024EDCC {
    unsigned char pad[0x18];
    Inner0024EDCC* inner;
};
}

extern "C" void* fn_0024EDCC(Outer0024EDCC* self) {
    return self->inner->value;
}
