#pragma once

#include <MapObj/alMapObjActor.h>
#include <LiveActor/alLiveActorGroup.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Placement/alPlacementFunction.h>

class BeatBlock : public al::MapObjActor
{
public:
        int beatIndex;
        bool state;
        BeatBlock( const sead::SafeString& name );
};
static_assert_( sizeof( BeatBlock ) == 0x68 );

class BeatBlockGroup : public al::LiveActorGroup
{
public:
        BeatBlockGroup( int count )
            : LiveActorGroup( "\x83\x72\x81\x5b\x83\x67\x83\x75\x83\x8d\x83\x62\x83\x4e\x8a\xc7\x97\x9d", count ) {}
};

class BeatBlockHolder : public al::MapObjActor
{
        BeatBlockGroup* blocks;
        int maximumBeatIndex;
        int currentBeatIndex;
        int currentStep;
        int interval;
        bool enabled;
        bool active;
        unsigned char padding[2];
        void* state;
public:
        virtual void init( const al::ActorInitInfo& info );
};
static_assert_( sizeof( BeatBlockHolder ) == 0x7C );
