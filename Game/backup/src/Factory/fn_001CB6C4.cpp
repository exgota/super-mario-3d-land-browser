#include <stdint.h>

extern "C" uint32_t fn_001CB6D0(uint32_t, uint32_t, uint32_t);

extern "C" uint32_t fn_001CB6C4(void *self, uint32_t arg1)
{
    return fn_001CB6D0(*(uint32_t *)((char *)self + 0x50), arg1, 14);
}
