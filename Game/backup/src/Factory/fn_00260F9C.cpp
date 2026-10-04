namespace {
struct Fn00260F9CObject {
    unsigned char pad[0x40];
    void **value;
};
}

extern "C" void *fn_00260FA8(void *);

extern "C" void *fn_00260F9C(Fn00260F9CObject *self) {
    return fn_00260FA8(*self->value);
}
