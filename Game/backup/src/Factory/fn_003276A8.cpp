#include <stdint.h>

namespace {
struct Record {
    uint8_t pad[0xa4];
    float value;
};
}

extern "C" float fn_003276A8(const Record *self) {
    return self->value;
}
