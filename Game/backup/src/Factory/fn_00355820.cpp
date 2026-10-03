#include <math/seadVectorCalcCtr.h>

namespace {
struct Vector3 {
    nn::math::VEC3 components;
    Vector3 operator-(const Vector3&) const;
    Vector3 operator*(float) const;
};
struct Actor {
    char padding0[0x70];
    float speed;
    char padding74[0x10];
    Vector3 destination;
};
typedef char VectorStorageSizeCheck[sizeof(Vector3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vector3, components) == 0 ? 1 : -1];
typedef char ActorVectorOffsetCheck[
    offsetof(Actor, speed) == 0x70 && offsetof(Actor, destination) == 0x84 ? 1 : -1];
struct Spine {
    Actor* actor;
};
struct Nerve {};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
const Vector3& _ZN2al8getTransEPKNS_9LiveActorE(const Actor*);
bool fn_0027D5C4(Vector3&);
void _ZN2al15setVelocityZeroEPNS_9LiveActorE(Actor*);
void fn_0027C0C8(Actor*, const Vector3&);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vector3&, const Vector3&, float);
void _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(Actor*, const Vector3&);
bool fn_00279ED4(const Actor*, int);
bool _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(const Actor*, int);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern const char dat_003BA760[];
extern const Nerve dat_003F1EA4;
}

namespace {
inline Vector3 Vector3::operator-(const Vector3& other) const {
    Vector3 result;
    sead::Vector3CalcCtr<float>::sub(result.components, components, other.components);
    return result;
}
inline Vector3 Vector3::operator*(float scale) const {
    Vector3 result;
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(result, *this, scale);
    return result;
}
}

extern "C" void fn_00355820(const Nerve*, Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor)) {
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BA760);
        Vector3 direction = actor->destination - _ZN2al8getTransEPKNS_9LiveActorE(actor);
        if (fn_0027D5C4(direction)) {
            _ZN2al15setVelocityZeroEPNS_9LiveActorE(actor);
        } else {
            fn_0027C0C8(actor, direction);
            _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(actor, direction * actor->speed);
        }
    }
    if (fn_00279ED4(actor, 0) && _ZN2al18isGreaterEqualStepEPKNS_9IUseNerveEi(actor, 10)) {
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F1EA4);
    }
}
