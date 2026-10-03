#include <stdint.h>

extern "C" void fn_00244004(void *dst, uint32_t first, uint32_t second)
{
    uint32_t *words = static_cast<uint32_t *>(dst);
    words[0] = first;
    words[2] = second;
}
