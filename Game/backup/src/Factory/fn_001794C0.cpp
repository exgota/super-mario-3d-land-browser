namespace {
struct ActorInitInfo;
struct Nerve { unsigned int vtable; };
struct NothrowTag {};
struct Helper;
struct Joint;
struct Switch;
struct Quaternion {
    float x, y, z, w;
    Quaternion& operator=(const Quaternion& other) {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
        return *this;
    }
};
struct Actor {
    unsigned char base0[0x0c];
    unsigned int configuration;
    unsigned char base1[0x50];
    Helper* helper;
    Switch* switchA;
    Switch* switchB;
    Quaternion initialQuaternion;
    unsigned char padding[0x20];
    Joint* wheelL;
    Joint* wheelR;
};
extern "C" {
void _ZN2al17initActorPoseTQSVEPNS_9LiveActorE(Actor*);
void fn_0028058C(Actor*, const ActorInitInfo*);
void _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(Actor*, const Nerve*, int);
void* _ZnwjRKSt9nothrow_t(unsigned int, const NothrowTag&);
Helper* fn_00147ADC(void*);
void fn_0026D5E0(Actor*, Joint**, const char*);
const Quaternion* _ZN2al7getQuatEPKNS_9LiveActorE(const Actor*);
void fn_00280538(unsigned int*, const ActorInitInfo*);
void fn_0027FCBC(unsigned int*, const ActorInitInfo*);
void fn_0026EDDC(unsigned int*, const ActorInitInfo*);
Switch* fn_0026D51C(Actor*, const ActorInitInfo*);
Switch* fn_0026D49C(Actor*, const ActorInitInfo*);
void fn_0027FAB8(Actor*);
extern const Nerve dat_003F25F0;
}
}

extern "C" void fn_001794C0(Actor* actor, const ActorInitInfo* info) {
    _ZN2al17initActorPoseTQSVEPNS_9LiveActorE(actor);
    fn_0028058C(actor, info);
    _ZN2al9initNerveEPNS_9LiveActorEPKNS_5NerveEi(actor, &dat_003F25F0, 0);
    void* memory = _ZnwjRKSt9nothrow_t(12, *reinterpret_cast<const NothrowTag*>(actor));
    actor->helper = memory ? fn_00147ADC(memory) : 0;
    fn_0026D5E0(actor, &actor->wheelL, "WheelL");
    fn_0026D5E0(actor, &actor->wheelR, "WheelR");
    actor->initialQuaternion = *_ZN2al7getQuatEPKNS_9LiveActorE(actor);
    fn_00280538(&actor->configuration, info);
    fn_0027FCBC(&actor->configuration, info);
    fn_0026EDDC(&actor->configuration, info);
    actor->switchA = fn_0026D51C(actor, info);
    actor->switchB = fn_0026D49C(actor, info);
    fn_0027FAB8(actor);
}
