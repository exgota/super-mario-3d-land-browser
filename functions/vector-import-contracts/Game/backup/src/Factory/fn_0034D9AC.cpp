#include <math/seadVectorCalcCtr.h>

namespace {
struct Actor;
struct Nerve;
struct Spine { Actor* actor; };
struct Vec3 {
    nn::math::VEC3 components;
    Vec3& operator*=(float scalar);
};
typedef char VectorStorageSizeCheck[sizeof(Vec3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vec3, components) == 0 ? 1 : -1];
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void _ZN2al11startActionEPNS_9LiveActorEPKc(Actor*, const char*);
bool _ZN2al11isActionEndEPKNS_9LiveActorE(const Actor*);
const Vec3& _ZN2al10getRailDirEPKNS_9LiveActorE(const Actor*);
bool _ZN2al10isLoopRailEPKNS_9LiveActorE(const Actor*);
void fn_0027A5DC(Vec3&, const Actor*);
bool fn_0027A724(const Vec3&, const Vec3&, float);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern const char dat_003BC238[];
extern const char dat_003BC248[];
extern const Nerve dat_003F2328;
extern const Nerve dat_003F232C;
}

extern "C" void fn_0034D9AC(const Nerve*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC238);
    if (_ZN2al11isActionEndEPKNS_9LiveActorE(actor)) {
        Vec3 direction = _ZN2al10getRailDirEPKNS_9LiveActorE(actor);
        if (!_ZN2al10isLoopRailEPKNS_9LiveActorE(actor))
            direction *= -1.0f;
        _ZN2al11startActionEPNS_9LiveActorEPKc(actor, dat_003BC248);
        Vec3 facing;
        fn_0027A5DC(facing, actor);
        if (fn_0027A724(direction, facing, 0.01f))
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F2328);
        else
            _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F232C);
    }
}

namespace {
inline Vec3& Vec3::operator*=(float scalar) {
    sead::Vector3CalcCtr<float>::multScalar(components, components, scalar);
    return *this;
}
}
