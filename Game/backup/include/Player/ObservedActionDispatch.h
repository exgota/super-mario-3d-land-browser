#pragma once

namespace observed_action_dispatch {

class Dispatch0C {
public:
    virtual void unknown00() = 0;
    virtual void unknown04() = 0;
    virtual void unknown08() = 0;
    virtual void unknown0C() = 0;
};

struct Prefix0019DD50 {
    unsigned char unknown00[0x14];
    Dispatch0C* dispatch;
};

}

extern "C" void fn_00173EE8(observed_action_dispatch::Prefix0019DD50*);
