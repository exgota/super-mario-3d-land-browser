#include <math/seadVectorCalcCtr.h>

namespace {
struct Vec3 { nn::math::VEC3 components; };
typedef char VectorStorageSizeCheck[sizeof(Vec3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vec3, components) == 0 ? 1 : -1];
struct Quat { float x, y, z, w; };
struct EffectController;
struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void kill() = 0;
    unsigned char reserved04[0x6c];
    Vec3 position;
    Vec3 direction;
    unsigned char reserved88[0x0c];
    EffectController* controller;
};
struct Spine { Actor* actor; };

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
void fn_0027A6D4(Actor*, const char*, const Vec3*);
void fn_00279AC0(Actor*, const Vec3&);
void fn_00279e5c(Actor*, float);
bool _ZN2al6isStepEPNS_9IUseNerveEi(Actor*, int);
const Quat& _ZN2al7getQuatEPKNS_9LiveActorE(const Actor*);
void fn_00266320(EffectController*, Actor*, const Vec3*, const Quat*);
void fn_00217798(EffectController*, Actor*);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void _ZN2al25startHitReactionDisappearEPKNS_9LiveActorE(const Actor*);
extern const char dat_003BCF54[];
extern const char dat_003BCF4C[];
}
}

extern "C" void fn_003449B4(const void*, Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BCF54);
        fn_0027A6D4(actor, dat_003BCF4C, &actor->position);
    }
    Vec3 velocity;
    Vec3 scaled;
    sead::Vector3CalcCtr<float>::multScalar(scaled.components, actor->direction.components, 2.0f);
    velocity = scaled;
    fn_00279AC0(actor, velocity);
    fn_00279e5c(actor, 0.99f);
    if (_ZN2al6isStepEPNS_9IUseNerveEi(actor, 3)) {
        const Quat& quat = _ZN2al7getQuatEPKNS_9LiveActorE(actor);
        fn_00266320(actor->controller, actor, &actor->position, &quat);
        fn_00217798(actor->controller, actor);
    }
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, 90)) {
        _ZN2al25startHitReactionDisappearEPKNS_9LiveActorE(actor);
        actor->kill();
    }
}
