#include <Collision/alObservedCollisionProjection.h>
#include <math/seadVectorCalcCtr.h>

// NonMatching: reconstructed projection of a point onto a prism feature.
extern "C" void fn_00249CE4(al::CollisionParts* parts, nn::math::VEC3* output,
    const nn::math::VEC3* point, const al::ObservedCollisionPrism* prism, u32 kind)
{
    typedef nn::math::VEC3 Vector;
    typedef sead::Vector3CalcCtr<float> Calc;
    switch (kind)
    {
    case 0:
        return;
    case 1:
    case 2:
    case 3:
    case 4:
    {
        Vector vertex;
        fn_00249F1C(parts->getCollisionServer(), &vertex, prism, 0);
        const Vector* faceNormal = fn_00216D88(parts->getCollisionServer(), prism->normal06);
        Vector delta;
        Calc::sub(delta, *point, vertex);
        float distance = delta.x * faceNormal->x + delta.y * faceNormal->y + delta.z * faceNormal->z;
        Calc::multScalarAdd(*output, -distance, *faceNormal, *point);
        if (kind == 1)
            return;
        // Keep this copy after the first output store: point may alias output.
        Vector relative = *point;
        Vector edgeNormal;
        switch (kind)
        {
        case 2:
            edgeNormal = *fn_00216D88(parts->getCollisionServer(), prism->normal08);
            Calc::sub(relative, relative, vertex);
            break;
        case 3:
            edgeNormal = *fn_00216D88(parts->getCollisionServer(), prism->normal0A);
            Calc::sub(relative, relative, vertex);
            break;
        case 4:
        {
            edgeNormal = *fn_00216D88(parts->getCollisionServer(), prism->normal0C);
            Vector secondVertex;
            fn_00249F1C(parts->getCollisionServer(), &secondVertex, prism, 1);
            Calc::sub(relative, relative, secondVertex);
            break;
        }
        }
        float edgeDistance = relative.x * edgeNormal.x + relative.y * edgeNormal.y + relative.z * edgeNormal.z;
        Calc::multScalarAdd(*output, -edgeDistance, edgeNormal, *output);
        return;
    }
    case 5:
        fn_00249F1C(parts->getCollisionServer(), output, prism, 0);
        return;
    case 6:
        fn_00249F1C(parts->getCollisionServer(), output, prism, 1);
        return;
    case 7:
        fn_00249F1C(parts->getCollisionServer(), output, prism, 2);
        return;
    }
}
