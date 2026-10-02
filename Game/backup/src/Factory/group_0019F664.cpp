#include <stdint.h>

namespace {
struct Record {
    volatile uint32_t type;
    volatile uint32_t value;
    uint64_t tail;
};
}

#define DECLARE_DATA(name) extern "C" uint32_t name
DECLARE_DATA(dat_003CFCE0);
DECLARE_DATA(dat_003D0984);
DECLARE_DATA(dat_003D0C00);
DECLARE_DATA(dat_003D1EAC);
DECLARE_DATA(dat_003D1724);
DECLARE_DATA(dat_003D18C4);
DECLARE_DATA(dat_003D199C);
DECLARE_DATA(dat_003D19B4);
DECLARE_DATA(dat_003D19E4);
DECLARE_DATA(dat_003D1A5C);
DECLARE_DATA(dat_003D1B94);
DECLARE_DATA(dat_003D1BDC);
DECLARE_DATA(dat_003D1EF4);

#define DEFINE_FN(name, data) \
extern "C" void name(Record* self, uint32_t value, uint64_t pair) { \
    uint32_t tag = (uint32_t)&data; \
    self->value = value; \
    self->type = tag; \
    self->tail = pair; \
}

DEFINE_FN(fn_0019F664, dat_003CFCE0)
DEFINE_FN(fn_001A8378, dat_003D0984)
DEFINE_FN(fn_001B4BDC, dat_003D0C00)
DEFINE_FN(fn_00251EE4, dat_003D1EAC)
DEFINE_FN(fn_00251F14, dat_003D1724)
DEFINE_FN(fn_002D2A84, dat_003D18C4)
DEFINE_FN(fn_002D2CC8, dat_003D199C)
DEFINE_FN(fn_002D2D34, dat_003D19B4)
DEFINE_FN(fn_002D30E4, dat_003D19E4)
DEFINE_FN(fn_002D34C8, dat_003D1A5C)
DEFINE_FN(fn_002D3B24, dat_003D1B94)
DEFINE_FN(fn_002D3C40, dat_003D1BDC)
DEFINE_FN(fn_002D59E0, dat_003D1EF4)
