#include <Execute/alExecuteTableHolder.h>
#include <HitSensor/alSensorHitGroup.h>
#include <LiveActor/alHitSensorDirector.h>

#ifdef NON_MATCHING
// Retail helpers whose semantic names have not yet been established.
extern "C" void fn_0024977C( al::HitSensor* sensor, al::HitSensor* other );
extern "C" al::HitSensor* fn_002497A4( al::SensorHitGroup* group, int index );
extern "C" void fn_002497B0( al::HitSensorDirector* director,
                            al::SensorHitGroup* first, al::SensorHitGroup* second );
extern "C" void fn_002498A4( al::SensorHitGroup* group );
#endif

namespace al
{

#ifdef NON_MATCHING
namespace
{

inline void tryAddHitSensorPair( HitSensor* first, HitSensor* second )
{
        LiveActor* secondHost = second->getHost();
        if ( first->getHost() == secondHost )
                return;

        const sead::Vector3f& firstPos = first->getPos();
        const sead::Vector3f& secondPos = second->getPos();
        sead::Vector3f difference( firstPos.x - secondPos.x,
                                   firstPos.y - secondPos.y,
                                   firstPos.z - secondPos.z );
        float radius = first->getRadius() + second->getRadius();
        if ( radius * radius <= difference.x * difference.x
                              + difference.y * difference.y
                              + difference.z * difference.z )
                return;

        if ( second->getType() != SensorType_Eye )
                fn_0024977C( first, second );
        if ( first->getType() != SensorType_Eye )
                fn_0024977C( second, first );
}

} // namespace
#endif

HitSensorDirector::HitSensorDirector()
    : mPlayerHitGroup( nullptr ), mRideHitGroup( nullptr ), mEyeHitGroup( nullptr ),
      mSimpleHitGroup( nullptr ), mMapObjHitGroup( nullptr ), mCharacterHitGroup( nullptr )
{
        mPlayerHitGroup    = new SensorHitGroup( 16, "Player" );
        mRideHitGroup      = new SensorHitGroup( 128, "Ride" );
        mEyeHitGroup       = new SensorHitGroup( 512, "Eye" );
        mSimpleHitGroup    = new SensorHitGroup( 2048, "Simple" );
        mMapObjHitGroup    = new SensorHitGroup( 1024, "MapObj" );
        mCharacterHitGroup = new SensorHitGroup( 1024, "Character" );

        registerExecutorUser( this, "ƒZƒ“ƒT[" );
}

#ifdef NON_MATCHING
void HitSensorDirector::execute()
{
        fn_002498A4( mPlayerHitGroup );
        fn_002498A4( mRideHitGroup );
        fn_002498A4( mEyeHitGroup );
        fn_002498A4( mSimpleHitGroup );
        fn_002498A4( mMapObjHitGroup );
        fn_002498A4( mCharacterHitGroup );

        fn_002497B0( this, mPlayerHitGroup, mCharacterHitGroup );
        fn_002497B0( this, mPlayerHitGroup, mMapObjHitGroup );
        fn_002497B0( this, mPlayerHitGroup, mRideHitGroup );
        fn_002497B0( this, mPlayerHitGroup, mSimpleHitGroup );
        fn_002497B0( this, mPlayerHitGroup, mEyeHitGroup );
        fn_002497B0( this, mRideHitGroup, mCharacterHitGroup );
        fn_002497B0( this, mRideHitGroup, mMapObjHitGroup );
        fn_002497B0( this, mRideHitGroup, mSimpleHitGroup );
        fn_002497B0( this, mRideHitGroup, mEyeHitGroup );
        fn_002497B0( this, mEyeHitGroup, mCharacterHitGroup );
        fn_002497B0( this, mEyeHitGroup, mMapObjHitGroup );
        fn_002497B0( this, mEyeHitGroup, mSimpleHitGroup );
        fn_002497B0( this, mCharacterHitGroup, mMapObjHitGroup );

        SensorHitGroup* group = mCharacterHitGroup;
        int count = group->getSensorCount();
        for ( int firstIndex = 0; firstIndex < count; ++firstIndex )
        {
                HitSensor* first = fn_002497A4( group, firstIndex );
                for ( int secondIndex = firstIndex; secondIndex < count; ++secondIndex )
                {
                        HitSensor* second = fn_002497A4( group, secondIndex );
                        tryAddHitSensorPair( first, second );
                }
        }
}

#endif

} // namespace al
