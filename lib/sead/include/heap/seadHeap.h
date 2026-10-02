#pragma once

#include <nn/types.h>
#include <container/seadOffsetList.h>
#include <heap/seadDisposer.h>
#include <prim/seadSafeString.h>

namespace sead
{

// The EU base constructor and FrameHeap creator establish this 0x70-byte layout.
// Lock storage and virtual slots without independent identities stay opaque.
class Heap : public IDisposer
{
private:
        SafeString mName;
        void* mStartAddress;
        u32 mSize;
        Heap* mParent;
        OffsetList<Heap> mChildren;
        ListNode mChildNode;
        OffsetList<IDisposer> mDisposers;
        u8 mDirectionEncoding;
        u8 mReservedBytes4D[ 3 ];
        u32 mCriticalSectionStorage[ 7 ];
        u32 mFlags;

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

static_assert( sizeof( Heap ) == 0x70, "Heap target layout" );

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
