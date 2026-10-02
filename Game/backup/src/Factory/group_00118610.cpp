#include <stdint.h>

namespace {
struct State {
    uint8_t pad[0x60];
    int32_t value;
};
}

extern "C" void fn_00118610(State* self) {
    if (self->value > 0) {
        --self->value;
    }
}

extern "C" void fn_00181058(State* self) {
    if (self->value > 0) {
        --self->value;
    }
}

extern "C" void fn_00316740(State* self) {
    if (self->value > 0) {
        --self->value;
    }
}
