#pragma once

namespace al {
class LiveActor;

// Actor timer layout established by 00270F00 and the update entry 001BFE60.
class ActorTimer {
public:
    virtual void update();

private:
    LiveActor* mActor;
    bool mBlinkEnabled;
    bool mStopped;
    bool mFixedBlinkPeriod;
    int mRemainingFrames;
    int mBlinkStartFrames;
    bool mWasShown;
    bool mSoundEnabled;
};

static_assert(sizeof(ActorTimer) == 0x18, "ActorTimer layout");
}
