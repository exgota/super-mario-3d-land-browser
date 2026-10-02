namespace {
struct Vec3;
extern "C" void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vec3&, const Vec3&, float);
extern "C" void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(Vec3&, const Vec3&, const Vec3&);
extern "C" void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(Vec3&, const Vec3&, const Vec3&);

struct Vec3 {
    float x, y, z;
    Vec3 operator*(float scalar) const {
        Vec3 result;
        _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(result, *this, scalar);
        return result;
    }
    void operator-=(const Vec3& other) {
        _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(*this, *this, other);
    }
    void operator+=(const Vec3& other) {
        _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(*this, *this, other);
    }
};
struct Body {
    char pad0[0x24];
    Vec3 velocity;
    char pad30[0x30];
    Vec3 normal;
};
struct State {
    char pad0[8];
    Body** body;
    bool flag;
};
struct Settings;
struct SettingsVtable {
    char pad0[0x394];
    float (*getScale)(Settings*);
    float (*getThreshold)(Settings*);
};
struct Settings {
    SettingsVtable* vtable;
};
extern "C" Settings* fn_0026E1DC();
extern "C" float fn_001735C4(float, float, float);
}

extern "C" void fn_00171BE0(State* self) {
    Body* body = *self->body;
    float scalar = body->velocity.x * body->normal.x
                 + body->velocity.y * body->normal.y
                 + body->velocity.z * body->normal.z;
    body->velocity -= body->normal * scalar;
    Settings* settings = fn_0026E1DC();
    Settings* other = fn_0026E1DC();
    scalar = fn_001735C4(scalar, 0.0f, other->vtable->getScale(other));
    body = *self->body;
    body->velocity += body->normal * scalar;
    if (settings->vtable->getThreshold(settings) > scalar)
        self->flag = true;
}
