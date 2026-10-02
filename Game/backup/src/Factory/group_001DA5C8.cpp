#include <stdint.h>

namespace {
struct SystemKit;
struct Interface;
}

extern "C" Interface* _ZN18alProjectInterface12getSystemKitEv();
extern "C" void* fn_0024EDCC();
extern "C" void* fn_001DA5DC(void*);
extern "C" void* fn_0024EAEC(void*);
extern "C" void* fn_0024ED90(void*);
extern "C" void* fn_00334E94(void*);

extern "C" void* fn_001DA5C8() {
    return fn_001DA5DC(*(void**)((char*)_ZN18alProjectInterface12getSystemKitEv() + 0x10));
}
extern "C" void* fn_0024EAD8() {
    return fn_0024EAEC(*(void**)((char*)fn_0024EDCC() + 0x10));
}
extern "C" void* fn_0024ED7C() {
    return fn_0024ED90(*(void**)((char*)fn_0024EDCC() + 0x10));
}
extern "C" void* fn_00334E80() {
    return fn_00334E94(*(void**)((char*)_ZN18alProjectInterface12getSystemKitEv() + 0x10));
}
