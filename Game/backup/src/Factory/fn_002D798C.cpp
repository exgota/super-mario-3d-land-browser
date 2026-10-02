extern "C" const void* _ZTVN4sead14SafeStringBaseIcEE[];

namespace {
struct ActorInterface { unsigned char storage[0x54]; };
struct Presentation;
struct Auxiliary;
struct Nerve;
struct StateOwner {
    unsigned char storage[0x0c];
    ActorInterface actor;
    Presentation* presentation;
    unsigned char padding[0x50];
    Auxiliary* auxiliary;
};
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value)
        : vtable(&_ZTVN4sead14SafeStringBaseIcEE[2]), text(value) {}
};
struct Vector3 {
    float x, y, z;
    Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
};
}

extern "C" {
extern const char dat_003BEE5C[];
extern const char dat_003BEE4C[];
extern const Nerve dat_003F2AD0;
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const StateOwner*);
bool _ZN2al6isStepEPNS_9IUseNerveEi(StateOwner*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(StateOwner*, const Nerve*);
void fn_002695AC(Presentation*, const SafeString&);
void fn_002695A0(Presentation*);
void fn_00273044(Presentation*, const SafeString&);
void fn_00273050(Presentation*, const Vector3&);
void fn_002D7A80(StateOwner*);
void fn_0026A9B8(Auxiliary*);
bool fn_00279B64(const ActorInterface*);
void fn_00256BE8(ActorInterface*);
bool fn_0025AAEC(const ActorInterface*);
void fn_0025AAA4(ActorInterface*);

void fn_002D798C(StateOwner* self) {
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(self)) {
        SafeString name(dat_003BEE5C);
        fn_002695AC(self->presentation, name);
        fn_002695A0(self->presentation);
        name = SafeString(dat_003BEE4C);
        fn_00273044(self->presentation, name);
        Vector3 zero(0.0f, 0.0f, 0.0f);
        fn_00273050(self->presentation, zero);
        fn_002D7A80(self);
        if (self->auxiliary)
            fn_0026A9B8(self->auxiliary);
        if (fn_00279B64(&self->actor))
            fn_00256BE8(&self->actor);
        if (fn_0025AAEC(&self->actor))
            fn_0025AAA4(&self->actor);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(self, 3))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(self, &dat_003F2AD0);
}
}
