#include <math/seadVectorCalcCtr.h>

namespace {

struct Vector3 {
    nn::math::VEC3 components;
};

struct Parameters {
    unsigned char padding[0x38];
    const int* duration;
    const float* drag;
    const float* recoil;
};

struct Actor {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void kill() = 0;
    unsigned char padding[0x64];
    Parameters* parameters;
};

struct NerveContext {
    Actor* actor;
};

}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(const Actor*, const char*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
void _ZN2al9hideModelEPNS_9LiveActorE(Actor*);
void fn_00277AF0(Actor*);
const Vector3& _ZN2al11getVelocityEPKNS_9LiveActorE(const Actor*);
void fn_0027D5C4(Vector3*);
void fn_00279AC0(Actor*, const Vector3&);
void fn_00279e5c(Actor*, float);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void _ZN2al15setVelocityZeroEPNS_9LiveActorE(Actor*);
extern const char dat_003A8AE0[];
extern const char dat_003A8AB8[];
}

namespace {

inline Vector3 operator*(const Vector3& vector, float scalar) {
    Vector3 result;
    sead::Vector3CalcCtr<float>::multScalar(result.components, vector.components, scalar);
    return result;
}

}

extern "C" void fn_003600F8(void*, const NerveContext* context) {
    Actor* actor = context->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al16startHitReactionEPKNS_9LiveActorEPKc(actor, dat_003A8AE0);
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003A8AB8);
        _ZN2al9hideModelEPNS_9LiveActorE(actor);
        fn_00277AF0(actor);
        Vector3 velocity = _ZN2al11getVelocityEPKNS_9LiveActorE(actor);
        fn_0027D5C4(&velocity);
        fn_00279AC0(actor, velocity * -(*actor->parameters->recoil));
    }
    fn_00279e5c(actor, *actor->parameters->drag);
    if (_ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, *actor->parameters->duration)) {
        _ZN2al15setVelocityZeroEPNS_9LiveActorE(actor);
        actor->kill();
    }
}

namespace {
typedef char VectorStorageSizeCheck[sizeof(Vector3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vector3, components) == 0 ? 1 : -1];

}
