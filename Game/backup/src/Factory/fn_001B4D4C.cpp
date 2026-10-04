#include "Factory/Observed0026E1DC.h"

extern "C" void fn_001B4D4C(unsigned int* result)
{
    Observed0026E1DC* object = fn_0026E1DC();
    *result = object->vtable->slot03D8(object);
}
