#pragma once

#include <container/seadPtrArray.h>

// Descriptive layout names; original class identities are unresolved.
struct Observed001B4680Entry
{
    void* value;
};

struct Observed001B4680
{
    sead::PtrArray<Observed001B4680Entry>* entries;
};

extern "C" void* fn_001B4680(Observed001B4680*, unsigned int);
