#include <stdint.h>

extern "C" void* fn_002448A0(void*);

extern "C" void* fn_00244898(void* actor) {
    return fn_002448A0(*reinterpret_cast<void**>(static_cast<unsigned char*>(actor) + 0x30));
}
