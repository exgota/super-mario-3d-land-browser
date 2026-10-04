#pragma once

// Only the two argument words used by fn_001E2294 are identified.
struct Observed001E2294
{
    unsigned int unknown00;
    unsigned int word04;
    unsigned int word08;
};

extern "C" int fn_001EC91C(unsigned int, unsigned int, int);
extern "C" int fn_001E2294(const Observed001E2294*);
