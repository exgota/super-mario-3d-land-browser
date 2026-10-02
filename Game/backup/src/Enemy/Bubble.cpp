#include "Enemy/Bubble.h"

#include <LiveActor/alHitSensorFunction.h>
#include <LiveActor/alSensorMsg.h>
#include <Nerve/alNerveFunction.h>
#include <Nerve/alNerve.h>

extern "C" void __rt_memclr( void*, unsigned int );

struct BubbleBlowDownNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};

extern "C" const BubbleBlowDownNerve dat_003F1F00;
extern "C" bool fn_00218B90( al::HitSensor* other, al::HitSensor* me );

bool Bubble::isWithinValueLimit( int value )
{
        if ( value > 9998 )
                return false;
        return true;
}

Bubble::Bubble( const sead::SafeString& name )
    : MapObjActor( name ), _60( 0 ), _64( 30 ), _68( nullptr ), _6C( 500.0f ), _70( 2.4f ),
      _74( sead::Vector3f::zero ), _80( sead::Quatf::unit ), _90( nullptr )
{
}

void Bubble::clearInitialState()
{
        __rt_memclr( this, 0x12 );
}

bool Bubble::isBelowLimit( int value )
{
        if ( value >= 900 )
                return false;
        return true;
}

void Bubble::attackSensor( al::HitSensor* me, al::HitSensor* other )
{
        if ( al::isNerve( this, &dat_003F1F00 ) )
                return;
        if ( !al::isSensorName( me, "Attack" ) )
                return;
        if ( al::isSensorPlayer( other ) )
        {
                if ( !al::sendMsg50( other, me ) )
                        fn_00218B90( other, me );
        }
        else
                al::sendMsg50( other, me );
}
