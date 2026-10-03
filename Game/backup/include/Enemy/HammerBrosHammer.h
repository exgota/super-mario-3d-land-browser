#pragma once
#include <LiveActor/alLiveActor.h>
namespace HammerProjectile { struct Parameters; }
struct HammerSpawnCounter;
class HammerBrosHammer : public al::LiveActor {
public:
    void* mSecondaryInterface;                  // 0x60
    al::LiveActor* mOwnerParameters;             // 0x64
    HammerProjectile::Parameters* mHammerParameters; // 0x68
    int mItemId;                                // 0x6C
    HammerSpawnCounter* mOwner;                 // 0x70
    float mUnresolvedScalar;                    // 0x74
    unsigned int mUnresolvedStorage[6];          // 0x78, two observed word triples

    virtual void init(const al::ActorInitInfo& info);
    void breakHammer(const char* reaction);
};
// Allocation extent is observed. This initializer view does not reconstruct the
// complete actor virtual interface or the secondary polymorphic subobject at 0x60.
// Remaining field meanings are unresolved. No actor constructor is supplied;
// compiler-generated actor tables from this partial view are not authoritative.
static_assert(sizeof(HammerBrosHammer)==0x90,"Observed actor allocation");
