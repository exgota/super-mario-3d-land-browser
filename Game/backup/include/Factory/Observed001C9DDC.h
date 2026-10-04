#pragma once

// Partial virtual interfaces; the original class identities are unknown.
class Observed001C9DDCTarget
{
public:
    virtual void unknown00() = 0;
    virtual void unknown04() = 0;
    virtual void unknown08() = 0;
    virtual void unknown0C() = 0;
    virtual void unknown10() = 0;
    virtual void* unknown14() = 0;
};

class Observed001C9DDC
{
public:
    virtual void unknown00() = 0;
    virtual void unknown04() = 0;
    virtual void unknown08() = 0;
    virtual void unknown0C() = 0;
    virtual void unknown10() = 0;
    virtual void unknown14() = 0;
    virtual Observed001C9DDCTarget* unknown18() = 0;
};

extern "C" void* fn_001C9DDC(Observed001C9DDC* self);
