#include <stdint.h>

namespace {
struct LookupOwner {
    unsigned char reserved[0x68];
    const uint32_t *entries;
};
}

extern "C" uint32_t fn_00257784(const LookupOwner *owner, uint32_t index) {
    return owner->entries[index];
}
