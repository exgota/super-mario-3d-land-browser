namespace {

struct Opaque;

struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void kill() = 0;
    unsigned char reserved[0x5c];
    Opaque* component;
};

struct Spine {
    Actor* actor;
};

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(float a, float b, float c) : x(a), y(b), z(c) {}
};

extern "C" {
extern const char dat_003BFCE0[];
extern const char dat_003BFC5C[];
extern const char dat_003BFCD8[];
extern const char dat_003BFC98[];
extern const void* _ZTVN4sead14SafeStringBaseIcEE[];

bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
void fn_002535E0(int);
Opaque* fn_00257A3C(const char*, int);
void fn_00273800(Opaque*);
Opaque* fn_00273008(Opaque**, const void*);
Opaque* fn_00276D98(Opaque*);
void fn_0026F994(Opaque*);
void fn_00277AF0(Actor*);
Opaque* fn_0026B948(Actor*);
void fn_00255DE8(Opaque*);
}

struct SafeString {
    const void* const* vtable;
    const char* text;

    SafeString(const char* value) : text(value) {
        vtable = _ZTVN4sead14SafeStringBaseIcEE + 2;
    }
};

extern "C" void fn_00273044(Opaque*, const SafeString&);
extern "C" void fn_00273050(Opaque*, const Vector3*);

}

extern "C" void fn_0035487C(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BFCE0);
        fn_002535E0(20);
        fn_00273800(fn_00257A3C(dat_003BFC5C, 0));
        fn_00273044(actor->component, SafeString(dat_003BFCD8));
        Vector3 position(0.0f, 50.0f, 0.0f);
        fn_00273050(actor->component, &position);
        fn_0026F994(fn_00276D98(fn_00273008(&actor->component, reinterpret_cast<const void*>(0x0042FF88))));
        fn_00277AF0(actor);
        fn_00255DE8(fn_0026B948(actor));
    }
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor)) {
        fn_00257A3C(dat_003BFC98, 10);
        actor->kill();
    }
}
