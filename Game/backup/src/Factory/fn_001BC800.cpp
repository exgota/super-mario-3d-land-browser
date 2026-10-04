#include <Factory/ObservedActor001BC800.h>

extern "C" void fn_001BC800(ObservedActor001BC800* self,
                           al::HitSensor* me, al::HitSensor* other)
{
    if (!self->flag84)
        self->actor60->attackSensor(me, other);
}
