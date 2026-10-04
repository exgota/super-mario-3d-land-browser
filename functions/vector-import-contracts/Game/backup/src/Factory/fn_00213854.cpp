#include <math/seadVectorCalcCtr.h>

namespace {
struct LiveActor;
struct Vec3 {
    nn::math::VEC3 components;
};
typedef char VectorStorageSizeCheck[sizeof(Vec3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vec3, components) == 0 ? 1 : -1];
union FloatBits {
    float value;
    int bits;
};
}

extern "C" bool fn_00279ED4(const LiveActor*, int);
extern "C" bool _ZN2al13isGreaterStepEPKNS_9IUseNerveEi(const LiveActor*, int);
extern "C" const Vec3& _ZN2al11getVelocityEPKNS_9LiveActorE(const LiveActor*);
extern "C" void _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(const LiveActor*);
extern "C" void _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(LiveActor*, const Vec3&);

namespace {
inline Vec3 multiply(const Vec3& v, float scale) {
    Vec3 result;
    sead::Vector3CalcCtr<float>::multScalar(result.components, v.components, scale);
    return result;
}
}

extern "C" bool fn_00213854(LiveActor* actor, float scale) {
    if (fn_00279ED4(actor, 0)) {
        if (_ZN2al13isGreaterStepEPKNS_9IUseNerveEi(actor, 9)) {
            const Vec3& velocity = _ZN2al11getVelocityEPKNS_9LiveActorE(actor);
            FloatBits square;
            square.value = velocity.components.x * velocity.components.x + velocity.components.y * velocity.components.y + velocity.components.z * velocity.components.z;
            if (square.bits < 0x43c80000)
                return true;
        }
        if (_ZN2al11getVelocityEPKNS_9LiveActorE(actor).components.y < 0.0f) {
            _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(actor);
            Vec3 velocity = _ZN2al11getVelocityEPKNS_9LiveActorE(actor);
            velocity.components.y = -velocity.components.y;
            Vec3 result = multiply(velocity, scale);
            _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(actor, result);
        }
    }
    return false;
}
