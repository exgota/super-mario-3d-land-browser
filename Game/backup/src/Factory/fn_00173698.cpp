#include <math/seadVectorCalcCtr.h>

#include "Math/alVectorNormalizationImports.h"

namespace {
struct Vec3 {
    nn::math::VEC3 components;
    Vec3& operator=(const Vec3& rhs)
    {
        components = rhs.components;
        return *this;
    }
    Vec3 operator*(float) const;
    Vec3& operator+=(const Vec3&);
};
struct Motion {
    char unknown00[12];
    Vec3 direction;
    Vec3 position;
    Vec3 velocity;
    char unknown30[60];
    Vec3 acceleration;
};
typedef char Vector3WrapperSizeCheck[sizeof(Vec3) == 12 ? 1 : -1];
typedef char MotionFieldOffsetCheck[
    offsetof(Motion, direction) == 12 && offsetof(Motion, position) == 24 &&
    offsetof(Motion, velocity) == 36 && offsetof(Motion, acceleration) == 108 &&
    sizeof(Motion) == 120 ? 1 : -1];
struct MotionState {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual bool active() = 0;
};
struct MotionControl {
    virtual void slot0() = 0;
    virtual void slot1() = 0;
    virtual void slot2() = 0;
    virtual void slot3() = 0;
    virtual void slot4() = 0;
    virtual void slot5() = 0;
    virtual void slot6() = 0;
    virtual void update() = 0;
    virtual void slot8() = 0;
    virtual bool stopVertical() = 0;
};
}

extern "C" void fn_0026E1DC(Motion*);
extern "C" void fn_0027306C(Vec3*, const Vec3*, const Vec3*);
extern "C" bool fn_0026F71C(const Vec3*, float);

namespace {
inline Vec3 Vec3::operator*(float scalar) const
{
    Vec3 result;
    sead::Vector3CalcCtr<float>::multScalar(result.components, components, scalar);
    return result;
}
inline Vec3& Vec3::operator+=(const Vec3& rhs)
{
    sead::Vector3CalcCtr<float>::add(components, components, rhs.components);
    return *this;
}
}

extern "C" void fn_00173698(Motion* self, MotionState* state, MotionControl* control, float dt)
{
    fn_0026E1DC(self);
    {
        Vec3 direction;
        fn_0027306C(&direction, &self->position, &self->velocity);
        if (!fn_0026F71C(&direction, 0.001f)) {
            fn_00279ABC(direction.components);
            self->direction = direction;
        }
    }
    if (!state->active()) {
        self->velocity.components.x = 0.0f;
        self->velocity.components.y = 0.0f;
        self->velocity.components.z = 0.0f;
    } else if (control->stopVertical()) {
        self->velocity.components.y = 0.0f;
    }
    Vec3& velocity = self->velocity;
    velocity.operator+=(self->acceleration.operator*(dt));
    control->update();
}
