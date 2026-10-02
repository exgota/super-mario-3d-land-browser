#include "NativeFloatingPoint.h"

#include <fenv.h>
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#pragma STDC FENV_ACCESS ON
#pragma STDC FP_CONTRACT OFF

_Static_assert(FLT_RADIX == 2 && FLT_MANT_DIG == 24 && DBL_MANT_DIG == 53,
               "NativeFloatingPoint requires IEEE binary32 and binary64");
_Static_assert(sizeof(float) == 4 && sizeof(double) == 8,
               "NativeFloatingPoint requires IEEE storage widths");

enum {
    NativeFloatingPointInvalidFlag = 1u,
    NativeFloatingPointDivideByZeroFlag = 2u,
    NativeFloatingPointOverflowFlag = 4u,
    NativeFloatingPointUnderflowFlag = 8u,
    NativeFloatingPointInexactFlag = 16u,
    NativeFloatingPointInputDenormalFlag = 128u,
    NativeFloatingPointTrapEnables = 0x9F00u,
    NativeFloatingPointFlushToZero = 1u << 24,
    NativeFloatingPointDefaultNan = 1u << 25
};

static void NativeFloatingPointRefuse(const char* reason) {
    fprintf(stderr, "NativeFloatingPoint: %s\n", reason);
    abort();
}

static void NativeFloatingPointValidate(uint32_t* fpscr) {
    if (fpscr == NULL) NativeFloatingPointRefuse("missing guest FPSCR");
    if (*fpscr & NativeFloatingPointTrapEnables)
        NativeFloatingPointRefuse("guest floating-point exception delivery is unsupported");
}

static void NativeFloatingPointBegin(uint32_t* fpscr, fenv_t* environment) {
    static const int rounding_modes[4] = {
        FE_TONEAREST, FE_UPWARD, FE_DOWNWARD, FE_TOWARDZERO
    };
    NativeFloatingPointValidate(fpscr);
    if (feholdexcept(environment) != 0)
        NativeFloatingPointRefuse("cannot preserve host floating-point environment");
    if (fesetenv(FE_DFL_ENV) != 0)
        NativeFloatingPointRefuse("cannot establish IEEE host floating-point environment");
    if (fesetround(rounding_modes[(*fpscr >> 22) & 3]) != 0)
        NativeFloatingPointRefuse("host does not support requested rounding mode");
}

static uint32_t NativeFloatingPointExceptionFlags(int exceptions) {
    uint32_t flags = 0;
    if (exceptions & FE_INVALID) flags |= NativeFloatingPointInvalidFlag;
    if (exceptions & FE_DIVBYZERO) flags |= NativeFloatingPointDivideByZeroFlag;
    if (exceptions & FE_OVERFLOW) flags |= NativeFloatingPointOverflowFlag;
    if (exceptions & FE_UNDERFLOW) flags |= NativeFloatingPointUnderflowFlag;
    if (exceptions & FE_INEXACT) flags |= NativeFloatingPointInexactFlag;
    return flags;
}

/* NaN classification and operand priority use integer bits so the host cannot
 * quiet signaling operands before the guest invalid-operation flag is set. */
#define DEFINE_NATIVE_FLOATING_POINT(Suffix, Integer, Real, SignMask, ExponentMask, FractionMask, QuietMask, DefaultNan, SquareRoot) \
static int NativeFloatingPointIsNan##Suffix(Integer bits) { \
    return (bits & (ExponentMask)) == (ExponentMask) && (bits & (FractionMask)); \
} \
static int NativeFloatingPointIsSignalingNan##Suffix(Integer bits) { \
    return NativeFloatingPointIsNan##Suffix(bits) && !(bits & (QuietMask)); \
} \
static int NativeFloatingPointIsSubnormal##Suffix(Integer bits) { \
    return !(bits & (ExponentMask)) && (bits & (FractionMask)); \
} \
Integer NativeFloatingPointPrepare##Suffix(Integer bits, uint32_t* fpscr) { \
    NativeFloatingPointValidate(fpscr); \
    if ((*fpscr & NativeFloatingPointFlushToZero) && NativeFloatingPointIsSubnormal##Suffix(bits)) { \
        *fpscr |= NativeFloatingPointInputDenormalFlag; \
        return bits & (SignMask); \
    } \
    return bits; \
} \
static Integer NativeFloatingPointSelectNan##Suffix(Integer left, Integer right, uint32_t* fpscr) { \
    Integer selected; \
    if (NativeFloatingPointIsSignalingNan##Suffix(left)) selected = left; \
    else if (NativeFloatingPointIsSignalingNan##Suffix(right)) selected = right; \
    else if (NativeFloatingPointIsNan##Suffix(left)) selected = left; \
    else selected = right; \
    if (NativeFloatingPointIsSignalingNan##Suffix(selected)) *fpscr |= NativeFloatingPointInvalidFlag; \
    return (*fpscr & NativeFloatingPointDefaultNan) ? (DefaultNan) : selected | (QuietMask); \
} \
static Integer NativeFloatingPointEvaluate##Suffix(NativeFloatingPointOperation operation, \
                                                  Integer left, Integer right, uint32_t* fpscr) { \
    if (operation == NativeFloatingPointSquareRoot) left = 0; \
    else left = NativeFloatingPointPrepare##Suffix(left, fpscr); \
    right = NativeFloatingPointPrepare##Suffix(right, fpscr); \
    if (NativeFloatingPointIsNan##Suffix(left) || NativeFloatingPointIsNan##Suffix(right)) \
        return NativeFloatingPointSelectNan##Suffix(left, right, fpscr); \
    Real left_value, right_value, result_value; \
    memcpy(&left_value, &left, sizeof(left)); \
    memcpy(&right_value, &right, sizeof(right)); \
    volatile Real a = left_value, b = right_value, result; \
    if (feclearexcept(FE_ALL_EXCEPT) != 0) NativeFloatingPointRefuse("cannot clear host exception flags"); \
    switch (operation) { \
    case NativeFloatingPointMultiply: result = a * b; break; \
    case NativeFloatingPointAdd: result = a + b; break; \
    case NativeFloatingPointSubtract: result = a - b; break; \
    case NativeFloatingPointDivide: result = a / b; break; \
    case NativeFloatingPointSquareRoot: result = SquareRoot(b); break; \
    default: NativeFloatingPointRefuse("invalid arithmetic operation"); result = 0; break; \
    } \
    result_value = result; \
    Integer bits; \
    memcpy(&bits, &result_value, sizeof(bits)); \
    uint32_t flags = NativeFloatingPointExceptionFlags(fetestexcept(FE_ALL_EXCEPT)); \
    if ((*fpscr & NativeFloatingPointFlushToZero) && \
        (NativeFloatingPointIsSubnormal##Suffix(bits) || (flags & NativeFloatingPointUnderflowFlag))) { \
        bits &= (SignMask); \
        flags = (flags & ~NativeFloatingPointInexactFlag) | NativeFloatingPointUnderflowFlag; \
    } \
    if (NativeFloatingPointIsNan##Suffix(bits)) bits = (DefaultNan); \
    *fpscr |= flags; \
    return bits; \
} \
Integer NativeFloatingPointApply##Suffix(NativeFloatingPointOperation operation, \
                                         Integer left, Integer right, Integer accumulator, uint32_t* fpscr) { \
    if (fpscr == NULL) NativeFloatingPointRefuse("missing guest FPSCR"); \
    if (operation == NativeFloatingPointMove) return right; \
    if (operation == NativeFloatingPointAbsolute) return right & ~(SignMask); \
    if (operation == NativeFloatingPointNegate) return right ^ (SignMask); \
    fenv_t environment; \
    NativeFloatingPointBegin(fpscr, &environment); \
    Integer result; \
    if (operation >= NativeFloatingPointMultiplyAccumulate && operation <= NativeFloatingPointNegatedAccumulatorSubtract) { \
        Integer product = NativeFloatingPointEvaluate##Suffix(NativeFloatingPointMultiply, left, right, fpscr); \
        if (operation == NativeFloatingPointMultiplySubtract || operation == NativeFloatingPointNegatedAccumulatorSubtract) product ^= (SignMask); \
        if (operation == NativeFloatingPointNegatedAccumulatorAdd || operation == NativeFloatingPointNegatedAccumulatorSubtract) accumulator ^= (SignMask); \
        result = NativeFloatingPointEvaluate##Suffix(NativeFloatingPointAdd, accumulator, product, fpscr); \
    } else if (operation == NativeFloatingPointNegatedMultiply) { \
        result = NativeFloatingPointEvaluate##Suffix(NativeFloatingPointMultiply, left, right, fpscr) ^ (SignMask); \
    } else { \
        result = NativeFloatingPointEvaluate##Suffix(operation, left, right, fpscr); \
    } \
    if (fesetenv(&environment) != 0) NativeFloatingPointRefuse("cannot restore host floating-point environment"); \
    return result; \
}

DEFINE_NATIVE_FLOATING_POINT(Single, uint32_t, float,
                            UINT32_C(0x80000000), UINT32_C(0x7F800000),
                            UINT32_C(0x007FFFFF), UINT32_C(0x00400000),
                            UINT32_C(0x7FC00000), sqrtf)
DEFINE_NATIVE_FLOATING_POINT(Double, uint64_t, double,
                            UINT64_C(0x8000000000000000), UINT64_C(0x7FF0000000000000),
                            UINT64_C(0x000FFFFFFFFFFFFF), UINT64_C(0x0008000000000000),
                            UINT64_C(0x7FF8000000000000), sqrt)

#undef DEFINE_NATIVE_FLOATING_POINT
