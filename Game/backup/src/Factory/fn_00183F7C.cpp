#include <stdint.h>

extern "C" uint32_t fn_0025CB78(void*);

extern "C" uint32_t fn_00183F7C(void* p) {
    return fn_0025CB78(*(void**)((char*)p + 0x18));
}
