#include <stdint.h>

namespace {
struct Receiver {
    unsigned char pad[0x8C];
    uint32_t value;
};
}

extern "C" uint32_t fn_00376414(Receiver *self) {
    return self->value;
}
