#pragma once

#include <MapObj/alMapObjActor.h>

// Retail 0x0012E234 initializes these fields after MapObjActor construction.
// The field at 0x64 is not used by init and its role remains unknown.
class KoopaPillar : public al::MapObjActor
{
        al::LiveActor* mBaseModel;
        void* mUnknown64;
        al::LiveActor* mBreakModel;
        int mItemType;
        int mPillarType;
        bool mArg1;

public:
        virtual void init( const al::ActorInitInfo& info );
};

static_assert( sizeof( KoopaPillar ) == 0x78, "KoopaPillar retail size" );
