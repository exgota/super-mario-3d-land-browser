namespace {
struct Actor;
struct ActorVtable {
    void (*slots[5])(Actor*);
    void (*kill)(Actor*);
};
struct Partner {
    char reserved[0x6c];
    bool active;
};
struct Actor {
    ActorVtable* vtable;
    char reserved04[0x90];
    bool flag94;
    char reserved95[0x13];
    Partner* partner;
};
struct Spine { Actor* actor; };
struct Vector3 { float x, y, z; };
struct Quaternion { float x, y, z, w; };
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al15setVelocityZeroEPNS_9LiveActorE(Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
void fn_002CF370(Actor*, int);
void fn_00214680(Actor*);
bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
void _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(const Actor*);
const Quaternion* _ZN2al7getQuatEPKNS_9LiveActorE(const Actor*);
const Vector3* _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
void fn_0026BEC4(Actor*, const Vector3*, const Quaternion*);
void _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(const Actor*, const char*);
extern const char dat_003BBB98[];
extern const char dat_003BBB48[];
}

extern "C" void fn_00351C84(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al15setVelocityZeroEPNS_9LiveActorE(actor);
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BBB98);
    }
    if (!actor->partner || !actor->partner->active) {
        if (actor->flag94)
            fn_002CF370(actor, 12);
        else
            fn_00214680(actor);
    }
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor)) {
        _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(actor);
        if (actor->partner && actor->partner->active) {
            const Quaternion* quat = _ZN2al7getQuatEPKNS_9LiveActorE(actor);
            const Vector3* trans = _ZN2al8getTransEPKNS_9LiveActorE(actor);
            fn_0026BEC4(actor, trans, quat);
            _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(actor, dat_003BBB48);
        }
        actor->vtable->kill(actor);
    }
}
