#include <stdint.h>

namespace {
struct FlagOwner {
    unsigned char pad[0x28];
    uint32_t flags;
};
}

extern "C" uint32_t fn_0021D760(FlagOwner *self) {
    return self->flags & 1;
}
