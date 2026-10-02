#include <MapObj/FireFlower.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerveFunction.h>
#include <Nerve/alNerve.h>
#include <prim/seadSafeString.h>

namespace FlowerInit {
class Param {
    unsigned char storage[0x20];
public:
    Param();
};
class Appear : public al::NerveStateBase {
    unsigned char storage[4];
public:
    Appear(al::LiveActor*);
};
class Forward : public al::NerveStateBase {
    unsigned char storage[0x18];
public:
    Forward(al::LiveActor*, Param*, bool, int);
};
class Vertical : public al::NerveStateBase {
    unsigned char storage[0xc];
public:
    Vertical(al::LiveActor*, Param*, bool, int);
};
class Release : public al::NerveStateBase {
    unsigned char storage[0x18];
public:
    Release(al::LiveActor*, Param*);
};
class Ground : public al::NerveStateBase {
    unsigned char storage[0x1c];
public:
    Ground(al::LiveActor*, Param*);
};
static_assert(sizeof(Appear)==0x10 && sizeof(Forward)==0x24 && sizeof(Vertical)==0x18 && sizeof(Release)==0x24 && sizeof(Ground)==0x28, "Retail allocation sizes");
}
extern "C" void* fn_002801B8(al::LiveActor*);
class FlowerInitNerve : public al::Nerve { public: virtual void execute(al::NerveKeeper*) const; };
extern "C" const FlowerInitNerve dat_003F15D0, dat_003F15D4, dat_003F15D8, dat_003F15DC, dat_003F15E0, dat_003F15E4;

void FireFlower::init(const al::ActorInitInfo& info)
{
    al::initActorWithArchiveName(this, info, sead::SafeString("FireFlower"), 0);
    al::initNerve(this, &dat_003F15D0, 5);
    mAppearParam = new FlowerInit::Param;
    mItemControl = fn_002801B8(this);
    al::initNerveState(this, new FlowerInit::Appear(this), &dat_003F15D4, "\x82\xbb\x82\xcc\x8f\xea\x8f\x6f\x8c\xbb");
    al::initNerveState(this, new FlowerInit::Forward(this, mAppearParam, true, 0), &dat_003F15DC, "\x92\xb5\x82\xcb\x8f\xe3\x82\xb0\x8f\x6f\x8c\xbb(\x91\x4f\x95\xfb)");
    al::initNerveState(this, new FlowerInit::Vertical(this, mAppearParam, true, 0), &dat_003F15D8, "\x92\xb5\x82\xcb\x8f\xe3\x82\xb0\x8f\x6f\x8c\xbb(\x90\x5e\x8f\xe3)");
    al::initNerveState(this, new FlowerInit::Release(this, mAppearParam), &dat_003F15E0, "\x8e\xe6\x82\xe8\x8f\x6f\x82\xb5\x8f\x6f\x8c\xbb");
    al::initNerveState(this, new FlowerInit::Ground(this, mAppearParam), &dat_003F15E4, "\x90\xda\x92\x6e");
    makeActorAppeared();
}
