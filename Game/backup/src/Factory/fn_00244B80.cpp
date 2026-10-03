namespace {
struct Inner {
    unsigned char padding[0xA0];
    void* value;
};

struct Object {
    unsigned char padding[0x2000];
    Inner inner;
};
}

extern "C" int fn_00244B8C(void*);

extern "C" int fn_00244B80(Object* self) {
    return fn_00244B8C(self->inner.value);
}
