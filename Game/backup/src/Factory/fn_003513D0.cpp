namespace {
struct Vec3 { float x, y, z; };
struct Actor {
    void (**vtable)(Actor*);
    char padding04[8];
    Vec3 position;
    char padding18[0x5c];
    void* field74;
    char padding78[8];
    void* field80;
    void* field84;
    char padding88[0x10];
    bool field98;
};
struct Context { Actor* actor; };
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al9onCollideEPNS_9LiveActorE(Actor*);
bool _ZN2al16updateNerveStateEPNS_9IUseNerveE(Actor*);
void fn_0021419C(void*);
void fn_00268778(Actor*);
void fn_002CD428(Actor*, void*);
void _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(const Actor*);
bool fn_00268718(void*);
void fn_0027EBFC(Vec3*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const void*);
extern char dat_003F20A8;

void fn_003513D0(void*, Context* context) {
    Actor* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al9onCollideEPNS_9LiveActorE(actor);
    if (_ZN2al16updateNerveStateEPNS_9IUseNerveE(actor)) {
        fn_0021419C(actor->field80);
        if (actor->field98)
            fn_00268778(actor);
        else
            fn_002CD428(actor, actor->field74);
        _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(actor);
        if (!actor->field84 || fn_00268718(actor->field84))
            fn_0027EBFC(&actor->position);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F20A8);
        actor->vtable[5](actor);
    }
}
}
