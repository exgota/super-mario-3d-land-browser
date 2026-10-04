#include <math/seadVectorCalcCtr.h>

namespace {

struct Vec3 {
    nn::math::VEC3 components;

    Vec3() {}

    Vec3(const Vec3& rhs)
    {
        components.x = rhs.components.x;
        components.y = rhs.components.y;
        components.z = rhs.components.z;
    }

    Vec3& operator=(const Vec3& rhs)
    {
        components.x = rhs.components.x;
        components.y = rhs.components.y;
        components.z = rhs.components.z;
        return *this;
    }

    void setSub(const Vec3&, const Vec3&);
    void setScale(const Vec3&, float);
    Vec3 operator-(const Vec3&) const;
};
typedef char VectorStorageSizeCheck[sizeof(Vec3) == 12 ? 1 : -1];
typedef char VectorStorageOffsetCheck[offsetof(Vec3, components) == 0 ? 1 : -1];

struct CubicCurve {
    Vec3 c0;
    Vec3 c1;
    Vec3 c2;
    Vec3 c3;
    float length;
};

}

extern "C" float fn_00250B70(const CubicCurve*, int, float, float);

namespace {

inline void Vec3::setSub(const Vec3& lhs, const Vec3& rhs)
{
    sead::Vector3CalcCtr<float>::sub(components, lhs.components, rhs.components);
}

inline void Vec3::setScale(const Vec3& rhs, float scale)
{
    sead::Vector3CalcCtr<float>::multScalar(components, rhs.components, scale);
}

inline Vec3 Vec3::operator-(const Vec3& rhs) const
{
    Vec3 result;
    result.setSub(*this, rhs);
    return result;
}

}

extern "C" void fn_001BD204(CubicCurve* curve, const Vec3* p0,
                            const Vec3* p1, const Vec3* p2, const Vec3* p3)
{
    Vec3 a = *p1 - *p0;
    Vec3 b = *p2 - *p1;
    Vec3 c = *p3 - *p2;
    Vec3 d = b - a;
    Vec3 e = c - b;
    Vec3 f = e - d;

    curve->c0 = *p0;
    curve->c1.setScale(a, 3.0f);
    curve->c2.setScale(d, 3.0f);
    curve->c3 = f;
    curve->length = fn_00250B70(curve, 10, 0.0f, 1.0f);
}
