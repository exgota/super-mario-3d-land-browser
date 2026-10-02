#include <stdint.h>

extern "C" int fn_00154F54(void*);

extern "C" int fn_00154F4C(void* arg) {
    return fn_00154F54(*(void**)((char*)arg + 0x38));
}
