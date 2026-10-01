#ifdef NON_MATCHING
#include <math/seadMatrix.h>

namespace nn
{
namespace math
{

struct MTX34
{
        float m00, m01, m02, m03;
        float m10, m11, m12, m13;
        float m20, m21, m22, m23;
};

struct QUAT
{
        float x, y, z, w;
};

} // namespace math
} // namespace nn

namespace sead
{

template <typename T>
class Matrix34CalcCtr
{
public:
        static void makeQ( nn::math::MTX34& output, const nn::math::QUAT& rotation );
};

} // namespace sead

namespace sead {
template <>
void Matrix34CalcCtr<float>::makeQ( nn::math::MTX34& output, const nn::math::QUAT& rotation )
{
        float xy = rotation.x * rotation.y;
        float xz = rotation.x * rotation.z;
        float xx = rotation.x * rotation.x;
        float yz = rotation.y * rotation.z;
        float yy = rotation.y * rotation.y;
        float wy = rotation.w * rotation.y;
        float wz = rotation.w * rotation.z;
        float zz = rotation.z * rotation.z;
        float wx = rotation.w * rotation.x;

        output.m00 = 1 - yy * 2.0f - zz * 2.0f;
        output.m01 = xy * 2.0f - wz * 2.0f;
        output.m02 = xz * 2.0f + wy * 2.0f;
        output.m03 = 0;
        output.m10 = xy * 2.0f + wz * 2.0f;
        output.m11 = 1 - xx * 2.0f - zz * 2.0f;
        output.m12 = yz * 2.0f - wx * 2.0f;
        output.m13 = 0;
        output.m20 = xz * 2.0f - wy * 2.0f;
        output.m21 = yz * 2.0f + wx * 2.0f;
        output.m22 = 1 - xx * 2.0f - yy * 2.0f;
        output.m23 = 0;
}

}

#endif
