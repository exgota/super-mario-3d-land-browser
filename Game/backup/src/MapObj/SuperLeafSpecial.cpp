#include <MapObj/SuperLeafSpecial.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerveFunction.h>
#include <Nerve/alNerve.h>
#include <prim/seadSafeString.h>

#include <MapObj/FlowerInitState.h>
#include <MapObj/FlowerInitNerve.h>

extern "C" void* fn_002801B8(al::LiveActor*);

void SuperLeafSpecial::init(const al::ActorInitInfo& info)
{
    al::initActorWithArchiveName(this, info, sead::SafeString("SuperLeafSpecial"), 0);
    al::initNerve(this, &dat_003F16EC, 5);
    mAppearParam = new FlowerInit::Param;
    mItemControl = fn_002801B8(this);
    al::initNerveState(this, new FlowerInit::Appear(this), &dat_003F16F0, "\x82\xbb\x82\xcc\x8f\xea\x8f\x6f\x8c\xbb");
    al::initNerveState(this, new FlowerInit::Forward(this, mAppearParam, true, 0), &dat_003F16F8, "\x92\xb5\x82\xcb\x8f\xe3\x82\xb0\x8f\x6f\x8c\xbb(\x91\x4f\x95\xfb)");
    al::initNerveState(this, new FlowerInit::Vertical(this, mAppearParam, true, 0), &dat_003F16F4, "\x92\xb5\x82\xcb\x8f\xe3\x82\xb0\x8f\x6f\x8c\xbb(\x90\x5e\x8f\xe3)");
    al::initNerveState(this, new FlowerInit::Release(this, mAppearParam), &dat_003F16FC, "\x8e\xe6\x82\xe8\x8f\x6f\x82\xb5\x8f\x6f\x8c\xbb");
    al::initNerveState(this, new FlowerInit::Ground(this, mAppearParam), &dat_003F1700, "\x90\xda\x92\x6e");
    makeActorAppeared();
}
