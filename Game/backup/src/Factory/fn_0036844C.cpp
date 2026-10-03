namespace {
struct Vec3 { float x, y, z; };
struct Nerve {};
struct Spawned {};
struct Spawner {};
struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void kill() = 0;
    char padding[0x5c];
    Spawner* spawner;
    int kind;
    int unknown68;
    int remaining;
    bool special;
};
struct Spine { Actor* actor; };
}
extern "C" {
bool fn_00256B58(Actor*, int);
const Vec3& _ZN2al8getFrontEPKNS_9LiveActorE(const Actor*);
const Vec3& _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vec3&, const Vec3&, float);
Spawned* fn_002674AC(Spawner*, const Vec3&, const Vec3&, int, const char*);
void fn_0031F158(Spawned*);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Actor*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern const char dat_003BA384[];
extern const Nerve dat_003F1DAC;
}
extern "C" void fn_0036844C(const Nerve*, Spine* spine) {
    Actor* actor = spine->actor;
    if (fn_00256B58(actor, 60)) {
        Vec3 velocity;
        Vec3 scaled;
        _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(scaled, _ZN2al8getFrontEPKNS_9LiveActorE(actor), 40.0f);
        velocity = scaled;
        Spawned* spawned = fn_002674AC(actor->spawner, _ZN2al8getTransEPKNS_9LiveActorE(actor), velocity, actor->kind, dat_003BA384);
        if (actor->special) fn_0031F158(spawned);
        actor->special = false;
        if (actor->remaining != -1) {
            if (--actor->remaining == 0) actor->kill();
        }
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(actor, 120))
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1DAC);
}
