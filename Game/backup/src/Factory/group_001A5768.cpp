#include <stdint.h>

namespace {
typedef uint32_t (*Next)(uint32_t);
extern "C" uint32_t fn_002913CC();
extern "C" uint32_t fn_001A577C(uint32_t);
extern "C" uint32_t fn_001A57F0(uint32_t);
extern "C" uint32_t fn_001A5B30(uint32_t);
extern "C" uint32_t fn_001A5BB0(uint32_t);
extern "C" uint32_t fn_001D18D0(uint32_t);
extern "C" uint32_t fn_001D1944(uint32_t);
extern "C" uint32_t fn_00240F90(uint32_t);
extern "C" uint32_t fn_0032E118(uint32_t);
extern "C" uint32_t fn_00333D20(uint32_t);
}

namespace alProjectInterface { uint32_t getSystemKit(); }

#define BODY(NAME, GET, NEXT) \
extern "C" uint32_t NAME() { return NEXT(GET() + 12); }
#define BODY_INDIRECT(NAME, GET, NEXT) \
extern "C" uint32_t NAME() { return NEXT(*(uint32_t *)(GET() + 12)); }

BODY_INDIRECT(fn_001A5768, fn_002913CC, fn_001A577C)
BODY_INDIRECT(fn_001A57DC, fn_002913CC, fn_001A57F0)
BODY_INDIRECT(fn_001A5B1C, fn_002913CC, fn_001A5B30)
BODY_INDIRECT(fn_001A5B9C, fn_002913CC, fn_001A5BB0)
BODY_INDIRECT(fn_001D18BC, alProjectInterface::getSystemKit, fn_001D18D0)
BODY_INDIRECT(fn_001D1930, alProjectInterface::getSystemKit, fn_001D1944)
BODY_INDIRECT(fn_00240F7C, alProjectInterface::getSystemKit, fn_00240F90)
BODY_INDIRECT(fn_0032E104, fn_002913CC, fn_0032E118)
BODY_INDIRECT(fn_00333D0C, alProjectInterface::getSystemKit, fn_00333D20)
