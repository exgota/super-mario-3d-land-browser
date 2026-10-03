namespace {
struct Inner {
    unsigned int value;
};

struct Holder {
    unsigned char pad[0x14];
    Inner* inner;
};

struct Object {
    unsigned char pad[0x68];
    Holder* holder;
};
}

extern "C" void* fn_0014CC3C(void*);

extern "C" void* fn_0014CC30(Object* self) {
    return fn_0014CC3C(self->holder->inner);
}
