namespace {
struct Vector3 {
    float x, y, z;
};

struct Actor {
    char padding[0x64];
    Vector3 base;
    Vector3 direction;
    int start;
    int duration;
    int wait;
};

struct SafeString {
    const void* vtable;
    const char* text;
    explicit SafeString(const char* value);
};

extern "C" {
extern char _ZTVN4sead14SafeStringBaseIcEE[];
extern char dat_003C0940[];
extern char dat_003C0954[];
extern char dat_003F3010;
extern char dat_003BB894[];
extern char dat_003BB8A8[];
extern char dat_003F2194;

bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void fn_00279D3C(void*);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Actor*, int);
void fn_0027109C(void*, const SafeString&);
float fn_00279CF0(Actor*, int, float, float);
float fn_00279C8C(Actor*, int, int);
float fn_00287908(float);
Vector3* _ZN2al11getTransPtrEPNS_9LiveActorE(Actor*);
void _ZN4sead14Vector3CalcCtrIfE13multScalarAddERN2nn4math4VEC3EfRKS4_S7_(Vector3&, float, const Vector3&, const Vector3&);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const void*);
}

inline SafeString::SafeString(const char* value)
    : vtable(_ZTVN4sead14SafeStringBaseIcEE + 8), text(value) {}

}

extern "C" void fn_0011D7A8(Actor* actor) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_00279D3C(actor->padding + 8);
    if (_ZN2al6isStepEPNS_9IUseNerveEi(actor, actor->start) &&
        !_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        SafeString name(dat_003C0940);
        fn_0027109C(actor->padding + 8, name);
    }
    float base = fn_00279CF0(actor, actor->start, -60.0f, 0.0f);
    float rate = fn_00279C8C(actor, actor->start, actor->start + actor->duration);
    float damped = fn_00287908(rate * 3.14159265358979323846f * 4.0f) * (1.0f - rate);
    float wave = damped * 20.0f;
    Vector3* trans = _ZN2al11getTransPtrEPNS_9LiveActorE(actor);
    const Vector3& direction = actor->direction;
    const Vector3& origin = actor->base;
    _ZN4sead14Vector3CalcCtrIfE13multScalarAddERN2nn4math4VEC3EfRKS4_S7_(*trans, base + wave, direction, origin);
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, actor->start + actor->duration + actor->wait)) {
        if (!_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
            SafeString name(dat_003C0954);
            fn_0027109C(actor->padding + 8, name);
        }
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3010);
    }
}

extern "C" void fn_00307F90(Actor* actor) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_00279D3C(actor->padding + 8);
    if (_ZN2al6isStepEPNS_9IUseNerveEi(actor, actor->start) &&
        !_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        SafeString name(dat_003BB894);
        fn_0027109C(actor->padding + 8, name);
    }
    float base = fn_00279CF0(actor, actor->start, -60.0f, 0.0f);
    float rate = fn_00279C8C(actor, actor->start, actor->start + actor->duration);
    float wave = fn_00287908(rate * 3.14159265358979323846f * 4.0f) * (1.0f - rate);
    wave *= 20.0f;
    Vector3* trans = _ZN2al11getTransPtrEPNS_9LiveActorE(actor);
    const Vector3& direction = actor->direction;
    const Vector3& origin = actor->base;
    _ZN4sead14Vector3CalcCtrIfE13multScalarAddERN2nn4math4VEC3EfRKS4_S7_(*trans, base + wave, direction, origin);
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, actor->start + actor->duration + actor->wait)) {
        if (!_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
            SafeString name(dat_003BB8A8);
            fn_0027109C(actor->padding + 8, name);
        }
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2194);
    }
}
