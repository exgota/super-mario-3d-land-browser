#pragma once

#include <MapObj/alMapObjActor.h>
#include <math/seadVector.h>

struct TeresaChildAreaGroup
{
        const char* mName;
        void** mAreas;
        int mCount;
        int mCapacity;
};

// Init-only storage view of the independently observed 0xB4-byte allocation.
// Unused fields and the secondary interface stay opaque; this is not a full
// reconstruction of the original class or all of its virtual overrides.
class Teresa : public al::MapObjActor
{
        void* mUnknownInterface60;
        TeresaChildAreaGroup* mChildAreas;
        void* mChildArea;
        sead::Vector3f mHomeTrans;
        sead::Vector3f mUnknown78;
        sead::Vector3f mInitialTrans;
        sead::Vector3f mUnknown90;
        int mCoinCount;
        int mUnknownA0;
        int mKind;
        al::LiveActor* mHost;
        void* mUnknownAC;
        int mArg0;

public:
        virtual void init( const al::ActorInitInfo& info );
};

static_assert( sizeof( Teresa ) == 0xB4, "Teresa retail size" );
