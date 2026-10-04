#pragma once

// Observed prefixes; the original class identities are unknown.
class Observed001A4928Target
{
public:
    virtual void unknownSlot00() = 0;
    virtual void unknownSlot04() = 0;
};

struct Observed001A4928Owner
{
    unsigned char unknown00[0x1C];
    Observed001A4928Target* target;
};

struct Observed001A4928
{
    const void* table;
    Observed001A4928Owner* owner;
};

extern "C" void fn_001A4928(Observed001A4928* self);
