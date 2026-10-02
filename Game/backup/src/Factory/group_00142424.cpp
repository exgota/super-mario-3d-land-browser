#include <stdint.h>

namespace {
struct Inner {
    unsigned char pad[0xC];
    uint32_t value;
};

struct Owner {
    unsigned char pad[0x64];
    Inner* inner;
};
}

extern "C" void fn_0026AA60(uint32_t);
extern "C" void fn_0026A9B8(uint32_t);
extern "C" void fn_002CEA40(uint32_t);

extern "C" void fn_00142424(Owner* self) {
    uint32_t value = self->inner->value;
    if (value != 0) fn_0026AA60(value);
}

extern "C" void fn_001426A0(Owner* self) {
    uint32_t value = self->inner->value;
    if (value != 0) fn_0026A9B8(value);
}

extern "C" void fn_00142AFC(Owner* self) {
    uint32_t value = self->inner->value;
    if (value != 0) fn_002CEA40(value);
}
