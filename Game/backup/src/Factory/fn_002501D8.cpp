#include <cstdint>

namespace {
struct LookupObject {
    std::uint32_t reserved[5];
    const std::uint32_t *values;
};
}

extern "C" std::uint32_t fn_002501D8(const LookupObject *object, std::uint32_t index) {
    return object->values[index];
}
