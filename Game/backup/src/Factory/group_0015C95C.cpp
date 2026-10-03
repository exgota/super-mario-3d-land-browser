#include <stdint.h>

namespace {
struct Result {
    unsigned char pad[0x20];
    uint32_t value;
};
}

extern "C" Result *fn_0027768C(void *);
extern "C" Result *fn_00268E64(void *);

extern "C" uint32_t fn_0015C95C(void *self) {
    return fn_0027768C(self)->value;
}

extern "C" uint32_t fn_0025161C(void *self) {
    return fn_00268E64(self)->value;
}
