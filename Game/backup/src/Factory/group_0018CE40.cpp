namespace {

struct LayoutInitInfo {};

struct InitInfo {
    void* first;
    LayoutInitInfo* layout;
};

struct Guide;

struct GuideVtable {
    void (*first)();
    void (*second)();
    void (*init)(Guide*);
};

struct Guide {
    GuideVtable* vtable;
    char storage[0x2c];
};

struct Wipe {
    char storage[0x34];
};

struct Nerve {};

extern "C" const unsigned int _ZTVN4sead14SafeStringBaseIcEE[];

struct SafeString {
    const unsigned int* vtable;
    const char* text;

    explicit SafeString(const char* value)
        : vtable(_ZTVN4sead14SafeStringBaseIcEE + 2), text(value) {}
};

struct Object {
    char first[0xc];
    void* actor;
    unsigned int unknown;
    Guide* guide;
    Wipe* fade;
    Wipe* ring;
    void* state;
};

extern "C" void fn_00268018(Object*, void*, const InitInfo*, int, int, int);
extern "C" Guide* fn_0027569C(Guide*, const char*, const char*,
                              const LayoutInitInfo&, const char*);
extern "C" Wipe* _ZN2al10WipeSimpleC1EPKcS2_RKNS_14LayoutInitInfoES2_(
    Wipe*, const char*, const char*, const LayoutInitInfo&, const char*);
extern "C" void _ZN2al13NerveExecutor9initNerveEPKNS_5NerveEi(
    Object*, const Nerve*, int);
extern "C" void* fn_00271038(const SafeString&, int);
extern "C" const Nerve dat_003F1B3C;
extern "C" const Nerve dat_003F1B48;

}

extern "C" void fn_0018CE40(Object* self, const InitInfo* info) {
    fn_00268018(self, self->actor, info, -1, 0, 0);
    Guide* guide = new Guide;
    if (guide)
        guide = fn_0027569C(guide,
            "\x8c\x88\x92\xe8" "A\x83\x7b\x83\x5e\x83\x93",
            "GuideAButton", *info->layout, 0);
    self->guide = guide;
    guide->vtable->init(guide);
    Wipe* fade = new Wipe;
    if (fade)
        fade = _ZN2al10WipeSimpleC1EPKcS2_RKNS_14LayoutInitInfoES2_(fade,
            "\x8d\x95\x83\x74\x83\x46\x81\x5b\x83\x68(\x8f\xe3\x89\xe6\x96\xca)",
            "WipeFadeBlack", *info->layout, 0);
    self->fade = fade;
    Wipe* ring = new Wipe;
    if (ring)
        ring = _ZN2al10WipeSimpleC1EPKcS2_RKNS_14LayoutInitInfoES2_(ring,
            "\x83\x8a\x83\x93\x83\x4f\x83\x8f\x83\x43\x83\x76(\x8f\xe3\x89\xe6\x96\xca)",
            "WipeRing", *info->layout, 0);
    self->ring = ring;
    _ZN2al13NerveExecutor9initNerveEPKNS_5NerveEi(self, &dat_003F1B3C, 0);
    self->state = fn_00271038(SafeString("ProductStateStage"), 4);
}

extern "C" void fn_0019535C(Object* self, const InitInfo* info) {
    fn_00268018(self, self->actor, info, -1, 0, 0);
    Guide* guide = new Guide;
    if (guide)
        guide = fn_0027569C(guide,
            "\x8c\x88\x92\xe8" "A\x83\x7b\x83\x5e\x83\x93",
            "GuideAButton", *info->layout, 0);
    self->guide = guide;
    guide->vtable->init(guide);
    Wipe* fade = new Wipe;
    if (fade)
        fade = _ZN2al10WipeSimpleC1EPKcS2_RKNS_14LayoutInitInfoES2_(fade,
            "\x8d\x95\x83\x74\x83\x46\x81\x5b\x83\x68(\x8f\xe3\x89\xe6\x96\xca)",
            "WipeFadeBlack", *info->layout, 0);
    self->fade = fade;
    Wipe* ring = new Wipe;
    if (ring)
        ring = _ZN2al10WipeSimpleC1EPKcS2_RKNS_14LayoutInitInfoES2_(ring,
            "\x83\x8a\x83\x93\x83\x4f\x83\x8f\x83\x43\x83\x76(\x8f\xe3\x89\xe6\x96\xca)",
            "WipeRing", *info->layout, 0);
    self->ring = ring;
    _ZN2al13NerveExecutor9initNerveEPKNS_5NerveEi(self, &dat_003F1B48, 0);
    self->state = fn_00271038(SafeString("ProductStateStage"), 4);
}
