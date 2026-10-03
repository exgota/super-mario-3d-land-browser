namespace {
struct Vector3 {
    float x, y, z;
    void set(const Vector3& value) { *this = value; }
};
struct KeyPoseKeeper;
struct Nerve;
struct Actor {
    char padding0[0x60];
    KeyPoseKeeper* keeper;
    char padding64[0x0c];
    float progress;
    bool enabled;
    char padding75[3];
    Vector3 origin;
};
struct Spine { Actor* actor; };
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
const Vector3& _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
int fn_00375408(Actor*, const Vector3&);
float fn_00255C04(Actor*, int);
const Vector3& _ZN2al15getNextKeyTransEPKNS_13KeyPoseKeeperE(const KeyPoseKeeper*);
Vector3* _ZN2al11getTransPtrEPNS_9LiveActorE(Actor*);
void _ZN2al7lerpVecEPN4sead7Vector3IfEERKS2_S5_f(Vector3*, const Vector3&, const Vector3&, float);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Actor*, int);
void fn_001C5BD4(KeyPoseKeeper*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern const char dat_003BC9F8[];
extern const Nerve dat_003F248C;
}

extern "C" void fn_00349F34(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC9F8);
        actor->origin.set(_ZN2al8getTransEPKNS_9LiveActorE(actor));
    }
    int duration = fn_00375408(actor, actor->origin);
    float rate = fn_00255C04(actor, duration);
    const Vector3& next = _ZN2al15getNextKeyTransEPKNS_13KeyPoseKeeperE(actor->keeper);
    _ZN2al7lerpVecEPN4sead7Vector3IfEERKS2_S5_f(
        _ZN2al11getTransPtrEPNS_9LiveActorE(actor), actor->origin, next, rate);
    if (_ZN2al6isStepEPNS_9IUseNerveEi(actor, duration) || (actor->enabled && actor->progress < 1.05f)) {
        fn_001C5BD4(actor->keeper);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F248C);
    }
}
