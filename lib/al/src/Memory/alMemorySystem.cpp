#include <Memory/alMemorySystem.h>
#include <Resource/alResource.h>
#include <System/alSystemKit.h>
#include <Util/alStringUtil.h>
#include <Yaml/alByamlIter.h>

namespace al
{

void MemorySystem::createSequenceHeap()
{
        mSequenceHeap = sead::ExpHeap::create( 0, "SequenceHeap", nullptr, sead::ExpHeap::cHeapDirection_Forward, false );
}

// Retail helper: create the frame heap and clear bit 2 of its flags at +0x6c.
extern "C" void fn_002911e8( sead::FrameHeap** out, u32 heapSize, const char* name,
        sead::Heap* parent, bool enableLock, sead::Heap::HeapDirection direction );

extern "C" const char dat_003A2230[]; // ObjectData/GameSystemDataTable
extern "C" const char dat_003A2220[]; // HeapSizeDefine
extern "C" const char dat_003A2250[]; // Stage
extern "C" const char dat_003A2258[]; // SceneResource
extern "C" const char dat_003A2268[]; // SceneHeapResource

#ifdef NON_MATCHING
static inline u32 calcSceneResourceHeapSize( u32 defaultSize, const char* stageName )
{
        if ( !stageName )
                return defaultSize;
        al::Resource* gameSystemDataTable =
                al::findOrCreateResource( dat_003A2230 );
        const u8*     tableData = gameSystemDataTable->getByml( dat_003A2220 );
        al::ByamlIter table( tableData );
        for ( int i = 0; i < table.getSize(); i++ )
        {
                al::ByamlIter entry;
                table.tryGetIterByIndex( &entry, i );
                const char* stage = nullptr;
                entry.tryGetStringByKey( &stage, dat_003A2250 );
                if ( al::isEqualString( stage, stageName ) )
                {
                        float resourceMb = 0;
                        entry.tryGetFloatByKey( &resourceMb, dat_003A2258 );
                        return resourceMb * 1024 * 1024;
                }
        }
        return defaultSize;
}

void MemorySystem::createSceneResourceHeap( const char* stageName )
{
        u32 heapSize = calcSceneResourceHeapSize( 8 * 1024 * 1024, stageName );
        fn_002911e8( &mSceneResourceHeap, heapSize, dat_003A2268, nullptr, true,
                sead::Heap::cHeapDirection_Forward );
}
#endif

void MemorySystem::freeAllSequenceHeap()
{
        mSequenceHeap->freeAll();
}

sead::ExpHeap* getStationedHeap()
{
        return alProjectInterface::getSystemKit()->getMemorySystem()->getStationedHeap();
}

sead::ExpHeap* getSequenceHeap()
{
        return alProjectInterface::getSystemKit()->getMemorySystem()->getSequenceHeap();
}

sead::FrameHeap* getSceneResourceHeap()
{
        return alProjectInterface::getSystemKit()->getMemorySystem()->getSceneResourceHeap();
}

sead::FrameHeap* getCourseSelectHeap()
{
        return alProjectInterface::getSystemKit()->getMemorySystem()->getCourseSelectHeap();
}

bool isCreatedSceneResourceHeap()
{
        return getSceneResourceHeap() != nullptr;
}

} // namespace al
