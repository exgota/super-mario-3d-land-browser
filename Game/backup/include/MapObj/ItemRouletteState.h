#pragma once

#include <Nerve/alNerveStateBase.h>
#include <math/seadVector.h>

namespace al { class ActorInitInfo; class LiveActor; }

// Descriptive reconstruction name. Retail class spelling is not established.
// The two allocating callers request 0x40 bytes; see roulette-state-init.md.
class ItemRouletteState : public al::NerveStateBase
{
private:
        al::LiveActor* mHost;                  // +0x0c
        int mSelectedItem;                    // +0x10, starts at one
        int mPlacementArg;                    // +0x14, Arg7 then Arg6
        bool mHasArg7;                        // +0x18
        int mCounter;                         // +0x1c
        al::LiveActor* mItems[5];              // +0x20..+0x30
        sead::Vector3f mPlacementOffset;       // +0x34..+0x3c

public:
        ItemRouletteState( al::LiveActor* host, const al::ActorInitInfo& info,
                bool omitPlacementOffset );
        virtual void init();                  // retail 0x0018730c; not reconstructed here
};

static_assert_( sizeof( ItemRouletteState ) == 0x40 );
