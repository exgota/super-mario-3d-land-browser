#pragma once

#include <MapObj/alMapObjActor.h>
#include <math/seadQuat.h>

// Constructor 0x00319ADC and its factory allocate the complete 0xB0 object.
// Init constructs linked doors of this same type.
class WarpDoor : public al::MapObjActor
{
        al::LiveActor* mPlayer;
        WarpDoor* mNextDoor;
        void* mExitCamera;
        al::HitSensor* mPlayerSensor;
        sead::Quatf mPlayerQuat;
        sead::Vector3f mPlayerTranslation;
        sead::Vector3f mInitialTranslation;
        sead::Vector3f mExitTranslation;
        int mReactionTimer;
        int mDoorType;
        bool mInitiallyHidden;

public:
        WarpDoor( const sead::SafeString& name );
        virtual void init( const al::ActorInitInfo& info );
        virtual bool receiveMsg( u32 message, al::HitSensor* other, al::HitSensor* me );
        virtual void control();
};

static_assert( sizeof( WarpDoor ) == 0xB0, "WarpDoor retail allocation" );
