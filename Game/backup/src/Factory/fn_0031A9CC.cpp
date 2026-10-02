namespace {
struct Vector3 { float x, y, z; };
struct Actor {
    char padding0[0x6c];
    void* timer;
    char padding1[0x14];
    bool flag;
};
struct Nerve {};
}

extern "C" {
void _ZN2al9LiveActor6appearEv(Actor*);
void _ZN2al8setTransEPNS_9LiveActorERKN4sead7Vector3IfEE(Actor*, const Vector3&);
void _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(Actor*, const Vector3&);
void fn_0027C05C(Actor*);
void fn_00270FA8(void*, int);
void _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(const Actor*, const char*);
void fn_00277AF0(Actor*);
void _ZN2al9onCollideEPNS_9LiveActorE(Actor*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern Nerve dat_003F0F30;

void fn_0031A9CC(Actor* actor, const Vector3& position, const Vector3& velocity, bool flag) {
    _ZN2al9LiveActor6appearEv(actor);
    _ZN2al8setTransEPNS_9LiveActorERKN4sead7Vector3IfEE(actor, position);
    _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(actor, velocity);
    fn_0027C05C(actor);
    fn_00270FA8(actor->timer, 300);
    _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(actor, "\x83\x52\x83\x43\x83\x93\x83\x7c\x83\x62\x83\x76\x83\x41\x83\x62\x83\x76");
    fn_00277AF0(actor);
    _ZN2al9onCollideEPNS_9LiveActorE(actor);
    actor->flag = flag;
    _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F0F30);
}
}
