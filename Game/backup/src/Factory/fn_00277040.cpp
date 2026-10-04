#include <stdint.h>

extern "C" uint32_t fn_001D4810(uint32_t, uint32_t, uint32_t, uint32_t);

extern "C" uint32_t fn_00277040(uint32_t a, uint32_t b, uint32_t c, uint32_t index) {
    uint32_t table = *(uint32_t *)(a + 0xC);
    uint32_t value = *(uint32_t *)(table + index * 4);
    return fn_001D4810(value, b, c, index);
}
