namespace {
struct Inner {
    unsigned char pad[0x18];
    float value;
};

struct Outer {
    unsigned char pad[0x20];
    Inner* inner;
};
}

extern "C" float fn_00265EB4(Outer* self) {
    return self->inner->value;
}
