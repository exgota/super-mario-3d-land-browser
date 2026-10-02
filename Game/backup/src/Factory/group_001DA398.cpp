#include <stdint.h>

namespace {
extern "C" uint32_t dat_003D6DD8;
extern "C" uint32_t dat_003D96F0;
}

extern "C" void fn_001DA398(uint32_t* object)
{
    object[0] = (uint32_t)&dat_003D6DD8;
    object[1] = 0;
}

extern "C" void fn_002DAB0C(uint32_t* object)
{
    object[0] = (uint32_t)&dat_003D96F0;
    object[1] = 0;
}
