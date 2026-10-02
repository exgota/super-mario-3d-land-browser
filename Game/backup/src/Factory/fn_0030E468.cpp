namespace {
struct Vector3 {
    float x, y, z;
    Vector3 operator*(float scale) const;
};
struct Actor {
    char prefix[8];
    void* effectInterface;
};
struct SafeString {
    const void* vtable;
    const char* text;
    SafeString(const char* name);
};
}

extern "C" {
bool fn_00279ED4(const Actor*, int);
bool _ZN2al13isGreaterStepEPKNS_9IUseNerveEi(const Actor*, int);
bool fn_0027DBC8(const Actor*, float);
const Vector3& _ZN2al11getVelocityEPKNS_9LiveActorE(const Actor*);
void _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(const Actor*);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vector3&, const Vector3&, float);
void _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(Actor*, const Vector3&);
void fn_0027109C(void*, const SafeString&);
extern const char dat_003BA600[];
extern const void* _ZTVN4sead14SafeStringBaseIcEE[];
}

namespace {
SafeString::SafeString(const char* name) {
    text = name;
    vtable = _ZTVN4sead14SafeStringBaseIcEE + 2;
}

Vector3 Vector3::operator*(float scale) const {
    Vector3 result;
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(result, *this, scale);
    return result;
}
}

extern "C" bool fn_0030E468(Actor* actor) {
    if (fn_00279ED4(actor, 0)) {
        if (_ZN2al13isGreaterStepEPKNS_9IUseNerveEi(actor, 9) && fn_0027DBC8(actor, 4.0f))
            return true;
        if (_ZN2al11getVelocityEPKNS_9LiveActorE(actor).y < 0.0f) {
            _ZN2al24startHitReactionOnGroundEPKNS_9LiveActorE(actor);
            Vector3 velocity = _ZN2al11getVelocityEPKNS_9LiveActorE(actor);
            velocity.y = -velocity.y;
            _ZN2al11setVelocityEPNS_9LiveActorERKN4sead7Vector3IfEE(actor, velocity * 0.8f);
            fn_0027109C(&actor->effectInterface, SafeString(dat_003BA600));
        }
    }
    return false;
}
