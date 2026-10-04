#include <math/seadVectorCalcCtr.h>

namespace {
struct Vector3Data {
    nn::math::VEC3 components;
};
typedef char VectorStorageSizeCheck[sizeof(Vector3Data) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vector3Data, components) == 0 ? 1 : -1];

struct Vector3 : Vector3Data {
    Vector3 operator*(float scalar) const;
    Vector3& operator+=(const Vector3& other);
    Vector3& operator=(const Vector3& other) {
        static_cast<Vector3Data&>(*this) = other;
        return *this;
    }
};
typedef char VectorWrapperSizeCheck[sizeof(Vector3) == 12 ? 1 : -1];

struct Object {
    unsigned char padding0[0x7c];
    Vector3 first;
    unsigned char padding1[0x0c];
    Vector3 second;
};
}

extern "C" float fn_00287AD0(float);
extern "C" float fn_00287908(float);

namespace {
Vector3 Vector3::operator*(float scalar) const {
    Vector3 result;
    sead::Vector3CalcCtr<float>::multScalar(result.components, components, scalar);
    return result;
}

Vector3& Vector3::operator+=(const Vector3& other) {
    sead::Vector3CalcCtr<float>::add(components, components, other.components);
    return *this;
}
}

extern "C" void fn_0032787C(Object* self, Vector3* out, float t) {
    if (t <= 0.0f) {
        *out = self->second;
        return;
    }
    if (t >= 1.0f) {
        out->components.x = -self->first.components.x;
        out->components.y = -self->first.components.y;
        out->components.z = -self->first.components.z;
        return;
    }
    float angle = t * 1.57079632679489661923f;
    *out = self->second * fn_00287AD0(angle);
    *out += self->first * -fn_00287908(angle);
}
