#pragma once

#include <HitSensor/alHitSensor.h>

namespace al
{

class SensorHitGroup
{
private:
        s32         mCapacity;
        s32         mSensorCount;
        HitSensor** mSensors;

public:
        void add( HitSensor* sensor );
        void remove( HitSensor* sensor );

public:
        SensorHitGroup( int capacity, const char* name );
};

} // namespace al
