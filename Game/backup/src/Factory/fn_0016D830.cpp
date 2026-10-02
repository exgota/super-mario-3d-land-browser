namespace {
struct Vector3 {
    float x, y, z;
    Vector3() {}
    Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
    Vector3 operator+(const Vector3& other) const;
};

struct SafeString {
    const void* vtable;
    const char* text;
    char storage[8];
    explicit SafeString(const char* value);
};

struct Actor {
    char prefix[8];
    char audio[0x60];
    void* child;
};
}

extern "C" {
extern const char dat_003C14A8[];
extern const char dat_003C1494[];
extern const char dat_003C147C[];
extern const char dat_003C1434[];
extern const char dat_003F3230;
extern const void* _ZTVN4sead14SafeStringBaseIcEE[];
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
void fn_0027109C(void*, const SafeString&);
const Vector3& _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(Vector3&, const Vector3&, const Vector3&);
void fn_0012ABF8(void*, const Vector3&);
bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const void*);
}

namespace {
SafeString::SafeString(const char* value) : vtable(_ZTVN4sead14SafeStringBaseIcEE + 2), text(value) {}

Vector3 Vector3::operator+(const Vector3& other) const {
    Vector3 result;
    _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(result, *this, other);
    return result;
}
}

extern "C" void fn_0016D830(Actor* actor) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003C14A8);
        fn_0027109C(actor->audio, SafeString(dat_003C1494));
        fn_0027109C(actor->audio, SafeString(dat_003C147C));
        Vector3 position = _ZN2al8getTransEPKNS_9LiveActorE(actor) + Vector3(0.0f, 100.0f, 0.0f);
        fn_0012ABF8(actor->child, position);
    }
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003C1434);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3230);
    }
}
