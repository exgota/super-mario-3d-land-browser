#include <stdint.h>

namespace {
struct ObjectAt306C84 {
    uint8_t padding[0x78];
    uint32_t value;
};
}

extern "C" void fn_00306C84(ObjectAt306C84 *self, uint32_t value) {
    self->value = value;
}
