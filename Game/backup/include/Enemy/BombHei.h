#pragma once

#include <MapObj/alMapObjActor.h>

class BombHei : public al::MapObjActor
{
private:
        u8    _60[ 0x10 ];
        int   _70;
        int   _74;
        int   _78;
        float _7C;
        float _80;
        bool  _84;
        float _88;

public:
        virtual void control();
        virtual void init( const al::ActorInitInfo& info );
        virtual void makeActorAppeared();
        virtual void attackSensor( al::HitSensor* me, al::HitSensor* other );
        virtual bool receiveMsg( u32 msg, al::HitSensor* other, al::HitSensor* me );

public:
        BombHei( const sead::SafeString& name );
};

static_assert_( sizeof( BombHei ) == 0x8C );
