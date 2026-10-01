#include <HitSensor/alSensorHitGroup.h>

namespace al
{

SensorHitGroup::SensorHitGroup( int capacity, const char* )
    : mCapacity( capacity ), mSensorCount( 0 ), mSensors( nullptr )
{
        mSensors = new HitSensor*[ mCapacity ];
        for ( int index = 0; index < mCapacity; ++index )
                mSensors[ index ] = nullptr;
}

void SensorHitGroup::add( HitSensor* sensor )
{
        mSensors[ mSensorCount ] = sensor;
        ++mSensorCount;
}

void SensorHitGroup::remove( HitSensor* sensor )
{
        int count = mSensorCount;
        for ( int index = 0; index < count; ++index )
        {
                if ( mSensors[ index ] == sensor )
                {
                        mSensors[ index ] = mSensors[ count - 1 ];
                        --mSensorCount;
                        return;
                }
        }
}

} // namespace al
