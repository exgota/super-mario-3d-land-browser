namespace {
struct WrapperObject {
    unsigned char padding[0x14];
    void* value;
};
}

extern "C" void* fn_0027B1C0(void*);
extern "C" void* fn_00249CC0(void*);
extern "C" void* fn_002C9318(void*);
extern "C" void* fn_002C936C(void*);

extern "C" void* fn_001604B4(WrapperObject* self) {
    return fn_0027B1C0(self->value);
}

extern "C" void* fn_0027B2A0(WrapperObject* self) {
    return fn_00249CC0(self->value);
}

extern "C" void* fn_00376310(WrapperObject* self) {
    return fn_002C9318(self->value);
}

extern "C" void* fn_00376318(WrapperObject* self) {
    return fn_002C936C(self->value);
}
