#ifndef NATIVE_TIMING_H
#define NATIVE_TIMING_H
#include "recomp.h"

#ifdef __cplusplus
extern "C" {
#endif
typedef void (*NativeBlockTimingCallback)(Context*, uint32_t, uint32_t, uint64_t);
extern RECOMP_EXPORT NativeBlockTimingCallback native_block_timing_callback;
#ifdef __cplusplus
}
#endif

#undef BUDGET
#define BUDGET(address, count, ticks) do { \
    if (UNLIKELY(ctx->budget < (int32_t)(count))) { \
        ctx->r[15] = (address); ctx->exit = EXIT_BUDGET; return; \
    } \
    ctx->budget -= (count); \
    if (native_block_timing_callback) native_block_timing_callback(ctx, (address), (count), (ticks)); \
} while (0)
#endif
