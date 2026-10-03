namespace {
struct Inner {
    unsigned char pad[0xC];
    unsigned value;
};

struct Outer {
    unsigned char pad[0x60];
    Inner* inner;
};
}

extern "C" unsigned fn_00166F3C(Outer* self) {
    return self->inner->value;
}
