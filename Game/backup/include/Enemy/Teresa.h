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

// The constructor at 0x0026D958 establishes this 0xB4-byte layout.
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
