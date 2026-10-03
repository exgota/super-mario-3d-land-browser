#include <cstdint>

namespace {
struct Object {
    std::uint8_t pad[0x88];
    std::uint32_t value;
};
}

extern "C" std::uint32_t fn_0037640C(Object* self) {
    return self->value;
}
