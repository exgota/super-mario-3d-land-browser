#include <LiveActor/alLiveActor.h>
#include <LiveActor/alSensorMsg.h>
#include <Nerve/alNerveFunction.h>

#include "Enemy/EnemyStateBlowDown.h"

extern "C" const al::Nerve dat_003F23BC;
extern "C" const al::Nerve dat_003F23C0;
extern "C" bool fn_00278B8C( u32 msg );
extern "C" bool fn_0027B180( u32 msg );
extern "C" bool fn_00278AF0( u32 msg );
extern "C" bool fn_0027A700( u32 msg );
extern "C" bool fn_0027DB4C( u32 msg, al::HitSensor* other, al::HitSensor* me );
extern "C" void fn_0027D760( u32 msg, const al::HitSensor* me, const al::HitSensor* other );
extern "C" bool fn_0027b704( u32 msg, al::HitSensor* other, al::HitSensor* me,
                            al::NerveStateBase* state );

// The retail vtable at 0x003D3AC8 identifies this as a LiveActor receiveMsg
// override. The actor's name and the untouched fields remain unidentified.
struct Actor3135E0 : al::LiveActor
{
        void* _60;
        EnemyStateBlowDown* mStateBlowDown; // 0x64; initialized by 0x00313AF8
        unsigned char _68[0x64];
        bool _CC;

        bool receiveTrample( u32 msg, al::HitSensor* other, al::HitSensor* me )
        {
                if ( al::isNerve( this, &dat_003F23C0 ) ||
                     al::isNerve( this, &dat_003F23BC ) || _CC )
                        return false;
                if ( fn_00278B8C( msg ) && !fn_0027DB4C( msg, other, me ) )
                        return false;
                fn_0027D760( msg, me, other );
                al::setNerve( this, &dat_003F23BC );
                return true;
        }

        bool receiveBlowDown( u32 msg, al::HitSensor* other, al::HitSensor* me )
        {
                if ( al::isNerve( this, &dat_003F23C0 ) || al::isNerve( this, &dat_003F23BC ) )
                        return false;
                fn_0027b704( msg, other, me, mStateBlowDown );
                al::setNerve( this, &dat_003F23C0 );
                return true;
        }
};

extern "C" bool fn_003135E0( Actor3135E0* actor, u32 msg, al::HitSensor* other, al::HitSensor* me )
{
        if ( al::isMsg9( msg ) )
        {
                if ( actor->_CC )
                        return actor->receiveBlowDown( msg, other, me );
                return actor->receiveTrample( msg, other, me );
        }
        if ( fn_00278B8C( msg ) || fn_0027B180( msg ) )
                return actor->receiveTrample( msg, other, me );
        if ( fn_00278AF0( msg ) || fn_0027A700( msg ) )
                return actor->receiveBlowDown( msg, other, me );
        return false;
}
