#pragma once

#include <MapObj/alMapObjActor.h>

namespace al { class LiveActorGroup; }
class BlockDragonSegment;

// Recovered from the retail constructor at 0x00193484 and its independent
// movement/clipping consumers. Unknown fields retain offset names.
class BlockDragonGenerator : public al::MapObjActor
{
private:
        void*                   _60;
        al::LiveActorGroup*      mSegments;       // 0x64
        BlockDragonSegment*     mHead;           // 0x68
        BlockDragonSegment*     mTail;           // 0x6C
        int                     mBodyCount;      // 0x70
        float                   mSegmentSpacing; // 0x74
        float                   mMoveSpeed;      // 0x78, Arg0
        al::LiveActor*          mLeadingActor;   // 0x7C
        u8                      _80[4]; // type unknown; storage between observed fields
        bool                    _84;
        bool                    _85;
        int                     mBodyTypes[7];   // 0x88, Arg1 through Arg7
        sead::Vector3f          mPlacementTrans; // 0xA4
        sead::Vector3f          mClippingCenter; // 0xB0
        float                   mShadowLength;   // 0xBC, Arg8
        float                   mShadowScale;    // 0xC0, percentage

public:
        virtual void init( const al::ActorInitInfo& info );
        virtual void startClipped();
        virtual void endClipped();

        void startAppear();
        BlockDragonGenerator( const sead::SafeString& name );
};

static_assert( sizeof( BlockDragonGenerator ) == 0xC4, "retail allocation and constructor" );
