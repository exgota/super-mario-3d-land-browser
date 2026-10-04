#pragma once

// Observed polymorphic object prefix; the original class is unresolved.
struct Observed002695B8;

struct Observed002695B8Vtable
{
    const void* unknown00[28];
    void (*unknownSlot70)(Observed002695B8* self);
};

struct Observed002695B8
{
    const Observed002695B8Vtable* table;
};

extern "C" void fn_002695B8(Observed002695B8* self);
