#pragma once

// Storage observed in the initializer at 0x001CDC9C; class identity unresolved.
struct Observed001CDC9C
{
    unsigned int mValue00;
    unsigned int mValue04;
    int mIndex08;
    bool mFlag0C;
    unsigned int mValue10;
};

extern "C" void fn_001CDC9C(Observed001CDC9C* object);
