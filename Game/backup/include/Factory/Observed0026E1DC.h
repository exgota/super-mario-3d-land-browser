#pragma once

// Observed interface prefix; the original class and result type are unknown.
struct Observed0026E1DC;

struct Observed0026E1DCVtable
{
    unsigned char unknown00[0x3D8];
    unsigned int (*slot03D8)(Observed0026E1DC*);
};

struct Observed0026E1DC
{
    const Observed0026E1DCVtable* vtable;
};

extern "C" Observed0026E1DC* fn_0026E1DC();
