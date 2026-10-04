#include <Factory/Observed001B4680.h>

extern "C" void* fn_001B4680(Observed001B4680* self, unsigned int index)
{
    return self->entries->at(index)->value;
}
