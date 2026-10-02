namespace {
struct Inner {
    void *first;
    unsigned char pad[12];
    void *field_10;
};
struct Wrapper {
    unsigned char pad[4];
    Inner *inner;
};
}

extern "C" void *fn_00252F50(void *, void *, void *);

extern "C" void *fn_0019EDB8(Wrapper *self, void *arg1, void *arg2) {
    Inner *p = self->inner;
    return fn_00252F50(p->field_10, p->first, arg2);
}

extern "C" void *fn_0019F3B8(Wrapper *self, void *arg1, void *arg2) {
    Inner *p = self->inner;
    return fn_00252F50(p->field_10, p->first, arg2);
}

extern "C" void *fn_001BA134(Wrapper *self, void *arg1, void *arg2) {
    Inner *p = self->inner;
    return fn_00252F50(p->field_10, p->first, arg2);
}
