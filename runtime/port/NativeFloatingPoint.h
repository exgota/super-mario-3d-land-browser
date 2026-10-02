#ifndef SUPER_MARIO_3D_LAND_NATIVE_FLOATING_POINT_H
#define SUPER_MARIO_3D_LAND_NATIVE_FLOATING_POINT_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* These operation numbers are compile-time selectors used by the static
 * translator. They are not ARM instruction encodings. */
typedef enum NativeFloatingPointOperation {
    NativeFloatingPointMultiplyAccumulate = 0,
    NativeFloatingPointMultiplySubtract = 1,
    NativeFloatingPointNegatedAccumulatorAdd = 2,
    NativeFloatingPointNegatedAccumulatorSubtract = 3,
    NativeFloatingPointMultiply = 4,
    NativeFloatingPointNegatedMultiply = 5,
    NativeFloatingPointAdd = 6,
    NativeFloatingPointSubtract = 7,
    NativeFloatingPointDivide = 8,
    NativeFloatingPointMove = 9,
    NativeFloatingPointAbsolute = 10,
    NativeFloatingPointNegate = 11,
    NativeFloatingPointSquareRoot = 12
} NativeFloatingPointOperation;

/* Operands/results are IEEE bits. Integer-only SoftFloat arithmetic has explicit
 * rounding and C11 thread-local state; it does not use host fenv or host floats.
 * SquareRoot, Move, Absolute and Negate use right only. Other arithmetic
 * uses left/right, and operations 0 through 3 also use accumulator.
 *
 * Rounding, DN, FZ and cumulative exception flags come from the explicit guest
 * FPSCR. All four SoftFloat state variables are restored before return.
 * Enabled guest arithmetic exception traps and invalid selectors abort.
 * Raw Move/Absolute/Negate preserve FPSCR even when traps are enabled.
 * Guest exception delivery,
 * conversions, comparisons and short-vector register sequencing are not
 * implemented here. See project/native_floating_point_evidence.md for limits. */
uint32_t NativeFloatingPointApplySingle(NativeFloatingPointOperation operation,
                                      uint32_t left, uint32_t right,
                                      uint32_t accumulator, uint32_t* fpscr);
uint64_t NativeFloatingPointApplyDouble(NativeFloatingPointOperation operation,
                                      uint64_t left, uint64_t right,
                                      uint64_t accumulator, uint32_t* fpscr);

/* Prepare flushes an arithmetic INPUT subnormal to signed zero and sets IDC
 * when FZ is enabled. Never apply it to raw register moves, loads or stores. */
uint32_t NativeFloatingPointPrepareSingle(uint32_t bits, uint32_t* fpscr);
uint64_t NativeFloatingPointPrepareDouble(uint64_t bits, uint32_t* fpscr);

#ifdef __cplusplus
}
#endif

#endif
