namespace {
struct Vec3 {
    float x, y, z;
    Vec3(const float& a, const float& b, const float& c, int) { set(a, b, c); }
    void set(const float& a, const float& b, const float& c) { x = a; y = b; z = c; }
};

class Actor {
public:
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void kill() = 0;
    char padding[0x5c];
    void* owner;
};
}

extern "C" {
extern bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
extern void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
extern void fn_0026F770(Actor*, float);
extern float fn_00279114(float, float);
extern void fn_00279AC0(Actor*, const Vec3&);
extern void _ZN2al9onCollideEPNS_9LiveActorE(Actor*);
extern void _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(const Actor*, const char*);
extern bool _ZN2al6isStepEPNS_9IUseNerveEi(Actor*, int);
extern bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
extern bool fn_00279ED4(Actor*, int);
extern void fn_00279e8c(Actor*, float);
extern void fn_00279e5c(Actor*, float);
extern void _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(const Actor*);
extern void _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(const Actor*);
extern void fn_00261FFC(void*, Actor*);
extern const char dat_003C05AC[];
extern const char dat_003C05A4[];
extern const char dat_003C0644[];
extern const char dat_003C063C[];
}

namespace {
inline void adjust(Actor* actor, float speed, float friction) {
    fn_00279e8c(actor, speed);
    fn_00279e5c(actor, friction);
}
}

#define DEFINE_BODY(NAME, ACTION, REACTION) \
extern "C" void NAME(Actor* actor) { \
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) { \
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, ACTION); \
        fn_0026F770(actor, 35.0f); \
        const Vec3& velocity = Vec3(0.0f, fn_00279114(-3.0f, 3.0f), 0.0f, 0); \
        fn_00279AC0(actor, velocity); \
        _ZN2al9onCollideEPNS_9LiveActorE(actor); \
        _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(actor, REACTION); \
    } \
    if (_ZN2al6isStepEPNS_9IUseNerveEi(actor, 3)) \
        fn_0026F770(actor, 35.0f); \
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, 3)) { \
        if (fn_00279ED4(actor, 0)) \
            adjust(actor, 3.5f, 0.98f); \
        else \
            adjust(actor, 3.5f, 0.98f); \
    } \
    bool dead = false; \
    if (fn_00279ED4(actor, 0)) { \
        _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(actor); \
        dead = true; \
    } else if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, 40)) { \
        dead = true; \
    } \
    if (dead) { \
        _ZN2al21startHitReactionDeathEPKNS_9LiveActorE(actor); \
        fn_00261FFC(actor->owner, actor); \
        actor->kill(); \
    } \
}

DEFINE_BODY(fn_001572BC, dat_003C05AC, dat_003C05A4)
DEFINE_BODY(fn_00157794, dat_003C0644, dat_003C063C)
