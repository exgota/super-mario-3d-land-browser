namespace {
struct Inner {
    char pad[4];
    void *value;
};

struct Outer {
    char pad[0x28];
    Inner *inner;
};
}

extern "C" void *fn_00227CE4(Outer *self) {
    return self->inner->value;
}
