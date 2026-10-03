extern "C" unsigned char _ZTVN4sead14SafeStringBaseIcEE[];

namespace {
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value)
        : vtable(_ZTVN4sead14SafeStringBaseIcEE + 8), text(value) {}
};
struct Vec2 {
    float x;
    float y;
};
struct Layout {
    unsigned char base[4];
    unsigned char animation[4];
    unsigned char pane[4];
};
struct ItemStock {
    unsigned char base[12];
    Layout* layout;
    unsigned char unknown[4];
    Vec2 position;
    float depth;
};
struct Nerve {};
}

extern "C" {
extern Vec2 dat_003F3644;
extern Nerve dat_003EFA98;
extern Nerve dat_003EFA94;
void fn_0027109C(void*, const SafeString&);
void fn_00176B5C(void*);
void fn_001589E8(Layout*);
void fn_002711A8(Vec2*, const Vec2*);
void fn_0027BEA0(void*, const char*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(ItemStock*, const Nerve*);
}

extern "C" void fn_00255C78(ItemStock* self, void* source, const Vec2* position, bool flag) {
    fn_0027109C(self->layout ? self->layout->animation : 0, SafeString("SeSyItemStockGot"));
    fn_00176B5C(source);
    fn_001589E8(self->layout);
    Vec2 converted = dat_003F3644;
    fn_002711A8(&converted, position);
    self->position = converted;
    self->depth = 0.0f;
    fn_0027BEA0(self->layout ? self->layout->pane : 0, "Blur", 0);
    if (flag)
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003EFA98);
    else
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003EFA94);
}
