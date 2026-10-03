#include <Enemy/Tenten.h>
#include <Enemy/EnemyStateBlowDown.h>
#include <Enemy/EnemyStateHipDropDown.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Placement/alPlacementFunction.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>
#include <Util/alStringUtil.h>
namespace al {
bool tryGetArg5(bool*,const ActorInitInfo&);
bool tryGetArg7(bool*,const ActorInitInfo&);
}
struct TentenNerve : al::Nerve { virtual void execute(al::NerveKeeper*) const; };
extern "C" const TentenNerve dat_003F3090,dat_003F30A4,dat_003F30A0;
extern "C" void fn_00273F7C(al::LiveActor*);
extern "C" void fn_0027CF20(al::LiveActor*,const al::ActorInitInfo&,int);
void Tenten::init(const al::ActorInitInfo& info)
{
    const char* objectName;
    al::tryGetObjectName(&objectName,info);
    if (al::isEqualString(objectName,"TentenGenerator")) {
        al::initActorWithArchiveName(this,info,"Tenten");
        al::startAction(this,"Walk");
    } else if (al::isEqualString(objectName,"TentenWingGenerator")) {
        al::initActorWithArchiveName(this,info,"TentenWing");
        al::startAction(this,"FlyWait");
    }
    if (al::isExistRail(info)) { initRailKeeper(info); fn_00273F7C(this); }
    al::initNerve(this,&dat_003F3090,2);
    al::invalidateClipping(this);
    al::tryGetArg5(&option5,info);
    blowDown=new EnemyStateBlowDown(this,0,0,0);
    al::initNerveState(this,blowDown,&dat_003F30A4,"state:BlowDown");
    hipDropDown=new EnemyStateHipDropDown(this,0,0);
    al::initNerveState(this,hipDropDown,&dat_003F30A0,"state:HipDropDown");
    fn_0027CF20(this,info,1);
    al::offCollide(this);
    al::tryGetArg7(&option7,info);
    makeActorAppeared();
}
