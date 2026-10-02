#include "Player/SwimControlUpdateObserved.h"
#include <math/seadQuat.h>
#include <math.h>
#include <stddef.h>

namespace nn { namespace math { struct VEC3; } }
class PlayerGraphConfiguration;

extern "C" {
PlayerGraphConfiguration* fn_0026E1DC();
void fn_00270844(sead::Quatf*, const sead::Vector3f*, float);
void fn_00258A54(sead::Vector3f*, const sead::Quatf*);
void fn_00279ABC(sead::Vector3f*);
void fn_00173F4C(swim174024::ActionView*);
void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&);
void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(nn::math::VEC3&, const nn::math::VEC3&, float);
void _ZN4sead14Vector3CalcCtrIfE5crossERN2nn4math4VEC3ERKS4_S7_(nn::math::VEC3&, const nn::math::VEC3&, const nn::math::VEC3&);
}

namespace swim174024
{
static_assert_(offsetof(PropertyView, front) == 0xC);
static_assert_(offsetof(PropertyView, up) == 0x18);
static_assert_(offsetof(PropertyView, velocity) == 0x24);
static_assert_(offsetof(PropertyView, turnAxis) == 0x6C);
static_assert_(offsetof(ContextView, input) == 0x14);
static_assert_(offsetof(ActionView, paddleFrames) == 8);
static_assert_(sizeof(ActionView) == 0xC);
static_assert_(offsetof(InputSlots, paddle4C) == 0x4C);
static_assert_(offsetof(AnimatorSlots, name34) == 0x34);
static_assert_(offsetof(ConfigurationSlots, lateralDamping378) == 0x378);

inline nn::math::VEC3& raw(Vec3& a) { return reinterpret_cast<nn::math::VEC3&>(a); }
inline const nn::math::VEC3& raw(const Vec3& a) { return reinterpret_cast<const nn::math::VEC3&>(a); }
inline Vec3 scaled(const Vec3& a, float s)
{
    Vec3 r;
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(raw(r), raw(a), s);
    return r;
}
inline void scale(Vec3& a, float s)
{
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(raw(a), raw(a), s);
}
inline void add(Vec3& a, const Vec3& b)
{
    _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(raw(a), raw(a), raw(b));
}
inline void sub(Vec3& a, const Vec3& b)
{
    _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(raw(a), raw(a), raw(b));
}
inline float dot(const Vec3& a, const Vec3& b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}
inline float length(const Vec3& a) { return sqrtf(dot(a,a)); }
inline void setLength(Vec3& a, float n)
{
    float old = length(a);
    if (old > 0.0f)
        a *= n / old;
}
inline bool outsideDeadzone(float value)
{
    union { float f; int i; } absolute;
    absolute.f = value > 0.0f ? value : -value;
    return absolute.i > 0x3DCCCCCD;
}
// VNMUL negates the rounded product, including its NaN sign. Express the
// sign operation explicitly because ARMCC folds unary minus into a negative
// constant under the established fast-float flags, changing NaN propagation.
inline float negativeProduct(float a, float b)
{
    union { float f; unsigned int u; } product;
    product.f = a * b;
    product.u ^= 0x80000000u;
    return product.f;
}
inline float removeDeadzone(float value)
{
    return (value >= 0.0f ? value - 0.1f : value + 0.1f) * (1.0f / 0.9f);
}
}

#ifdef NON_MATCHING
extern "C" void fn_00174024(swim174024::ActionView* self)
{
    using namespace swim174024;
    ConfigurationView* config = reinterpret_cast<ConfigurationView*>(fn_0026E1DC());
    if (outsideDeadzone(self->context->input->slots->axis14(self->context->input)))
    {
        float amount = removeDeadzone(self->context->input->slots->axis14(self->context->input));
        const Vec3* axis = &self->context->property->turnAxis;
        float rate = config->slots->horizontal354(config);
        sead::Quatf rotation;
        fn_00270844(&rotation, axis, negativeProduct(rate * amount, 0.01745329238474369f));
        fn_00258A54(&self->context->property->front, &rotation);
        fn_00279ABC(&self->context->property->front);
        fn_00258A54(&self->context->property->up, &rotation);
        fn_00279ABC(&self->context->property->up);
    }
    if (outsideDeadzone(self->context->input->slots->axis18(self->context->input)))
    {
        float amount = removeDeadzone(self->context->input->slots->axis18(self->context->input));
        Vec3 axis;
        _ZN4sead14Vector3CalcCtrIfE5crossERN2nn4math4VEC3ERKS4_S7_(raw(axis), raw(self->context->property->up), raw(self->context->property->front));
        fn_00279ABC(&axis);
        float rate = config->slots->vertical358(config);
        sead::Quatf rotation;
        fn_00270844(&rotation, &axis, (rate * amount) * 0.01745329238474369f);
        fn_00258A54(&self->context->property->front, &rotation);
        fn_00279ABC(&self->context->property->front);
        fn_00258A54(&self->context->property->up, &rotation);
        fn_00279ABC(&self->context->property->up);
        if (self->context->property->front.y >= 0.0f)
        {
            self->context->property->front.y = 0.0f;
            fn_00279ABC(&self->context->property->front);
            PropertyView* property = self->context->property;
            property->up = Vec3(0.0f, dot(property->up, property->turnAxis) >= 0.0f ? 1.0f : -1.0f, 0.0f);
        }
    }
    if (self->context->input->slots->paddle4C(self->context->input))
    {
        self->paddleFrames = config->slots->paddleFrames364(config);
        self->context->animator->slots->name34(self->context->animator, sead::SafeString("SwimPaddle"));
        Vec3& velocity = self->context->property->velocity;
        const Vec3& up = self->context->property->up;
        add(velocity, scaled(up, config->slots->paddleAcceleration35C(config)));
        if (length(self->context->property->velocity) > config->slots->paddleMaximum360(config))
        {
            PropertyView* property = self->context->property;
            setLength(property->velocity, config->slots->paddleMaximum360(config));
        }
    }
    if (self->context->animator->slots->name34(self->context->animator, sead::SafeString("SwimPaddle")) &&
        self->context->animator->slots->query30(self->context->animator))
        self->context->animator->slots->operation2C(self->context->animator);

    if (self->paddleFrames != 0)
    {
        --self->paddleFrames;
        fn_00173F4C(self);
    }
    else if (self->context->input->slots->swim50(self->context->input))
    {
        if (!self->context->animator->slots->name18(self->context->animator, sead::SafeString("Swim")))
            self->context->animator->slots->name08(self->context->animator, sead::SafeString("Swim"));
        fn_00173F4C(self);
        if (length(self->context->property->velocity) > config->slots->swimSpeed36C(config))
        {
            Vec3& velocity = self->context->property->velocity;
            scale(velocity, config->slots->swimDamping370(config));
            if (length(self->context->property->velocity) < config->slots->swimSpeed36C(config))
            {
                PropertyView* property = self->context->property;
                setLength(property->velocity, config->slots->swimSpeed36C(config));
            }
        }
        else
        {
            Vec3& velocity = self->context->property->velocity;
            const Vec3& up = self->context->property->up;
            add(velocity, scaled(up, config->slots->swimAcceleration368(config)));
            if (config->slots->swimSpeed36C(config) < length(self->context->property->velocity))
            {
                PropertyView* property = self->context->property;
                setLength(property->velocity, config->slots->swimSpeed36C(config));
            }
        }
    }
    else
    {
        Vec3& velocity = self->context->property->velocity;
        scale(velocity, config->slots->idleDamping374(config));
        if (!self->context->animator->slots->name18(self->context->animator, sead::SafeString("SwimWait")))
            self->context->animator->slots->name08(self->context->animator, sead::SafeString("SwimWait"));
    }
    float vertical = dot(self->context->property->up, self->context->property->velocity);
    sub(self->context->property->velocity, scaled(self->context->property->up, vertical));
    Vec3& velocity = self->context->property->velocity;
    scale(velocity, config->slots->lateralDamping378(config));
    add(self->context->property->velocity, scaled(self->context->property->up, vertical));
}
#endif
