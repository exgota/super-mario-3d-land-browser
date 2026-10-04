namespace {
struct Inner {
    unsigned int reserved;
    void* value;
};

struct Outer {
    unsigned int reserved[2];
    Inner* inner;
};
}

extern "C" void* fn_002428CC(Outer* self) {
    return self->inner->value;
}
