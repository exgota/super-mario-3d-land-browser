#pragma once

#include <LiveActor/alLiveActor.h>

namespace swing_needle_roller
{
class ChainModel;
class SwingState;
class RollerModel;
class ChainGroup;
}

// Layout recovered from constructor 0x00185DFC and its consumers.
// The root class name is provisional; the resource name is SwingNeedleRoller.
class SwingNeedleRoller : public al::LiveActor
{
public:
        virtual void init( const al::ActorInitInfo& info );

private:
        sead::Quatf mInitialQuat;                              // 0x60
        swing_needle_roller::SwingState* mSwingState;           // 0x70
        swing_needle_roller::RollerModel* mRoller;               // 0x74
        al::LiveActor* mLeftHead;                              // 0x78
        al::LiveActor* mRightHead;                             // 0x7C
        swing_needle_roller::ChainGroup* mChains;               // 0x80
        sead::Vector3f mSide;                                  // 0x84
        sead::Vector3f mFront;                                 // 0x90
        float mLowestY;                                       // 0x9C
        float mChainLength;                                   // 0xA0
        float mRollerLength;                                  // 0xA4
        float mRollAngle;                                     // 0xA8
        float mRollSpeed;                                     // 0xAC
        float mChainSideOffset;                               // 0xB0
        u8 mGrounded;                                         // 0xB4
        u8 mFindGround;                                       // 0xB5
};

static_assert( sizeof( SwingNeedleRoller ) == 0xB8, "" );
