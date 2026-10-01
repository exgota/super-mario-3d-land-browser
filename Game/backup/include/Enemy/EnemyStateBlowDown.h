#pragma once

#include <Nerve/alActorStateBase.h>
#include <math/seadVector.h>

class EnemyStateBlowDownParam;

class EnemyStateBlowDown : public al::ActorStateBase
{
private:
        EnemyStateBlowDownParam* mParam;            // 0x10
        sead::Vector3f           mBlowDownDirection; // 0x14
        const char*             mAnimName;         // 0x20
        u32                     mBlowDownMessage;  // 0x24

public:
        virtual void appear();

public:
        EnemyStateBlowDown( al::LiveActor* host, EnemyStateBlowDownParam* blowDownParam, const char*, int );
};

static_assert( sizeof( EnemyStateBlowDown ) == 0x28, "" );
