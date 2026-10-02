#pragma once

#include <MapObj/alMapObjActor.h>

class EnemyStateBlowDown;

class Bubble : public al::MapObjActor
{
private:
        int            _60;
        int            _64;
        void*          _68;
        float          _6C;
        float          _70;
        sead::Vector3f _74;
        sead::Quatf    _80;
        EnemyStateBlowDown* _90;

public:
        virtual void init( const al::ActorInitInfo& info );
        void clearInitialState();
        static bool isBelowLimit( int value );
        virtual void attackSensor( al::HitSensor* me, al::HitSensor* other );
        virtual bool receiveMsg( u32 msg, al::HitSensor* other, al::HitSensor* me );

public:
        Bubble( const sead::SafeString& name );
};
