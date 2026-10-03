#include <Model/ObservedTransformComposition.h>
namespace observed_transform_composition {
static void scaleColumns(nn::math::MTX33& matrix, const nn::math::VEC3& scale) {
    for (int row = 0; row < 3; ++row) matrix.m[row][0] *= scale.x;
    for (int row = 0; row < 3; ++row) matrix.m[row][1] *= scale.y;
    for (int row = 0; row < 3; ++row) matrix.m[row][2] *= scale.z;
}
static void translateWithParentScale(nn::math::MTX34* output,
        const nn::math::MTX34* parent, const nn::math::VEC3& scale,
        nn::math::VEC3* translation) {
    fn_00291470(output, parent);
    nn::math::MTX33 rotation;
    nn::math::ARMv6::MTX34ToMTX33Asm(&rotation, output);
    scaleColumns(rotation, scale);
    nn::math::ARMv6::VEC3TransformAsm(translation, &rotation, translation);
    output->m[0][3] += translation->x;
    output->m[1][3] += translation->y;
    output->m[2][3] += translation->z;
}
}
extern "C" void fn_0033AA94(void*, nn::math::MTX34* output, nn::math::VEC3* outputScale,
        const observed_transform_composition::Transform* local,
        const observed_transform_composition::Transform* parentComposed,
        const observed_transform_composition::Transform* parentLocal) {
    using namespace observed_transform_composition;
    if (local->flags & 0x60) {
        fn_00291470(output, &parentComposed->matrix);
    } else {
        nn::math::VEC3 translation = {
            local->matrix.m[0][3], local->matrix.m[1][3], local->matrix.m[2][3]
        };
        if (local->flags & 0x80) {
            if (parentLocal->flags & 0x200) {
                fn_00216360(output, &parentComposed->matrix, &translation);
            } else {
                translateWithParentScale(output, &parentComposed->matrix,
                    parentLocal->scale, &translation);
            }
        } else {
            if (parentLocal->flags & 0x200) {
                fn_00216360(output, &parentComposed->matrix, &translation);
                fn_00224AD0(output, output, &local->matrix);
            } else {
                translateWithParentScale(output, &parentComposed->matrix,
                    parentLocal->scale, &translation);
                fn_00224AD0(output, output, &local->matrix);
            }
        }
    }
    if (parentComposed->flags & 0x200) {
        outputScale->x = local->scale.x;
        outputScale->y = local->scale.y;
        outputScale->z = local->scale.z;
    } else {
        outputScale->x = parentComposed->scale.x * local->scale.x;
        outputScale->y = parentComposed->scale.y * local->scale.y;
        outputScale->z = parentComposed->scale.z * local->scale.z;
    }
}
