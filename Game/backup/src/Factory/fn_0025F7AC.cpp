#include <stdint.h>

namespace {
struct Object {
    uint32_t unknown0;
    uint32_t next;
    uint8_t unknown8[0x40];
    float value;
};
}

extern "C" uint32_t fn_0025F7B8(uint32_t);
extern "C" uint32_t fn_0025F7AC(Object *object, float value) {
    object->value = value;
    return fn_0025F7B8(object->next);
}
