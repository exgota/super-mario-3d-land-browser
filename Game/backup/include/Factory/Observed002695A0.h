#pragma once

// Minimum observed polymorphic prefix; the original class name is unknown.
struct Observed002695A0;

struct Observed002695A0Dispatch
{
    const void* unknown00[35];
    void (*unknown8C)(Observed002695A0*);
};

struct Observed002695A0
{
    const Observed002695A0Dispatch* dispatch;
};

extern "C" void fn_002695A0(Observed002695A0* self);
