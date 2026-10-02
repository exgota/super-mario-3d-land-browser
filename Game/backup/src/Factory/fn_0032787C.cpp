namespace {
struct Vector3Data {
    float x, y, z;
};

struct Vector3 : Vector3Data {
    Vector3 operator*(float scalar) const;
    Vector3& operator+=(const Vector3& other);
    Vector3& operator=(const Vector3& other) {
        static_cast<Vector3Data&>(*this) = other;
        return *this;
    }
};

struct Object {
    unsigned char padding0[0x7c];
    Vector3 first;
    unsigned char padding1[0x0c];
    Vector3 second;
};
}

extern "C" float fn_00287AD0(float);
extern "C" float fn_00287908(float);
extern "C" void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(
    Vector3&, const Vector3&, float);
extern "C" void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(
    Vector3&, const Vector3&, const Vector3&);

namespace {
Vector3 Vector3::operator*(float scalar) const {
    Vector3 result;
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(result, *this, scalar);
    return result;
}

Vector3& Vector3::operator+=(const Vector3& other) {
    _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(*this, *this, other);
    return *this;
}
}

extern "C" void fn_0032787C(Object* self, Vector3* out, float t) {
    if (t <= 0.0f) {
        *out = self->second;
        return;
    }
    if (t >= 1.0f) {
        out->x = -self->first.x;
        out->y = -self->first.y;
        out->z = -self->first.z;
        return;
    }
    float angle = t * 1.57079632679489661923f;
    *out = self->second * fn_00287AD0(angle);
    *out += self->first * -fn_00287908(angle);
}
