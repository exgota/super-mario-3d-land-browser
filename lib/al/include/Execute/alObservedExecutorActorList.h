#pragma once

#include <Execute/alExecutorRegistrationList.h>

namespace al
{

// Observed storage prefix; the concrete executor-list identity is unresolved.
struct ObservedExecutorActorList : ExecutorRegistrationList
{
    int mCapacity;
    int mSize;
    void** mBuffer;
};

extern "C" ObservedExecutorActorList* fn_001E2BCC(ObservedExecutorActorList* list);
extern "C" ObservedExecutorActorList* fn_001A2F10(ObservedExecutorActorList* list);
extern "C" const unsigned char dat_003D0384;

} // namespace al
