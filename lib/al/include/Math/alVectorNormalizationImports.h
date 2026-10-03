#pragma once

#include <nn/math/math_Vector3.h>

// Neutral wrapper for the mapped in-place normalizer. Float retains the
// accepted wrapper contract; the original callee leaves vector length in s0.
extern "C" float fn_00279ABC( nn::math::VEC3& vector );
