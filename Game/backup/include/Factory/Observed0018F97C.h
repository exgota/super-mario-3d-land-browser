#pragma once

// Minimum observed object prefix; the original class identity is unknown.
struct Observed0018F97C
{
    const void* table;
    void* argument;
    bool flag08;
    bool flag09;
};

extern "C" const unsigned char dat_003CE96C;
extern "C" void fn_0018F97C(Observed0018F97C* self, void* argument);
