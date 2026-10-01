#pragma once

#include <heap/seadHeap.h>

namespace sead
{

class HeapMgr
{
public:
        static s32 getRootHeapNum();
        static Heap* getRootHeap( s32 index );
};

} // namespace sead
