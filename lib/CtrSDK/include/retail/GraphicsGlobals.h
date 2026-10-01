#pragma once
#include <nn/types.h>

// Descriptive ABI names; the full pointed allocations remain unrecovered.
namespace retail_graphics {
struct ProgramContext;
struct RenderControl;
struct ContextSlots {
    unsigned int unknown0[2];
    ProgramContext* current;
    unsigned int geometryEnabled;
};
#ifdef __arm__
static_assert_(sizeof(ContextSlots) == 16);
static_assert_(offsetof(ContextSlots, current) == 8);
static_assert_(offsetof(ContextSlots, geometryEnabled) == 12);
#endif
}
extern "C" retail_graphics::ContextSlots dat_003E2E40;
extern "C" retail_graphics::RenderControl* dat_003E3154;
