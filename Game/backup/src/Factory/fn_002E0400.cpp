#include <stdint.h>

namespace {
struct Object {
    uint8_t pad[0x20];
    uint32_t value;
};
}

extern "C" void fn_002E0400(Object* self, uint32_t value) {
    self->value = value;
}
