#include "Factory/Observed001A4928.h"

extern "C" void fn_001A4928(Observed001A4928* self)
{
    Observed001A4928Target* target = self->owner->target;
    if (target)
        target->unknownSlot04();
}
