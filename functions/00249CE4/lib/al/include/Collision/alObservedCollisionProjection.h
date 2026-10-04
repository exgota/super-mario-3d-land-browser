#pragma once

#include <Collision/alCollisionParts.h>
#include <Collision/alKCollisionServer.h>
#include <Collision/alObservedCollisionPrism.h>
#include <nn/math/math_Vector3.h>

// Reconstructed direct-output effect contract. The original return spelling
// of 00249F1C is not recovered; all observed callers ignore its direct result.
extern "C" void fn_00249F1C(al::KCollisionServer*, nn::math::VEC3*,
    const al::ObservedCollisionPrism*, u32 vertex);
extern "C" const nn::math::VEC3* fn_00216D88(al::KCollisionServer*, u32 normal);
extern "C" void fn_00249CE4(al::CollisionParts*, nn::math::VEC3* output,
    const nn::math::VEC3* point, const al::ObservedCollisionPrism*, u32 kind);
