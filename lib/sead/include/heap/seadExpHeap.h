#pragma once

#include <heap/seadHeap.h>
#include <prim/seadSafeString.h>

namespace sead
{

class ExpHeap : public Heap
{
public:
        static ExpHeap* create( u32 size, const SafeString& name, Heap* parent,
                HeapDirection direction, bool enableLock );
};

} // namespace sead
