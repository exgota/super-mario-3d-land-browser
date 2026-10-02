#include <Layout/alLayoutActor.h>

namespace {

class ResultCoinCallback {
public:
    virtual void unknown() = 0;
    virtual void execute() = 0;
};

struct ResultTimer {
    unsigned int unknown00[4];
    al::LayoutActor* coinLayout;
    al::LayoutActor* timerLayout;
    ResultCoinCallback* coinCallback;
    int remainingTime;
    int unknown20;
    int conversionTicks;
    int unknown28;
    int extraLifeCount;
};

extern "C" void fn_00260984(al::LayoutActor*, const char*, unsigned int);
extern "C" bool fn_00138B68();
extern "C" void* fn_0027109C(al::IUseAudioKeeper*, const sead::SafeString&);
extern "C" int fn_00326DEC();
extern "C" void fn_00270DB0(al::LayoutActor*, const char*, int);
extern "C" void fn_00270CCC(al::LayoutActor*, const sead::SafeString&);

}

extern "C" bool fn_0018C7DC(ResultTimer* self) {
    if (self->remainingTime <= 0)
        return true;

    --self->remainingTime;
    fn_00260984(self->timerLayout, "TxtTime", self->remainingTime);
    ++self->conversionTicks;
    if (self->conversionTicks >= 10) {
        self->conversionTicks = 0;
        if (fn_00138B68()) {
            if (self->extraLifeCount == 1109)
                fn_0027109C(self->coinLayout, sead::SafeString("SeSy1000Up"));
            else if (self->extraLifeCount == 999)
                fn_0027109C(self->coinLayout, sead::SafeString("SeSy1000Up"));
            else
                fn_0027109C(self->coinLayout, sead::SafeString("SeSy1Up"));
            ++self->extraLifeCount;
            self->coinCallback->execute();
        }
        fn_00270DB0(self->coinLayout, "TxtCoin", fn_00326DEC());
        if (fn_00326DEC() & 1)
            fn_00270CCC(self->coinLayout, sead::SafeString("ResultFlash"));
        fn_0027109C(self->coinLayout, sead::SafeString("SeSyCoinCountUpResult"));
    }

    if (self->remainingTime <= 0) {
        fn_0027109C(self->timerLayout, sead::SafeString("SeSyTimerCountDownResultEnd"));
        return true;
    }
    return false;
}
