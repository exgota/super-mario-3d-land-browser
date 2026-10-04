#include <stdint.h>

namespace {
struct NestedValue {
    unsigned char padding[0x14];
    uint32_t value;
};

struct Owner {
    unsigned char padding[0x18];
    NestedValue *nested;
};
}

extern "C" uint32_t fn_0024EB70(Owner *self) {
    return self->nested->value;
}
