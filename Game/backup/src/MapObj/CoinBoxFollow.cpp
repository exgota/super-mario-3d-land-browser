#include "MapObj/CoinBox.h"

#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerveFunction.h>
#include <math.h>

// Address-labelled declarations avoid assigning unproved original API names.
// Word predicates retain their full register result; their exact original
// bool/enum spelling is not needed by this caller's zero/nonzero tests.
extern "C" bool fn_00262B84( const al::HitSensor* sensor );
extern "C" void fn_002728C0( al::LiveActor* actor, const char* shadowName );
extern "C" void fn_00272918( al::LiveActor* actor, const char* shadowName );
extern "C" void fn_00262A20( al::LiveActor* actor, const char* shadowName,
                             const sead::Vector3f& scale );
extern "C" int fn_00262AF8( const al::HitSensor* sensor );
extern "C" void fn_0027285C( al::LiveActor* actor );
extern "C" void fn_00272AEC( al::LiveActor* actor );
extern "C" int fn_00262ADC( const al::HitSensor* sensor );
extern "C" void fn_00262A70( al::LiveActor* actor );
extern "C" void fn_00262CC8( al::LiveActor* actor );
extern "C" signed char fn_00262A5C( const al::HitSensor* sensor );
extern "C" void fn_00217E70( al::HitSensor* sensor, bool flagA, bool flagB );
extern "C" int fn_00262A40( const al::HitSensor* sensor );
extern "C" const al::LiveActor* fn_00262A04( const al::HitSensor* sensor );
extern "C" const sead::Matrix34f* fn_002519A8( const al::LiveActor* actor,
                                              const char* jointName );
extern "C" int fn_002CE988( const al::HitSensor* sensor );
extern "C" void fn_00269380( al::LiveActor* actor, const sead::Vector3f& position );

// Shared named-shadow strings, distinct from this function's inline literals.
extern "C" const char dat_003BD4C0[]; // "Body"
extern "C" const char dat_003BD4C8[]; // "Wait"
// Initialized independently by 0x003805B0, not encoded instruction payloads.
extern "C" const sead::Vector3f dat_0042FC88; // (1, 4, 1)
extern "C" const sead::Vector3f dat_0042FC94; // (1.7, 0.15, 1.7)

#ifdef NON_MATCHING
namespace
{
// The retail state copies x/y/z in order, including when source and destination
// are the same vector. This local helper expresses that memberwise assignment
// without changing copy behavior for unrelated vector consumers.
inline void copyTranslation( sead::Vector3f& destination, const sead::Vector3f& source )
{
        destination.x = source.x;
        destination.y = source.y;
        destination.z = source.z;
}
inline float vectorLength( const sead::Vector3f& value )
{
        return sqrtf( value.x * value.x + value.y * value.y + value.z * value.z );
}
} // namespace

extern "C" void fn_0030FABC( CoinBox* actor )
{
        if ( al::isFirstStep( actor ) )
        {
                actor->mUsesMiniWaitAnimation = false;
                if ( fn_00262B84( actor->mAttachedSensor ) )
                        al::startAction( actor, "AttachMini" );
                else
                        al::startAction( actor, "Attach" );
                fn_002728C0( actor, dat_003BD4C0 );
                fn_00272918( actor, dat_003BD4C8 );
                fn_00262A20( actor, "Wait", sead::Vector3f( 1.0f, 1.0f, 1.0f ) );
                actor->mDistanceSinceCoin = 0.0f;
                copyTranslation( actor->mPreviousTranslation, al::getTrans( actor ) );
        }

        if ( al::isActionEnd( actor ) )
        {
                if ( fn_00262B84( actor->mAttachedSensor ) )
                {
                        al::startAction( actor, "WaitMini" );
                        actor->mUsesMiniWaitAnimation = true;
                }
                else
                        al::startAction( actor, "Wait" );
        }
        if ( actor->mUsesMiniWaitAnimation && !fn_00262B84( actor->mAttachedSensor ) )
        {
                al::startAction( actor, "Wait" );
                actor->mUsesMiniWaitAnimation = false;
        }

        if ( fn_00262AF8( actor->mAttachedSensor ) )
                fn_0027285C( actor );
        else
                fn_00272AEC( actor );
        if ( fn_00262ADC( actor->mAttachedSensor ) )
                fn_00262A70( actor );
        else
                fn_00262CC8( actor );

        if ( fn_00262A5C( actor->mAttachedSensor ) )
        {
                fn_00217E70( actor->mAttachedSensor, false, false );
                return;
        }

        if ( fn_00262A40( actor->mAttachedSensor ) )
                fn_00262A20( actor, "Body", dat_0042FC94 );
        else
                fn_00262A20( actor, "Body", dat_0042FC88 );

        const al::LiveActor* playerModel = fn_00262A04( actor->mAttachedSensor );
        al::updatePoseMtx( actor, fn_002519A8( playerModel, "JointRoot" ) );
        const float distance = vectorLength( al::getTrans( actor ) - actor->mPreviousTranslation );
        if ( fn_002CE988( actor->mAttachedSensor ) && distance > 3.0f )
        {
                actor->mDistanceSinceCoin += distance;
                if ( actor->mDistanceSinceCoin >= 500.0f )
                {
                        sead::Vector3f up;
                        al::calcUpDir( &up, actor );
                        if ( fn_00262B84( actor->mAttachedSensor ) )
                                al::startAction( actor, "CoinMini" );
                        else
                                al::startAction( actor, "Coin" );
                        const sead::Vector3f coinPosition = al::getTrans( actor ) + up * 100.0f;
                        fn_00269380( actor, coinPosition );
                        if ( actor->mCoinsRemaining > 0 )
                        {
                                --actor->mCoinsRemaining;
                                if ( actor->mCoinsRemaining <= 0 )
                                {
                                        fn_00217E70( actor->mAttachedSensor, false, false );
                                        return;
                                }
                        }
                        actor->mDistanceSinceCoin = 0.0f;
                }
        }
        copyTranslation( actor->mPreviousTranslation, al::getTrans( actor ) );
}
#endif
