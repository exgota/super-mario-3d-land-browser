#pragma once

#include <Nerve/alActorStateBase.h>
#include <math/seadMatrix.h>

namespace al
{
class LiveActor;
class LiveActorGroup;
}

struct BunbunSpinAttackParameters;

// Descriptive reconstruction name for the ordinary spin-attack state owner.
class BunbunStateSpinAttack : public al::ActorStateBase
{
        al::LiveActor* mArm;
        al::LiveActorGroup* mTrailActors;
        BunbunSpinAttackParameters* mParameters;
        sead::Matrix34f* mFirstMatrix;
        sead::Matrix34f* mSecondMatrix;

public:
        BunbunStateSpinAttack( al::LiveActor* host, al::LiveActorGroup* trailActors,
                               sead::Matrix34f* firstMatrix, sead::Matrix34f* secondMatrix,
                               const char* variant );
        virtual void appear();
        virtual void kill();
};

static_assert( sizeof( BunbunStateSpinAttack ) == 0x24, "Observed spin-state allocation" );
