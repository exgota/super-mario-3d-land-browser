#include "NativeFloatingPoint.h"

#include <stdio.h>
#include <stdlib.h>

#ifndef THREAD_LOCAL
#error "NativeFloatingPoint requires SoftFloat built with THREAD_LOCAL=_Thread_local"
#endif
#include "softfloat.h"

/* These declarations reject an empty/non-TLS THREAD_LOCAL definition. The
 * linked ARM-VFPv2 SoftFloat library must use the same C11 TLS declarations. */
extern _Thread_local uint_fast8_t softfloat_roundingMode;
extern _Thread_local uint_fast8_t softfloat_detectTininess;
extern _Thread_local uint_fast8_t softfloat_exceptionFlags;
extern _Thread_local uint_fast8_t extF80_roundingPrecision;

_Static_assert(sizeof(float32_t) == sizeof(uint32_t), "SoftFloat binary32 storage width");
_Static_assert(sizeof(float64_t) == sizeof(uint64_t), "SoftFloat binary64 storage width");

typedef struct NativeFloatingPointEnvironment {
    uint_fast8_t roundingMode;
    uint_fast8_t detectTininess;
    uint_fast8_t exceptionFlags;
    uint_fast8_t extendedRoundingPrecision;
} NativeFloatingPointEnvironment;

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

static void NativeFloatingPointBegin(uint32_t* fpscr, NativeFloatingPointEnvironment* environment) {
    static const uint_fast8_t rounding_modes[4] = {
        softfloat_round_near_even, softfloat_round_max,
        softfloat_round_min, softfloat_round_minMag
    };
    NativeFloatingPointValidate(fpscr);
    environment->roundingMode = softfloat_roundingMode;
    environment->detectTininess = softfloat_detectTininess;
    environment->exceptionFlags = softfloat_exceptionFlags;
    environment->extendedRoundingPrecision = extF80_roundingPrecision;
    softfloat_roundingMode = rounding_modes[(*fpscr >> 22) & 3];
    /* Original VMUL boundary probes in both precisions set UFC|IXC for a tiny
     * result that rounds to minimum normal. FZ also flushes that result. */
    softfloat_detectTininess = softfloat_tininess_beforeRounding;
    softfloat_exceptionFlags = 0;
}

static void NativeFloatingPointEnd(const NativeFloatingPointEnvironment* environment) {
    softfloat_roundingMode = environment->roundingMode;
    softfloat_detectTininess = environment->detectTininess;
    softfloat_exceptionFlags = environment->exceptionFlags;
    extF80_roundingPrecision = environment->extendedRoundingPrecision;
}

static uint32_t NativeFloatingPointExceptionFlags(uint_fast8_t exceptions) {
    uint32_t flags = 0;
    if (exceptions & softfloat_flag_invalid) flags |= NativeFloatingPointInvalidFlag;
    if (exceptions & softfloat_flag_infinite) flags |= NativeFloatingPointDivideByZeroFlag;
    if (exceptions & softfloat_flag_overflow) flags |= NativeFloatingPointOverflowFlag;
    if (exceptions & softfloat_flag_underflow) flags |= NativeFloatingPointUnderflowFlag;
    if (exceptions & softfloat_flag_inexact) flags |= NativeFloatingPointInexactFlag;
    return flags;
}

/* NaN classification, operand priority and raw operations use integer bits.
 * SoftFloat performs only the selected arithmetic, never a fused operation. */
#define DEFINE_NATIVE_FLOATING_POINT(Suffix, Integer, Real, FunctionPrefix, SignMask, ExponentMask, FractionMask, QuietMask, DefaultNan) \
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
    const Real a = { left }, b = { right }; \
    Real result = { 0 }; \
    softfloat_exceptionFlags = 0; \
    switch (operation) { \
    case NativeFloatingPointMultiply: result = FunctionPrefix##_mul(a, b); break; \
    case NativeFloatingPointAdd: result = FunctionPrefix##_add(a, b); break; \
    case NativeFloatingPointSubtract: result = FunctionPrefix##_sub(a, b); break; \
    case NativeFloatingPointDivide: result = FunctionPrefix##_div(a, b); break; \
    case NativeFloatingPointSquareRoot: result = FunctionPrefix##_sqrt(b); break; \
    default: NativeFloatingPointRefuse("invalid arithmetic operation"); break; \
    } \
    Integer bits = result.v; \
    uint32_t flags = NativeFloatingPointExceptionFlags(softfloat_exceptionFlags); \
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
    NativeFloatingPointEnvironment environment; \
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
    NativeFloatingPointEnd(&environment); \
    return result; \
}

DEFINE_NATIVE_FLOATING_POINT(Single, uint32_t, float32_t, f32,
                            UINT32_C(0x80000000), UINT32_C(0x7F800000),
                            UINT32_C(0x007FFFFF), UINT32_C(0x00400000),
                            UINT32_C(0x7FC00000))
DEFINE_NATIVE_FLOATING_POINT(Double, uint64_t, float64_t, f64,
                            UINT64_C(0x8000000000000000), UINT64_C(0x7FF0000000000000),
                            UINT64_C(0x000FFFFFFFFFFFFF), UINT64_C(0x0008000000000000),
                            UINT64_C(0x7FF8000000000000))

#undef DEFINE_NATIVE_FLOATING_POINT
