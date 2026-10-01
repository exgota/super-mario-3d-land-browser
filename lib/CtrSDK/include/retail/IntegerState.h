// Layout and ABI recovered from retail EU 0x00206474 and its direct callers.
#pragma once
#include <retail/FloatState.h>

// This 12-byte BSS record is observed at 0x0042013C, outside the current map.
// A separate main-owned data identity is needed before canonical resolution.
extern "C" retail_float_state::LocationRecord dat_0042013C;
extern "C" const u32 dat_003A480C[8];
extern "C" void fn_00206474(u32 location, const s32* values, int width, int count);
