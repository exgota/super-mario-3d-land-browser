#pragma once

#include <Player/ObservedSquatAction.h>

// Minimum accessed prefix; the original class identity is unknown.
// Predicate08 supplies the existing neutral declaration for a Boolean slot +0x8.
struct Observed001B9994
{
    unsigned char unknown00[4];
    observed_squat::Predicate08* target;
    bool result;
};

extern "C" void fn_001B9994(Observed001B9994* self);
