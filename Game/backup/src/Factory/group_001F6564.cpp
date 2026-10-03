#include <stdint.h>

namespace {
struct ObjectView {
    unsigned char pad[0x18];
    uint32_t value;
};
}

extern "C" uint32_t fn_001F6564(ObjectView* self) { return self->value; }
extern "C" uint32_t fn_0035D6E0(ObjectView* self) { return self->value; }
extern "C" uint32_t fn_0036F30C(ObjectView* self) { return self->value; }
extern "C" uint32_t fn_003761A4(ObjectView* self) { return self->value; }
extern "C" uint32_t fn_00376880(ObjectView* self) { return self->value; }
