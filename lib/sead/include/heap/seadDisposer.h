#pragma once

#include <container/seadListImpl.h>

namespace sead
{

class Heap;

class IDisposer
{
private:
        Heap* mDisposerHeap;
        ListNode mDisposerNode;

public:
        enum HeapNullOption
        {
                cHeapNullOption_FindContainHeap = 0,
                cHeapNullOption_DoNotAppendDisposer = 1
        };
        IDisposer( Heap* heap = 0, HeapNullOption option = cHeapNullOption_FindContainHeap );
        virtual ~IDisposer();
};

static_assert( sizeof( IDisposer ) == 0x10, "IDisposer target layout" );

} // namespace sead

#define SEAD_SINGLETON_DISPOSER( Class ) \
private: \
        class Disposer : public sead::IDisposer { public: virtual ~Disposer(); }; \
        Disposer mDisposer; \
        static Class* sInstance; \
        static Disposer* sDisposer; \
public: \
        static Class* instance() { return sInstance; } \
        static Class* createInstance( sead::Heap* heap );

#define SEAD_SINGLETON_DISPOSER_IMPL( Class ) \
        Class* Class::sInstance = 0; \
        Class::Disposer* Class::sDisposer = 0;
