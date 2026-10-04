#pragma once
#include <Player/PlayerAnimator.h>
#include <nn/types.h>

namespace observed_squat {
// Unknown slots are placeholders only; original interface identities are unproved.
class Predicate08 {
public:
    virtual void unknown00() = 0;
    virtual void unknown04() = 0;
    virtual bool test() = 0;
};
class Predicate24 {
public:
    virtual void unknown00() = 0;
    virtual void unknown04() = 0;
    virtual void unknown08() = 0;
    virtual void unknown0C() = 0;
    virtual void unknown10() = 0;
    virtual void unknown14() = 0;
    virtual void unknown18() = 0;
    virtual void unknown1C() = 0;
    virtual void unknown20() = 0;
    virtual bool test() = 0;
};
struct ServicePrefix {
    u8 unknown00[4];
    IUsePlayerAnimator* animator;
    u8 unknown08[8];
    Predicate24* predicate24;
    Predicate08* predicate08;
};
// Constructor 0019E5D4 allocates exactly eight bytes for this record.
struct StateRecord {
    u32 value;
    bool changed;
    u8 unknown05[3];
    void set(u32 next) {
        if (value!=next) { value=next; changed=true; }
    }
};
union ProgressValue { float value; u32 bits; };
// This is a minimum accessed prefix, not the original complete action class.
struct ActionPrefix {
    u8 unknown00[8];
    ServicePrefix* services;
    u8 unknown0C[8];
    ProgressValue progress;
    StateRecord* state;
    u8 unknown1C[4];
    u32 counter;
};
typedef char StateSize[(sizeof(StateRecord)==8)?1:-1];
typedef char StateOffset[(offsetof(ActionPrefix,state)==0x18)?1:-1];
typedef char CounterOffset[(offsetof(ActionPrefix,counter)==0x20)?1:-1];
}
