#include <math/seadVectorCalcCtr.h>

namespace {
struct Vec3 {
    nn::math::VEC3 components;
    Vec3() {}
    Vec3(float a, float b, float c) { components.x = a; components.y = b; components.z = c; }
    Vec3& operator=(const Vec3& v) {
        float newY = v.components.y;
        float newX = v.components.x;
        float newZ = v.components.z;
        components.y = newY;
        components.x = newX;
        components.z = newZ;
        return *this;
    }
};
struct Actor;
struct Quat;
}
extern "C" {
extern const Vec3 _ZN4sead7Vector3IfE4zeroE;
const Vec3& _ZN2al10getGravityEPKNS_9LiveActorE(const Actor*);
const Vec3& fn_00337264(const Actor*, int);
float fn_002700E0(const Actor*);
const Vec3& _ZN2al11getVelocityEPKNS_9LiveActorE(const Actor*);
void fn_00270044(Vec3&, const Vec3&, const Vec3&, float);
Quat* _ZN2al10getQuatPtrEPNS_9LiveActorE(Actor*);
void fn_0026FFF8(Quat*, const Quat*, const Vec3&);
}
namespace {
inline Vec3 scaled(const Vec3& v, float scale) {
    Vec3 result;
    sead::Vector3CalcCtr<float>::multScalar(result.components, v.components, scale);
    return result;
}
}
extern "C" void fn_00213B74(Actor* actor, bool useAlternate) {
    Vec3 rotation = _ZN4sead7Vector3IfE4zeroE;
    const Vec3& gravity = _ZN2al10getGravityEPKNS_9LiveActorE(actor);
    Vec3 up(-gravity.components.x, -gravity.components.y, -gravity.components.z);
    if (useAlternate)
        up = fn_00337264(actor, 0);
    float rate = fn_002700E0(actor);
    Vec3 velocity = scaled(_ZN2al11getVelocityEPKNS_9LiveActorE(actor), 1.0f);
    fn_00270044(rotation, velocity, up, rate);
    Quat* quat = _ZN2al10getQuatPtrEPNS_9LiveActorE(actor);
    fn_0026FFF8(quat, quat, rotation);
}

namespace {
typedef char VectorStorageSizeCheck[sizeof(Vec3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vec3, components) == 0 ? 1 : -1];

}
