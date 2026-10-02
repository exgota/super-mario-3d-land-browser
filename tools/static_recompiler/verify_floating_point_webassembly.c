#include "NativeFloatingPoint.h"
#include "softfloat.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int VerifyFloatingPointEnvironment(void);

static uint32_t ReadUnsigned32(const unsigned char* bytes) {
    return (uint32_t)bytes[0] | (uint32_t)bytes[1] << 8
         | (uint32_t)bytes[2] << 16 | (uint32_t)bytes[3] << 24;
}
static uint64_t ReadUnsigned64(const unsigned char* bytes) {
    return (uint64_t)ReadUnsigned32(bytes) | (uint64_t)ReadUnsigned32(bytes + 4) << 32;
}
static void WriteUnsigned64(unsigned char* bytes, uint64_t value) {
    for (unsigned index = 0; index < 8; ++index) bytes[index] = (unsigned char)(value >> (index * 8));
}
static void WriteUnsigned32(unsigned char* bytes, uint32_t value) {
    for (unsigned index = 0; index < 4; ++index) bytes[index] = (unsigned char)(value >> (index * 8));
}
static unsigned RefusalControl(char** arguments) {
    unsigned wide = strtoul(arguments[2], NULL, 10);
    unsigned operation = strtoul(arguments[3], NULL, 10);
    uint32_t status = (uint32_t)strtoul(arguments[4], NULL, 10);
    uint32_t* pointer = arguments[5][0] == '0' ? NULL : &status;
    if (arguments[1][0] == 'P') {
        if (wide) NativeFloatingPointPrepareDouble(1, pointer);
        else NativeFloatingPointPrepareSingle(1, pointer);
    } else {
        if (wide) NativeFloatingPointApplyDouble((NativeFloatingPointOperation)operation,
            UINT64_C(0x3FF0000000000000), UINT64_C(0x3FF0000000000000), 0, pointer);
        else NativeFloatingPointApplySingle((NativeFloatingPointOperation)operation,
            UINT32_C(0x3F800000), UINT32_C(0x3F800000), 0, pointer);
    }
    return 0;
}

int main(int argumentCount, char** arguments) {
    if (argumentCount == 6) return (int)RefusalControl(arguments);
    if (argumentCount != 3) return 2;
    if (sizeof(void*) != 4) return 3;
    int environmentResult = VerifyFloatingPointEnvironment();
    if (environmentResult) return 4;
    FILE* input = fopen(arguments[1], "rb");
    FILE* output = fopen(arguments[2], "wbx");
    if (!input || !output) return 5;
    unsigned char record[48], actualRecord[12];
    unsigned count = 0, mismatchCount = 0, stateFailureCount = 0, singleCount = 0, doubleCount = 0;
    size_t length;
    while ((length = fread(record, 1, sizeof(record), input)) != 0) {
        if (length != sizeof(record)) return 6;
        uint32_t operation = ReadUnsigned32(record), wide = ReadUnsigned32(record + 4);
        uint32_t fpscr = ReadUnsigned32(record + 8);
        if (operation > 12 || wide > 1) return 7;
        uint64_t left = ReadUnsigned64(record + 12), right = ReadUnsigned64(record + 20);
        uint64_t accumulator = ReadUnsigned64(record + 28), expected = ReadUnsigned64(record + 36);
        uint32_t expectedFpscr = ReadUnsigned32(record + 44);
        uint_fast8_t state[4] = {
            (uint_fast8_t)(count % 2 ? 6 : 4), (uint_fast8_t)(count % 2),
            (uint_fast8_t)(count % 32), (uint_fast8_t)(count % 3 == 0 ? 32 : count % 3 == 1 ? 64 : 80)
        };
        softfloat_roundingMode = state[0]; softfloat_detectTininess = state[1];
        softfloat_exceptionFlags = state[2]; extF80_roundingPrecision = state[3];
        uint64_t actual;
        if (wide) {
            actual = NativeFloatingPointApplyDouble((NativeFloatingPointOperation)operation,
                left, right, accumulator, &fpscr);
            ++doubleCount;
        } else {
            actual = NativeFloatingPointApplySingle((NativeFloatingPointOperation)operation,
                (uint32_t)left, (uint32_t)right, (uint32_t)accumulator, &fpscr);
            ++singleCount;
        }
        if (actual != expected || fpscr != expectedFpscr) ++mismatchCount;
        if (softfloat_roundingMode != state[0] || softfloat_detectTininess != state[1]
            || softfloat_exceptionFlags != state[2] || extF80_roundingPrecision != state[3]) ++stateFailureCount;
        WriteUnsigned64(actualRecord, actual); WriteUnsigned32(actualRecord + 8, fpscr);
        if (fwrite(actualRecord, 1, sizeof(actualRecord), output) != sizeof(actualRecord)) return 8;
        ++count;
    }
    if (ferror(input) || fclose(input) || fclose(output)) return 9;
    printf("{\"wasm_pointer_bytes\":%u,\"fixture_count\":%u,\"single_cases\":%u,"
           "\"double_cases\":%u,\"numerical_mismatch_count\":%u,\"state_failure_count\":%u}\n",
           (unsigned)sizeof(void*), count, singleCount, doubleCount, mismatchCount, stateFailureCount);
    return mismatchCount || stateFailureCount;
}
