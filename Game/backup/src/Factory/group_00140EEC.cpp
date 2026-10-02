#include <stdint.h>

namespace {
extern "C" const uint32_t dat_003E2940[];
extern "C" const uint32_t dat_003EF624[];
extern "C" const uint32_t sPhotoScenarioNames[];
extern "C" const uint32_t dat_003F01C0[];
extern "C" const uint32_t dat_003F0224[];
}

extern "C" uint32_t fn_00140EEC(uint32_t index) { return dat_003E2940[index]; }
extern "C" uint32_t fn_001B6424(uint32_t index) { return dat_003EF624[index]; }
extern "C" uint32_t fn_0025EB88(uint32_t index) { return sPhotoScenarioNames[index]; }
extern "C" uint32_t fn_002CCBA4(uint32_t index) { return dat_003F01C0[index]; }
extern "C" uint32_t fn_002CCBB4(uint32_t index) { return dat_003F0224[index]; }
