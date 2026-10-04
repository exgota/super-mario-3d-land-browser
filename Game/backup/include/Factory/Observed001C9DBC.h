#pragma once

// Minimum virtual interfaces observed at 0x001C9DBC; identities are unknown.
class Observed001C9DBCTarget
{
public:
    virtual void unknown00() = 0;
    virtual void unknown04() = 0;
    virtual void unknown08() = 0;
};

class Observed001C9DBCOwner
{
public:
    virtual void unknown00() = 0;
    virtual void unknown04() = 0;
    virtual void unknown08() = 0;
    virtual void unknown0C() = 0;
    virtual void unknown10() = 0;
    virtual void unknown14() = 0;
    virtual Observed001C9DBCTarget* unknown18() = 0;
};

extern "C" void fn_001C9DBC(Observed001C9DBCOwner* self);
