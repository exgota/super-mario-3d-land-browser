#include <Factory/Observed001E2294.h>

extern "C" int fn_001E2294(const Observed001E2294* self)
{
    int result = fn_001EC91C(self->word04, self->word08, 1);
    int failure = result & 0x80000000;
    if (failure >= 0)
        result = 0;
    return result;
}
