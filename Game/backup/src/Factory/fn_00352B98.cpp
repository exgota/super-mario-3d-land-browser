namespace {
struct Vector3 { float x, y, z; };
struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void kill() = 0;
    unsigned char pad04[8];
    Vector3 position;
    unsigned char pad18[0x7c];
    Vector3 from;
    Vector3 to;
    unsigned char padAC[4];
    float scale;
    unsigned char padB4[4];
    int duration;
    Actor* parent;
    int moving;
};
struct Spine { Actor* actor; };
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
void fn_001C96B8(Actor*);
float fn_0027F3D0(Actor*, int);
void fn_00278740(Actor*, const Vector3*, const Vector3*, float);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Actor*, int);
void _ZN2al9hideModelEPNS_9LiveActorE(Actor*);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const void*);
void fn_0027889C(Actor*);
void fn_002788B8(Vector3*);
extern const char dat_003BEC14[];
extern const unsigned char dat_003F2A68;

void fn_00352B98(const void*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BEC14);
        fn_001C96B8(actor);
    }
    if (actor->moving) {
        fn_00278740(actor, &actor->from, &actor->to,
                   fn_0027F3D0(actor, actor->duration) * actor->scale);
    }
    if (_ZN2al6isStepEPNS_9IUseNerveEi(actor, actor->duration))
        _ZN2al9hideModelEPNS_9LiveActorE(actor);
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, actor->duration + 5)) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2A68);
        if (!actor->moving) {
            if (actor->parent)
                fn_0027889C(actor->parent);
            else
                fn_002788B8(&actor->position);
        }
        actor->kill();
    }
}
}
