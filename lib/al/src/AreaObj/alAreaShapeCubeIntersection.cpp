#include <AreaObj/alAreaShapeCube.h>

// Dump-grounded ABI imports. Vector3f and the retail VEC3 parameters are
// three contiguous floats; the helpers read all operands before writing out.
extern "C" {
void _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(
    sead::Vector3f*, const sead::Vector3f*, const sead::Vector3f*);
void _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(
    sead::Vector3f*, const sead::Vector3f*, const sead::Vector3f*);
void _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(
    sead::Vector3f*, const sead::Vector3f*, float);
bool fn_00337488(const al::AreaShape*, sead::Vector3f*, const sead::Vector3f&);
}

namespace {
inline sead::Vector3f subtract(const sead::Vector3f& a, const sead::Vector3f& b)
{
    sead::Vector3f result;
    _ZN4sead14Vector3CalcCtrIfE3subERN2nn4math4VEC3ERKS4_S7_(&result, &a, &b);
    return result;
}
inline sead::Vector3f multiply(const sead::Vector3f& a, float scalar)
{
    sead::Vector3f result;
    _ZN4sead14Vector3CalcCtrIfE10multScalarERN2nn4math4VEC3ERKS4_f(&result, &a, scalar);
    return result;
}
inline sead::Vector3f add(const sead::Vector3f& a, const sead::Vector3f& b)
{
    sead::Vector3f result;
    _ZN4sead14Vector3CalcCtrIfE3addERN2nn4math4VEC3ERKS4_S7_(&result, &a, &b);
    return result;
}
}

// NonMatching: original-callee replay agrees, but the full compiled interval
// is 2,232 bytes versus 2,236 in retail. No exact credit.
#ifdef NON_MATCHING
// 0x0033091C..0x003311D8: third Cube virtual slot. The semantic method
// name is unproven. The constructor and both Cube methods establish flag +20.
extern "C" bool fn_0033091C(const al::AreaShapeCube* shape, sead::Vector3f* out,
                          const sead::Vector3f& from, const sead::Vector3f& to)
{
    const bool base = reinterpret_cast<const u8*>(shape)[20] == 1;
    const float bottom = base ? 0.0f : -500.0f;
    const float top = base ? 1000.0f : 500.0f;
    sead::Vector3f start = sead::Vector3f::zero;
    shape->calcLocalPos(&start, from);
    sead::Vector3f end = sead::Vector3f::zero;
    shape->calcLocalPos(&end, to);
    sead::Vector3f direction = subtract(end, start);

    // Repeat the retail flag read after both transformations.
    const bool startBase = reinterpret_cast<const u8*>(shape)[20] == 1;
    const float startBottom = startBase ? 0.0f : -500.0f;
    const float startTop = startBase ? 1000.0f : 500.0f;
    if (!(start.y < startBottom || start.y > startTop ||
          start.x < -500.0f || start.x > 500.0f ||
          start.z < -500.0f || start.z > 500.0f))
    {
        if (direction.y > 0.0f)
        {
            const float t = (top - start.y) / (end.y - start.y);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.z >= -500.0f && hit.z <= 500.0f)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        else if (direction.y < 0.0f)
        {
            const float t = (bottom - start.y) / (end.y - start.y);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.z >= -500.0f && hit.z <= 500.0f)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        if (direction.z > 0.0f)
        {
            const float t = (500.0f - start.z) / (end.z - start.z);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        else if (direction.z < 0.0f)
        {
            const float t = (-500.0f - start.z) / (end.z - start.z);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        if (direction.x > 0.0f)
        {
            const float t = (500.0f - start.x) / (end.x - start.x);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.z >= -500.0f && hit.z <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        else if (direction.x < 0.0f)
        {
            const float t = (-500.0f - start.x) / (end.x - start.x);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.z >= -500.0f && hit.z <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
    }
    else
    {
        if (direction.y > 0.0f)
        {
            const float t = (bottom - start.y) / (end.y - start.y);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.z >= -500.0f && hit.z <= 500.0f)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        else if (direction.y < 0.0f)
        {
            const float t = (top - start.y) / (end.y - start.y);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.z >= -500.0f && hit.z <= 500.0f)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        if (direction.z > 0.0f)
        {
            const float t = (-500.0f - start.z) / (end.z - start.z);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        else if (direction.z < 0.0f)
        {
            const float t = (500.0f - start.z) / (end.z - start.z);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.x >= -500.0f && hit.x <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        if (direction.x > 0.0f)
        {
            const float t = (-500.0f - start.x) / (end.x - start.x);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.z >= -500.0f && hit.z <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
        else if (direction.x < 0.0f)
        {
            const float t = (500.0f - start.x) / (end.x - start.x);
            if (t >= 0.0f && t <= 1.0f)
            {
                sead::Vector3f hit = add(multiply(direction, t), start);
                if (hit.z >= -500.0f && hit.z <= 500.0f &&
                    hit.y >= bottom && hit.y <= top)
                {
                    fn_00337488(shape, out, hit);
                    return true;
                }
            }
        }
    }
    return false;
}

#endif
