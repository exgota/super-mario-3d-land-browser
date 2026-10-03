#include <stdint.h>

namespace {
struct Receiver;
}

extern "C" uint32_t fn_001C0FB4(uint32_t);
extern "C" uint32_t fn_001C1EF0(uint32_t);
extern "C" uint32_t fn_001C1F58(uint32_t);
extern "C" uint32_t fn_0024CC9C(uint32_t);

extern "C" uint32_t fn_001C0FAC(Receiver *self) {
    return fn_001C0FB4(*(uint32_t *)((char *)self + 0x44));
}
extern "C" uint32_t fn_001C1EE8(Receiver *self) {
    return fn_001C1EF0(*(uint32_t *)((char *)self + 0x44));
}
extern "C" uint32_t fn_001C1F50(Receiver *self) {
    return fn_001C1F58(*(uint32_t *)((char *)self + 0x44));
}
extern "C" uint32_t fn_0024CC94(Receiver *self) {
    return fn_0024CC9C(*(uint32_t *)((char *)self + 0x44));
}
