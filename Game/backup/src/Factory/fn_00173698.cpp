namespace {
struct VecStorage {
    float x, y, z;
};
struct Vec3 : VecStorage {
    Vec3& operator=(const Vec3& rhs)
    {
        static_cast<VecStorage&>(*this) = rhs;
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
extern "C" void fn_00279ABC(Vec3*);
extern "C" void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(Vec3&, const Vec3&, float);
extern "C" void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(Vec3&, const Vec3&, const Vec3&);

namespace {
inline Vec3 Vec3::operator*(float scalar) const
{
    Vec3 result;
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(result, *this, scalar);
    return result;
}
inline Vec3& Vec3::operator+=(const Vec3& rhs)
{
    _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(*this, *this, rhs);
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
            fn_00279ABC(&direction);
            self->direction = direction;
        }
    }
    if (!state->active()) {
        self->velocity.x = 0.0f;
        self->velocity.y = 0.0f;
        self->velocity.z = 0.0f;
    } else if (control->stopVertical()) {
        self->velocity.y = 0.0f;
    }
    Vec3& velocity = self->velocity;
    velocity.operator+=(self->acceleration.operator*(dt));
    control->update();
}
