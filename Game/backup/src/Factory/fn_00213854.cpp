namespace {
struct LiveActor;
struct Vec3 {
    float x, y, z;
};
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
extern "C" void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vec3&, const Vec3&, float);

namespace {
inline Vec3 multiply(const Vec3& v, float scale) {
    Vec3 result;
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(result, v, scale);
    return result;
}
}

extern "C" bool fn_00213854(LiveActor* actor, float scale) {
    if (fn_00279ED4(actor, 0)) {
        if (_ZN2al13isGreaterStepEPKNS_9IUseNerveEi(actor, 9)) {
            const Vec3& velocity = _ZN2al11getVelocityEPKNS_9LiveActorE(actor);
            FloatBits square;
            square.value = velocity.x * velocity.x + velocity.y * velocity.y + velocity.z * velocity.z;
            if (square.bits < 0x43c80000)
                return true;
        }
        if (_ZN2al11getVelocityEPKNS_9LiveActorE(actor).y < 0.0f) {
            _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(actor);
            Vec3 velocity = _ZN2al11getVelocityEPKNS_9LiveActorE(actor);
            velocity.y = -velocity.y;
            Vec3 result = multiply(velocity, scale);
            _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(actor, result);
        }
    }
    return false;
}
