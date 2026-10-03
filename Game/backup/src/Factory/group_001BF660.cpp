namespace {
struct SafeString {
    const void* vtable;
    const char* text;
};

union VtablePair {
    struct {
        const void* outer;
        const void* inner;
    } pointers;
    unsigned long long bits;
};

struct StringObject {
    volatile unsigned long long vtables;
    const char* text;
};
}

extern "C" {
extern const unsigned char dat_003D60F8[];
extern const unsigned char dat_003D6BF0[];
extern const unsigned char _ZTVN4sead14SafeStringBaseIcEE[];

void fn_001BF660(StringObject* self, const SafeString* source) {
    VtablePair pair;
    pair.pointers.outer = dat_003D60F8;
    pair.pointers.inner = _ZTVN4sead14SafeStringBaseIcEE + 8;
    self->vtables = pair.bits;
    self->text = source->text;
}

void fn_00241678(StringObject* self, const SafeString* source) {
    VtablePair pair;
    pair.pointers.outer = dat_003D6BF0;
    pair.pointers.inner = _ZTVN4sead14SafeStringBaseIcEE + 8;
    self->vtables = pair.bits;
    self->text = source->text;
}
}
