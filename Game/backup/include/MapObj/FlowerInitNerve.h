#pragma once

#include <Nerve/alNerve.h>

// Each observed object installs a distinct virtual table. The address parameter
// gives each import its own concrete type without inventing original class names.
// Only execute is overridden; every observed executeOnEnd uses the base method.
template<unsigned int ObjectAddress>
struct FlowerInitNerve : public al::Nerve
{
        virtual void execute( al::NerveKeeper* ) const;
};

extern "C"
{
    extern const FlowerInitNerve<0x003F15D0> dat_003F15D0;
    extern const FlowerInitNerve<0x003F15D4> dat_003F15D4;
    extern const FlowerInitNerve<0x003F15D8> dat_003F15D8;
    extern const FlowerInitNerve<0x003F15DC> dat_003F15DC;
    extern const FlowerInitNerve<0x003F15E0> dat_003F15E0;
    extern const FlowerInitNerve<0x003F15E4> dat_003F15E4;
    extern const FlowerInitNerve<0x003F15A8> dat_003F15A8;
    extern const FlowerInitNerve<0x003F15AC> dat_003F15AC;
    extern const FlowerInitNerve<0x003F15B0> dat_003F15B0;
    extern const FlowerInitNerve<0x003F15B4> dat_003F15B4;
    extern const FlowerInitNerve<0x003F15B8> dat_003F15B8;
    extern const FlowerInitNerve<0x003F15BC> dat_003F15BC;
    extern const FlowerInitNerve<0x003F16EC> dat_003F16EC;
    extern const FlowerInitNerve<0x003F16F0> dat_003F16F0;
    extern const FlowerInitNerve<0x003F16F4> dat_003F16F4;
    extern const FlowerInitNerve<0x003F16F8> dat_003F16F8;
    extern const FlowerInitNerve<0x003F16FC> dat_003F16FC;
    extern const FlowerInitNerve<0x003F1700> dat_003F1700;
}
