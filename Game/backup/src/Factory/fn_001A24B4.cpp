#include <stdint.h>

namespace {
struct Object {
    uint8_t padding[0x60];
    uint32_t value;
};
}

extern "C" void fn_001A24B4(Object *object, uint32_t value) {
    object->value = value;
}
