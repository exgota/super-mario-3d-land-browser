extern "C" const unsigned int _ZTVN4sead14SafeStringBaseIcEE[];

namespace {
struct Actor;
struct ActionInterface { unsigned int reserved; };
struct DeathContext;
struct ActorVtable {
    void (*reserved[5])();
    void (*kill)(Actor*);
};
struct Actor {
    ActorVtable* vtable;
    unsigned int reserved04;
    ActionInterface action;
    unsigned char reserved0c[0x54];
    DeathContext* deathContext;
};
struct Spine { Actor* actor; };
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* value) : text(value) {
        vtable = _ZTVN4sead14SafeStringBaseIcEE + 2;
    }
};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al15setVelocityZeroEPNS_9LiveActorE(Actor*);
void fn_00279FA0(Actor*, float);
void fn_0027109C(ActionInterface*, const SafeString&);
void _ZN2al9onCollideEPNS_9LiveActorE(Actor*);
bool fn_00279ED4(const Actor*, unsigned int);
void fn_00279e8c(Actor*, float);
void fn_00279e5c(Actor*, float);
void _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(const Actor*);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(const Actor*);
void fn_00279D54(DeathContext*, Actor*);
void fn_0015E854(DeathContext*, Actor*);
extern const char dat_003BB8D4[];
extern const char dat_003BBA0C[];
}

extern "C" void fn_0034851C(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al15setVelocityZeroEPNS_9LiveActorE(actor);
        fn_00279FA0(actor, 20.0f);
        fn_0027109C(&actor->action, SafeString(dat_003BB8D4));
        _ZN2al9onCollideEPNS_9LiveActorE(actor);
    }
    if (fn_00279ED4(actor, 0)) {
        fn_00279e8c(actor, 3.5f);
        fn_00279e5c(actor, 0.98f);
    } else {
        fn_00279e8c(actor, 3.5f);
        fn_00279e5c(actor, 0.98f);
    }
    bool end = false;
    if (fn_00279ED4(actor, 0)) {
        _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(actor);
        end = true;
    } else if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, 20)) {
        end = true;
    }
    if (end) {
        _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(actor);
        fn_00279D54(actor->deathContext, actor);
        actor->vtable->kill(actor);
    }
}

extern "C" void fn_00359448(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al15setVelocityZeroEPNS_9LiveActorE(actor);
        fn_00279FA0(actor, 20.0f);
        fn_0027109C(&actor->action, SafeString(dat_003BBA0C));
        _ZN2al9onCollideEPNS_9LiveActorE(actor);
    }
    if (fn_00279ED4(actor, 0)) {
        fn_00279e8c(actor, 3.5f);
        fn_00279e5c(actor, 0.98f);
    } else {
        fn_00279e8c(actor, 3.5f);
        fn_00279e5c(actor, 0.98f);
    }
    bool end = false;
    if (fn_00279ED4(actor, 0)) {
        _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(actor);
        end = true;
    } else if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, 20)) {
        end = true;
    }
    if (end) {
        _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(actor);
        fn_0015E854(actor->deathContext, actor);
        actor->vtable->kill(actor);
    }
}
