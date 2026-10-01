#pragma once

// Clean-room layouts observed in EU 0x0022F804 and constructor 0x0023062C.
// Names describe observed storage, not an identified SDK class.
namespace dot22f804 {
typedef unsigned int Word;
struct Allocator;
struct AllocatorVtable {
    void* unknown00;
    void* unknown04;
    Word* (*allocate)(Allocator*, Word bytes, Word alignment);
    void (*deallocate)(Allocator*, Word*);
};
struct Allocator { const AllocatorVtable* vtable; };
struct WordArray {
    Allocator* allocator;
    Word* begin;
    Word* end;
    Word capacityAndFlags;
};
struct Resource {
    Word unknown00[4];
    int valueCount; // +0x10
    Word unknown14;
    int selectorCount; // +0x18
    int selectorsRelative; // +0x1c, relative to this field
};
struct Storage {
    const void* vtable;
    Allocator* allocator;
    const Resource* resource;
    WordArray selectors; // +0x0c
    Word unknown1c;
    WordArray values0; // +0x20
    WordArray values1; // +0x30
    WordArray values2; // +0x40
    WordArray optionalValues; // +0x50
};
struct Descriptor {
    const void* vtable;
    unsigned char flag4, flag5;
    unsigned char padding[2];
};
// Only the observed 16-byte prefix at 0x0042A4B0 is known.
// Neither the surrounding BSS allocation extent nor its ownership is established.
struct DescriptorPairPrefix { Descriptor base; Descriptor extra; };
}
extern "C" unsigned int fn_0022F804(dot22f804::Storage*, bool includeOptional);
