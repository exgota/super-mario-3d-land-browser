#pragma once

#include <nn/types.h>

namespace sead
{

// Only the dispatch prefix required by the current game sources is recovered.
class Heap
{
public:
        enum HeapDirection
        {
                cHeapDirection_Forward = 1,
                cHeapDirection_Reverse = -1
        };
        virtual ~Heap();
private:
        virtual void reservedVirtualFunction08();
        virtual void reservedVirtualFunction0C();
        virtual void reservedVirtualFunction10();
        virtual void reservedVirtualFunction14();
        virtual void reservedVirtualFunction18();
        virtual void reservedVirtualFunction1C();
        virtual void reservedVirtualFunction20();
public:
        virtual void freeAll();
};

class ScopedCurrentHeapSetter
{
private:
        Heap* mPreviousHeap;
        Heap* mCurrentHeap;

public:
        explicit ScopedCurrentHeapSetter( Heap* heap );
        ~ScopedCurrentHeapSetter();
};

} // namespace sead
