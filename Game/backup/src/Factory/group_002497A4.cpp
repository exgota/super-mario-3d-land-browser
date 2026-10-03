#include <stdint.h>

namespace {
struct LookupOwner {
    unsigned char reserved[8];
    const uint32_t* entries;
};
}

extern "C" uint32_t fn_002497A4(const LookupOwner* owner, uint32_t index) {
    return owner->entries[index];
}

extern "C" uint32_t fn_00332A3C(const LookupOwner* owner, uint32_t index) {
    return owner->entries[index];
}
