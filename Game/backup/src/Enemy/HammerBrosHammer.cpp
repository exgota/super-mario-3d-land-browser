#include <Enemy/HammerBrosHammer.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <prim/seadSafeString.h>
extern "C" float* fn_0027B0DC(al::LiveActor*,const char*);
extern "C" int* fn_0026C9A8(al::LiveActor*,const char*);
namespace HammerProjectile {
struct Parameters {
    float *airResistance,*gravity,*mediumGravity,*nearGravity,*nearSpeed;
    float *maximumHeight,*mediumMaximumHeight,*nearDistance,*mediumDistance;
    int* initialSensorDisabledFrames;
    Parameters(al::LiveActor* owner) {
        airResistance=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x8b\xf3\x8b\x43\x92\xef\x8d\x52\x5d");
        gravity=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x8f\x64\x97\xcd\x5d");
        mediumGravity=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x8f\x64\x97\xcd\x28\x92\x86\x8b\x97\x97\xa3\x29\x5d");
        nearGravity=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x8f\x64\x97\xcd\x28\x8b\xdf\x8b\x97\x97\xa3\x29\x5d");
        nearSpeed=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x91\xac\x93\x78\x28\x8b\xdf\x8b\x97\x97\xa3\x29\x5d");
        maximumHeight=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x8d\xc5\x91\xe5\x8d\x82\x82\xb3\x5d");
        mediumMaximumHeight=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x8d\xc5\x91\xe5\x8d\x82\x82\xb3\x28\x92\x86\x8b\x97\x97\xa3\x29\x5d");
        nearDistance=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x8b\xdf\x8b\x97\x97\xa3\x5d");
        mediumDistance=fn_0027B0DC(owner,"\x83\x6e\x83\x93\x83\x7d\x81\x5b\x5b\x92\x86\x8b\x97\x97\xa3\x5d");
        initialSensorDisabledFrames=fn_0026C9A8(owner,"\x83\x5a\x83\x93\x83\x54\x81\x5b\x96\xb3\x8c\xf8\x8e\x9e\x8a\xd4\x5b\x88\xda\x93\xae\x8a\x4a\x8e\x6e\x8e\x9e\x5d");
    }
};
static_assert(sizeof(Parameters)==0x28,"Observed parameter allocation");
class InitialNerve : public al::Nerve { public: virtual void execute(al::NerveKeeper*) const; };
}
extern "C" const HammerProjectile::InitialNerve dat_003F2FEC;
void HammerBrosHammer::init(const al::ActorInitInfo& info)
{
    al::initActorWithArchiveName(this,info,sead::SafeString("HammerBrosHammer"),0);
    mHammerParameters=new HammerProjectile::Parameters(mOwnerParameters);
    al::initNerve(this,&dat_003F2FEC,0);
    makeActorDead();
}
