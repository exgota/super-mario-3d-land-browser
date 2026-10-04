namespace {

struct Vec3 {
    float x;
    float y;
    float z;

    Vec3() {}

    Vec3(const Vec3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
    }

    Vec3& operator=(const Vec3& rhs)
    {
        x = rhs.x;
        y = rhs.y;
        z = rhs.z;
        return *this;
    }

    void setSub(const Vec3&, const Vec3&);
    void setScale(const Vec3&, float);
    Vec3 operator-(const Vec3&) const;
};

struct CubicCurve {
    Vec3 c0;
    Vec3 c1;
    Vec3 c2;
    Vec3 c3;
    float length;
};

}

extern "C" void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(
    Vec3&, const Vec3&, const Vec3&);
extern "C" void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(
    Vec3&, const Vec3&, float);
extern "C" float fn_00250B70(const CubicCurve*, int, float, float);

namespace {

inline void Vec3::setSub(const Vec3& lhs, const Vec3& rhs)
{
    _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(*this, lhs, rhs);
}

inline void Vec3::setScale(const Vec3& rhs, float scale)
{
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(*this, rhs, scale);
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
