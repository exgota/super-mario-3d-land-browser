#pragma once

#include <HitSensor/alSensorType.h>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

namespace al
{

class LiveActor;
class SensorHitGroup;

class HitSensor
{
        friend class HitSensorKeeper;

private:
        const char*      mName;
        SensorType       mSensorType;
        sead::Vector3f   mPosition;
        float            mSensorRadius;
        u16              mMaxSensorCount;
        u16              mSensorCount;
        HitSensor**      mSensors;
        SensorHitGroup*  mSensorHitGroup;
        bool             mIsValidBySystem;
        bool             mIsValid;
        LiveActor*       mHostActor;
        sead::Vector3f*  mFollowPos;
        sead::Matrix34f* mFollowMtx;
        sead::Vector3f   _34;

public:
        const char* getName()
        {
                return mName;
        }

        LiveActor* getHost()
        {
                return mHostActor;
        }

        u32 getType() const
        {
                return mSensorType;
        }

        const sead::Vector3f& getPos() const
        {
                return mPosition;
        }

        float getRadius() const
        {
                return mSensorRadius;
        }

        void validate();
        void invalidate();
        void validateBySystem();
        void invalidateBySystem();
        void update();
};

static_assert( sizeof( HitSensor ) == 0x40, "" );

bool isHitCylinderSensor( HitSensor*, HitSensor*, const sead::Vector3f&, float );

} // namespace al
