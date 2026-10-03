#include <LiveActor/alActorTimer.h>
#include <LiveActor/alLiveActor.h>
#include <LiveActor/alLiveActorFunction.h>
#include <prim/seadSafeString.h>

extern "C" signed char readActorTimerDeadFlag(const al::LiveActor*);
extern "C" signed char readLiveActorClippingFlag(const al::LiveActor*);
extern "C" signed char readLiveActorModelHiddenFlag(const al::LiveActor*);
extern "C" void fn_001BF140(al::IUseAudioKeeper*, const sead::SafeString&, int);
extern "C" void fn_001BF250(al::IUseAudioKeeper*, const sead::SafeString&);
extern "C" void fn_0026a9fc(al::LiveActor*);
extern "C" bool fn_00268df8(al::IUseAudioKeeper*, const sead::SafeString&);
extern "C" const char dat_003B2AB0[];
extern "C" const char dat_003B2AA8[];

void al::ActorTimer::update() {
    if (mStopped)
        return;

    --mRemainingFrames;
    if (mSoundEnabled) {
        if (mRemainingFrames > 90)
            fn_001BF140(mActor, "TimerNormal", 2);
        else
            fn_001BF140(mActor, "TimerFast", 2);
    }
    if (mRemainingFrames <= 0 || readLiveActorClippingFlag(mActor) || readActorTimerDeadFlag(mActor)) {
        mStopped = true;
        mRemainingFrames = 0;
        if (mBlinkEnabled && !readActorTimerDeadFlag(mActor) && !readLiveActorClippingFlag(mActor) &&
            readLiveActorModelHiddenFlag(mActor))
            if (readLiveActorModelHiddenFlag(mActor))
                fn_0026a9fc(mActor);
        if (mSoundEnabled)
            fn_001BF250(mActor, dat_003B2AB0);
        return;
    }
    if (mRemainingFrames <= mBlinkStartFrames && mBlinkEnabled) {
        int period;
        if (mFixedBlinkPeriod)
            period = 4;
        else
            period = mRemainingFrames < 45 ? 3 : 5;
        if ((mRemainingFrames / period) & 1) {
            if (readLiveActorModelHiddenFlag(mActor))
                fn_0026a9fc(mActor);
            mWasShown = true;
        } else {
            if (!readLiveActorModelHiddenFlag(mActor))
                al::hideModel(mActor);
            if (mWasShown) {
                fn_00268df8(mActor, dat_003B2AA8);
                mWasShown = false;
            }
        }
    }
}
