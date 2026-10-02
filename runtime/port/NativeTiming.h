#ifndef NATIVE_TIMING_H
#define NATIVE_TIMING_H
#include "recomp.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*NativeBlockTimingCallback)(Context*, uint32_t, uint32_t, uint64_t);
extern RECOMP_EXPORT NativeBlockTimingCallback native_block_timing_callback;
extern RECOMP_EXPORT const uint32_t native_timing_revision;
#ifdef __cplusplus
}
#endif

// The stock CPU charges a skipped conditional instruction one cycle. Conditions
// are evaluated before the translated instruction can change the flags.
static inline uint64_t NativeConditionalTicks(const Context* context, uint32_t condition, uint64_t passed) {
    int execute;
    switch (condition) {
    case 0: execute = context->z; break;
    case 1: execute = !context->z; break;
    case 2: execute = context->c; break;
    case 3: execute = !context->c; break;
    case 4: execute = context->n; break;
    case 5: execute = !context->n; break;
    case 6: execute = context->v; break;
    case 7: execute = !context->v; break;
    case 8: execute = context->c && !context->z; break;
    case 9: execute = !context->c || context->z; break;
    case 10: execute = context->n == context->v; break;
    case 11: execute = context->n != context->v; break;
    case 12: execute = !context->z && context->n == context->v; break;
    case 13: execute = context->z || context->n != context->v; break;
    default: execute = 1; break;
    }
    return execute ? passed : 1;
}
// SVC timing becomes visible after HLE completes in the pinned stock CPU.
#define NATIVE_SUPERVISOR_TICKS(ticks) ((uint64_t)(ticks) | (UINT64_C(1) << 63))

#undef BUDGET
#define BUDGET(address, count, ticks) do { \
    if (UNLIKELY(ctx->budget < (int32_t)(count))) { \
        ctx->r[15] = (address); ctx->exit = EXIT_BUDGET; return; \
    } \
    if (native_block_timing_callback) { \
        native_block_timing_callback(ctx, (address), (count), (ticks)); \
        if (UNLIKELY(ctx->exit != EXIT_NONE)) return; \
    } \
    ctx->budget -= (count); \
} while (0)
#endif
