#include "NativeFloatingPoint.h"
#include "softfloat.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>


static void ReadState(uint_fast8_t state[4]) {
    state[0] = softfloat_roundingMode;
    state[1] = softfloat_detectTininess;
    state[2] = softfloat_exceptionFlags;
    state[3] = extF80_roundingPrecision;
}
static void WriteState(const uint_fast8_t state[4]) {
    softfloat_roundingMode = state[0];
    softfloat_detectTininess = state[1];
    softfloat_exceptionFlags = state[2];
    extF80_roundingPrecision = state[3];
}
static unsigned ValidateState(const uint_fast8_t state[4]) {
    uint_fast8_t actual[4];
    ReadState(actual);
    for (unsigned index = 0; index < 4; ++index)
        if (actual[index] != state[index]) return 1;
    return 0;
}

struct ThreadResult {
    uint_fast8_t state[4];
    uintptr_t addresses[4];
    unsigned failureCount;
};
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condition = PTHREAD_COND_INITIALIZER;
static unsigned readyCount;
static void* ObserveThread(void* argument) {
    struct ThreadResult* result = argument;
    WriteState(result->state);
    result->addresses[0] = (uintptr_t)&softfloat_roundingMode;
    result->addresses[1] = (uintptr_t)&softfloat_detectTininess;
    result->addresses[2] = (uintptr_t)&softfloat_exceptionFlags;
    result->addresses[3] = (uintptr_t)&extF80_roundingPrecision;
    pthread_mutex_lock(&mutex);
    ++readyCount;
    pthread_cond_broadcast(&condition);
    while (readyCount < 2) pthread_cond_wait(&condition, &mutex);
    pthread_mutex_unlock(&mutex);
    for (unsigned index = 0; index < 10000; ++index) {
        uint32_t fpscr = index & 1 ? 0x00C00000u : 0;
        uint32_t expected = index & 1 ? 0x3EAAAAAAu : 0x3EAAAAABu;
        uint32_t actual = NativeFloatingPointApplySingle(
            NativeFloatingPointDivide, 0x3F800000u, 0x40400000u, 0, &fpscr);
        if (actual != expected || (fpscr & 0x9Fu) != 16u) ++result->failureCount;
        result->failureCount += ValidateState(result->state);
    }
    return NULL;
}

int VerifyFloatingPointEnvironment(void) {
    const uint_fast8_t states[][4] = {
        {4, 1, 31, 32}, {6, 0, 0, 64}, {99, 99, 255, 99}, {2, 1, 16, 80}
    };
    unsigned checks = 0, failures = 0;
    for (unsigned stateIndex = 0; stateIndex < 4; ++stateIndex) {
        WriteState(states[stateIndex]);
        for (unsigned operation = 0; operation <= 12; ++operation) {
            for (unsigned controls = 0; controls < 16; ++controls) {
                uint32_t fpscr = (controls << 22) | 0x9Fu;
                NativeFloatingPointApplySingle((NativeFloatingPointOperation)operation,
                    0x3F800000u, 0x40400000u, 0xBF800000u, &fpscr);
                failures += ValidateState(states[stateIndex]); ++checks;
                fpscr = (controls << 22) | 0x9Fu;
                NativeFloatingPointApplyDouble((NativeFloatingPointOperation)operation,
                    UINT64_C(0x3FF0000000000000), UINT64_C(0x4008000000000000),
                    UINT64_C(0xBFF0000000000000), &fpscr);
                failures += ValidateState(states[stateIndex]); ++checks;
            }
        }
        uint32_t fpscr = 0x01000010u;
        if (NativeFloatingPointPrepareSingle(0x80000001u, &fpscr) != 0x80000000u
            || fpscr != 0x01000090u) ++failures;
        failures += ValidateState(states[stateIndex]); ++checks;
        fpscr = 0x01000010u;
        if (NativeFloatingPointPrepareDouble(UINT64_C(0x8000000000000001), &fpscr)
            != UINT64_C(0x8000000000000000) || fpscr != 0x01000090u) ++failures;
        failures += ValidateState(states[stateIndex]); ++checks;
    }
    const uint_fast8_t mainState[] = {4, 1, 31, 32};
    WriteState(mainState);
    struct ThreadResult results[2] = {
        {{6, 0, 16, 64}, {0}, 0}, {{2, 1, 7, 80}, {0}, 0}
    };
    pthread_t threads[2];
    for (unsigned index = 0; index < 2; ++index)
        if (pthread_create(&threads[index], NULL, ObserveThread, &results[index])) return 2;
    for (unsigned index = 0; index < 2; ++index)
        if (pthread_join(threads[index], NULL)) return 3;
    for (unsigned index = 0; index < 4; ++index)
        if (results[0].addresses[index] == results[1].addresses[index]) ++failures;
    failures += results[0].failureCount + results[1].failureCount + ValidateState(mainState);
    printf("{\"state_preservation_checks\":%u,\"thread_arithmetic_checks\":20000,"
           "\"distinct_thread_local_variables\":4,\"failure_count\":%u}\n", checks, failures);
    return failures != 0;
}
