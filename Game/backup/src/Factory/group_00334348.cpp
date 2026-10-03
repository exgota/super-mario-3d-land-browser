#include <stdint.h>

namespace {
struct Object {
    unsigned char pad[0x50];
    uint32_t value;
};
}

extern "C" uint32_t fn_00334348(void *self) {
    return static_cast<Object *>(self)->value;
}

extern "C" uint32_t fn_0036E930(void *self) {
    return static_cast<Object *>(self)->value;
}

extern "C" uint32_t fn_0036EB18(void *self) {
    return static_cast<Object *>(self)->value;
}
