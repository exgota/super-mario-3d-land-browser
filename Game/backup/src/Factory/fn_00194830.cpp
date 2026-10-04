#include <Nerve/alNerveExecutor.h>

extern "C" void fn_00194850(al::NerveExecutor* self);

extern "C" void fn_00194830(al::NerveExecutor* self,
                            unsigned int valueC, unsigned int value8)
{
    reinterpret_cast<unsigned int*>(self)[3] = valueC;
    reinterpret_cast<unsigned int*>(self)[2] = value8;
    self->updateNerve();
    return fn_00194850(self);
}
