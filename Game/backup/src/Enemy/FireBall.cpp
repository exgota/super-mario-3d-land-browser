#include "Enemy/FireBall.h"

#include <Collision/alCollisionUtil.h>
#include <LiveActor/alActorActionKeeper.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alHitSensorFunction.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alSensorMsg.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>

namespace NrvFireBall
{

NERVE_DEF( FireBall, Shot )

} // namespace NrvFireBall

FireBall::FireBall( const sead::SafeString& name ) : MapObjActor( name )
{
}

void FireBall::init( const al::ActorInitInfo& info )
{
        al::initActorWithArchiveName( this, info, "FireBall" );
        al::initNerve( this, &NrvFireBall::Shot );
        makeActorDead();
}

void FireBall::attackSensor( al::HitSensor* me, al::HitSensor* other )
{ // :skull:?
        if ( ( !al::isSensorPlayer( other ) || !al::sendMsgEnemyAttack( other, me ) ) &&
                ( ( ( ( !al::isSensorEnemy( other ) || !al::isGreaterEqualStep( (IUseNerve*)this, 10 ) ) &&
                            ( !al::isSensorMapObj( other ) ) ) ||
                        !al::sendMsg50( other, me ) ) ) )
                return;

        kill();
}

bool FireBall::receiveMsg( u32 msg, al::HitSensor* other, al::HitSensor* me )
{
        if ( al::isMsg9( msg ) || al::isMsgPlayerStatueTouch( msg ) || al::isMsgKickStoneAttack( msg ) ||
                al::isMsgPlayerInvincibleAttack( msg ) )
        {
                kill();
                if ( al::isMsgPlayerInvincibleAttack( msg ) )
                        return false;
                else
                        return true;
        }
        return false;
}

extern "C" const char dat_003C0824[8];
extern "C" bool fn_0027063c( al::LiveActor* actor, const char* actionName );

inline void FireBall::exeShot()
{
        if ( al::isFirstStep( this ) )
                fn_0027063c( this, dat_003C0824 );
        if ( al::isCollided( this ) )
        {
                al::startHitReactionBreak( this );
                kill();
                return;
        }
        if ( al::isGreaterStep( this, 90 ) )
        {
                al::startHitReactionDeath( this );
                kill();
        }
}
