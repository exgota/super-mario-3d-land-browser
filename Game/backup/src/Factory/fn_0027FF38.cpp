#include <stdint.h>

namespace {
extern "C" uintptr_t fn_0027FF40(void *, uint32_t);
}

extern "C" uintptr_t fn_0027FF38(void *arg) {
    return fn_0027FF40(arg, *(uint32_t *)((char *)arg + 0x68));
}
