#include <stdint.h>

extern "C" uint32_t fn_001DB2B8(uint32_t, uint32_t);

extern "C" uint32_t fn_001DB2B0(uint32_t a, uint32_t b) {
    return fn_001DB2B8(a, *(uint32_t *)(b + 0x28));
}
