#pragma once
#include <LiveActor/alLiveActor.h>
namespace HammerProjectile { struct Parameters; }
struct HammerSpawnCounter;
class HammerBrosHammer : public al::LiveActor {
public:
    virtual void v24();
    virtual void startMoving();
    virtual bool canReceiveAttack() const;

    void* mSecondaryInterface;                  // 0x60
    al::LiveActor* mOwnerParameters;             // 0x64
    HammerProjectile::Parameters* mHammerParameters; // 0x68
    int mItemId;                                // 0x6C
    HammerSpawnCounter* mOwner;                 // 0x70

    virtual void init(const al::ActorInitInfo& info);
    void breakHammer(const char* reaction);
};
static_assert(sizeof(HammerBrosHammer)==0x74,"Observed field prefix; allocation is 0x90");
