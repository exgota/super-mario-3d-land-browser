#include <MapObj/BoomerangFlower.h>
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
extern "C" const FlowerInitNerve dat_003F15A8, dat_003F15AC, dat_003F15B0, dat_003F15B4, dat_003F15B8, dat_003F15BC;

void BoomerangFlower::init(const al::ActorInitInfo& info)
{
    al::initActorWithArchiveName(this, info, sead::SafeString("BoomerangFlower"), 0);
    al::initNerve(this, &dat_003F15A8, 5);
    mItemControl = fn_002801B8(this);
    mAppearParam = new FlowerInit::Param;
    al::initNerveState(this, new FlowerInit::Appear(this), &dat_003F15AC, "\x82\xbb\x82\xcc\x8f\xea\x8f\x6f\x8c\xbb");
    al::initNerveState(this, new FlowerInit::Forward(this, mAppearParam, true, 0), &dat_003F15B4, "\x92\xb5\x82\xcb\x8f\xe3\x82\xb0\x8f\x6f\x8c\xbb(\x91\x4f\x95\xfb)");
    al::initNerveState(this, new FlowerInit::Vertical(this, mAppearParam, true, 0), &dat_003F15B0, "\x92\xb5\x82\xcb\x8f\xe3\x82\xb0\x8f\x6f\x8c\xbb(\x90\x5e\x8f\xe3)");
    al::initNerveState(this, new FlowerInit::Release(this, mAppearParam), &dat_003F15B8, "\x8e\xe6\x82\xe8\x8f\x6f\x82\xb5\x8f\x6f\x8c\xbb");
    al::initNerveState(this, new FlowerInit::Ground(this, mAppearParam), &dat_003F15BC, "\x90\xda\x92\x6e");
    makeActorAppeared();
}
