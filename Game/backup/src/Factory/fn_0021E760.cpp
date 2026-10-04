#include <stdint.h>

extern "C" uintptr_t fn_0021E760(void* self) {
    typedef uintptr_t (*Dispatch)(void*);
    Dispatch* methods = *static_cast<Dispatch**>(self);
    return methods[0x40 / sizeof(Dispatch)](self);
}
