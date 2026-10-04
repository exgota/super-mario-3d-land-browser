#pragma once

// Minimum dispatch prefixes; the original class identity is unresolved.
struct Observed0022AC44;

struct Observed0022AC44Table
{
    void (*unknown00[5])();
    unsigned int (*slot14)(Observed0022AC44* self, unsigned int argument);
};

struct Observed0022AC44
{
    const Observed0022AC44Table* table;
};

extern "C" unsigned int fn_0022AC44(Observed0022AC44* self, unsigned int argument);
