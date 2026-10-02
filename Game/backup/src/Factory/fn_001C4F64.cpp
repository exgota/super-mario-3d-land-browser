namespace {
struct ByamlIter {
    const void* root;
    const void* node;
};

struct LightParameters {
    unsigned char storage[0x60];
};

struct LightPreset {
    const char* name;
    int interpolateFrame;
    LightParameters playerLight;
    LightParameters objLight;
    LightParameters mapObjLight;
};
}

extern "C" void _ZN2al9ByamlIterC1Ev(ByamlIter*);
extern "C" bool _ZNK2al9ByamlIter17tryGetStringByKeyEPPKcS2_(const ByamlIter*, const char**, const char*);
extern "C" bool _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(const ByamlIter*, int*, const char*);
extern "C" bool _ZNK2al9ByamlIter15tryGetIterByKeyEPS0_PKc(const ByamlIter*, ByamlIter*, const char*);
extern "C" void fn_0024E0B8(LightParameters*, const ByamlIter*);

extern "C" void fn_001C4F64(LightPreset* preset, const ByamlIter* source) {
    ByamlIter iter;
    _ZN2al9ByamlIterC1Ev(&iter);
    _ZNK2al9ByamlIter17tryGetStringByKeyEPPKcS2_(source, &preset->name, "Name");
    _ZNK2al9ByamlIter14tryGetIntByKeyEPiPKc(source, &preset->interpolateFrame, "Interpolate Frame");
    if (_ZNK2al9ByamlIter15tryGetIterByKeyEPS0_PKc(source, &iter, "Player Light"))
        fn_0024E0B8(&preset->playerLight, &iter);
    if (_ZNK2al9ByamlIter15tryGetIterByKeyEPS0_PKc(source, &iter, "Obj Light"))
        fn_0024E0B8(&preset->objLight, &iter);
    if (_ZNK2al9ByamlIter15tryGetIterByKeyEPS0_PKc(source, &iter, "MapObj Light"))
        fn_0024E0B8(&preset->mapObjLight, &iter);
}
