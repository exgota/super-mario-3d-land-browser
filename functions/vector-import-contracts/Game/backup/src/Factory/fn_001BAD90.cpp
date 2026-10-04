#include <math.h>
#include <math/seadVectorCalcCtr.h>

namespace {
struct Vec3 { nn::math::VEC3 components; };
struct Body {
    char pad0[0x24];
    Vec3 position;
    char pad30[0x3c];
    Vec3 target;
};
typedef char VectorStorageSizeCheck[sizeof(Vec3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vec3, components) == 0 ? 1 : -1];
typedef char BodyVectorOffsetCheck[
    offsetof(Body, position) == 0x24 && offsetof(Body, target) == 0x6C ? 1 : -1];
struct Holder { Body* body; };
struct State { void* vtable; Holder* holder; };
struct Limit;
struct LimitVtable {
    char pad0[0x38];
    float (*value)(Limit*);
};
struct Limit { LimitVtable* vtable; };
}

extern "C" void fn_0027306C(Vec3*, const Vec3*, const Vec3*);
extern "C" Limit* fn_0026E1DC();
extern "C" void fn_00173790(State*);

extern "C" void fn_001BAD90(State* self) {
    Vec3 delta;
    Body* body = self->holder->body;
    const Vec3* positionInput = &body->position;
    const Vec3* targetInput = &body->target;
    fn_0027306C(&delta, targetInput, positionInput);
    Limit* limit = fn_0026E1DC();
    float length = sqrtf(delta.components.x * delta.components.x + delta.components.y * delta.components.y + delta.components.z * delta.components.z);
    float allowed = limit->vtable->value(limit);
    if (length > allowed) {
        Vec3& position = self->holder->body->position;
        sead::Vector3CalcCtr<float>::sub(position.components, position.components, delta.components);
        limit = fn_0026E1DC();
        float maxLength = limit->vtable->value(limit);
        float magnitude = sqrtf(delta.components.x * delta.components.x + delta.components.y * delta.components.y + delta.components.z * delta.components.z);
        if (magnitude > 0.0f) {
            float scale = maxLength / magnitude;
            delta.components.x *= scale;
            delta.components.y *= scale;
            delta.components.z *= scale;
        }
        Vec3& current = self->holder->body->position;
        sead::Vector3CalcCtr<float>::add(current.components, current.components, delta.components);
    }
    fn_00173790(self);
}
