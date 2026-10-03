namespace {
struct Vec3 {
    float x, y, z;
    Vec3() {}
    Vec3(float a, float b, float c) : x(a), y(b), z(c) {}
    Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) {}
};
struct VectorWork {
    Vec3 velocity;
    Vec3 negative;
    Vec3 scaled;
    VectorWork(const Vec3& direction)
        : negative(-direction.x, -direction.y, -direction.z) {}
};
struct Actor {
    char reserved[0x68];
    float** acceleration;
    char reserved6c[8];
    Vec3 direction;
};
struct Spine {
    Actor* actor;
};
struct Nerve {};
}

extern "C" {
bool _ZN2al11isFirstStepEPKNS_9IUseNerveE(const Actor*);
void fn_0025E50C(Actor*, float);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vec3&, const Vec3&, float);
void fn_00279AC0(Actor*, const Vec3&);
Vec3* _ZN2al11getVelocityEPKNS_9LiveActorE(const Actor*);
bool fn_0026F71C(const Vec3&, float);
bool fn_0026F924(const Vec3&, const Vec3&, float);
void _ZN2al15setVelocityZeroEPNS_9LiveActorE(Actor*);
void _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(Actor*, const Nerve*);
extern Nerve dat_003F3474;
}

extern "C" void fn_00360DC0(const Nerve*, const Spine* spine) {
    Actor* actor = spine->actor;
    if (_ZN2al11isFirstStepEPKNS_9IUseNerveE(actor))
        fn_0025E50C(actor, 85.0f);
    float scalar = **actor->acceleration;
    VectorWork work(actor->direction);
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(work.scaled, work.negative, scalar);
    work.velocity = work.scaled;
    fn_00279AC0(actor, work.velocity);
    if (fn_0026F71C(*_ZN2al11getVelocityEPKNS_9LiveActorE(actor), 0.001f)
        || fn_0026F924(*_ZN2al11getVelocityEPKNS_9LiveActorE(actor), actor->direction, 0.01f)) {
        _ZN2al15setVelocityZeroEPNS_9LiveActorE(actor);
        _ZN2al8setNerveEPNS_9IUseNerveEPKNS_5NerveE(actor, &dat_003F3474);
    }
}
