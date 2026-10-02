// Clean-room layouts observed in EU 0x0028F9DC and its direct callees.
#pragma once
#include <nn/types.h>

namespace retail_display {
typedef void* (*Allocate)(u32 area, u32 alignment, u32 user, u32 bytes);
typedef void (*Release)(u32 area, u32 alignment, u32 user, void* memory);
struct CommandList {
    u32 id;
    u8* buffer;
    u32 capacity;
    u32 used;
    u32 unknown10;
    u32 unknown14;
    u8* records;
    u32 recordCapacity;
    s32 recordCount;
    s32 submittedCount;
    s32 processedCount;
    u32 mode;
    u32 unknown30;
    u32 unknown34;
    CommandList* next;
};
struct Context {
    u32 width0, width1, height0, height1;
    u8 initialized, busy, submitting, unknown13;
    u32 speculativeRequests;
    u8 cancelPending;
    u8 unknown19[3];
    CommandList* buckets[32];
    CommandList* current;
    CommandList* active;
    u8 unknownA4[0x164 - 0xa4];
    u32 flags;
    u8 unknown168[0x180 - 0x168];
};
struct AllocatorState {
    Allocate allocate;
    Release release;
    u32 enabled0, enabled1;
    void* managers;
};
}
// Observed access extents, not independently established object boundaries.
// The context is missing from the map; the allocator crosses two existing rows.
extern "C" retail_display::Context dat_0041CFA0;
extern "C" retail_display::AllocatorState dat_003E2654;
extern "C" u8* dat_003E2E30;
extern "C" u8* dat_003E2E34;
extern "C" int fn_0028F9DC(retail_display::Allocate, retail_display::Release);
