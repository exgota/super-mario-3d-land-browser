#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alLiveActor.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alLiveActorGroup.h>
#include <LiveActor/alLiveActorKit.h>
#include <Rail/alRailKeeper.h>
#include <Model/alModelKeeper.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alHitSensorKeeper.h>
#include <LiveActor/alSubActorFunction.h>

extern "C" void fn_00268E80( al::AudioKeeper* keeper );
extern "C" const sead::Matrix34f* fn_0025C754( const alModelCtr* model );

extern "C" void fn_001DC038( al::LiveActor* actor );
extern "C" void fn_0024C9EC( al::Collider* collider );
extern "C" void fn_001C96B8( al::LiveActor* actor );

extern "C" void fn_00129360( al::ModelKeeper* keeper );
extern "C" void fn_0024ADCC( al::EffectKeeper* keeper );
extern "C" void fn_0024AD94( al::AudioKeeper* keeper );
extern "C" void fn_001C1F50( al::LiveActor* actor );
extern "C" void fn_00252B04( al::LiveActor* actor );
extern "C" void fn_00250F94( al::LiveActor* actor );

namespace al
{

class ActorExecuteInfo
{
public:
        unsigned char _0[ 0x18 ];
        void* _18;
};

LiveActor::LiveActor( const char* name )
    : mActorName( name ), mActorPoseKeeper( nullptr ), mActorExecuteInfo( nullptr ),
      mActorActionKeeper( nullptr ), mCollider( nullptr ), mCollisionParts( nullptr ),
      mModelKeeper( nullptr ), mNerveKeeper( nullptr ), mHitSensorKeeper( nullptr ),
      mEffectKeeper( nullptr ), mAudioKeeper( nullptr ), mStageSwitchKeeper( nullptr ),
      mRailKeeper( nullptr ), mShadowKeeper( nullptr ), mActorLightCtrl( nullptr ), _4C( nullptr ),
      mSubActorKeeper( nullptr )
{
        getLiveActorKit()->getAllActors()->registerActor( this );
}

NerveKeeper* LiveActor::getNerveKeeper() const
{
        return mNerveKeeper;
}

void LiveActor::init( const ActorInitInfo& info )
{
}

void LiveActor::initAfterPlacement()
{
}

void LiveActor::appear()
{
        makeActorAppeared();
}

void LiveActor::kill()
{
        makeActorDead();
}

void LiveActor::calcAnim()
{
        if ( !mLiveActorFlag.isDead && ( !mLiveActorFlag.isClipped || mLiveActorFlag.isDrawClipping ) )
        {
                if ( mActorPoseKeeper )
                        alLiveActorFunction::calcAnimDirect( this );
                if ( getAudioKeeper() )
                        fn_00268E80( getAudioKeeper() );
        }
}

void LiveActor::attackSensor( HitSensor* me, HitSensor* other )
{
}

bool LiveActor::receiveMsg( u32 msg, HitSensor* other, HitSensor* me )
{
        return false;
}

void LiveActor::draw()
{
}

const sead::Matrix34f* LiveActor::getBaseMtx() const
{
        if ( mModelKeeper )
                return fn_0025C754( mModelKeeper->getModel() );
        return nullptr;
}

EffectKeeper* LiveActor::getEffectKeeper() const
{
        return mEffectKeeper;
}

AudioKeeper* LiveActor::getAudioKeeper() const
{
        return mAudioKeeper;
}

StageSwitchKeeper* LiveActor::getStageSwitchKeeper() const
{
        return mStageSwitchKeeper;
}

void LiveActor::initStageSwitchKeeper()
{
        mStageSwitchKeeper = new StageSwitchKeeper();
}

void LiveActor::control()
{
}

void LiveActor::startClipped()
{
        mLiveActorFlag.isClipped = true;
        if ( mModelKeeper )
                fn_00129360( mModelKeeper );
        if ( !mLiveActorFlag.isDrawClipping )
        {
                if ( mHitSensorKeeper )
                        mHitSensorKeeper->invalidateBySystem();
                if ( getEffectKeeper() )
                        fn_0024ADCC( getEffectKeeper() );
                if ( getAudioKeeper() )
                        fn_0024AD94( getAudioKeeper() );
        }
        if ( mShadowKeeper )
                fn_001C1F50( this );
        if ( mActorExecuteInfo )
        {
                if ( !mLiveActorFlag.isDrawClipping )
                        fn_00252B04( this );
                if ( mActorExecuteInfo->_18 )
                        fn_00250F94( this );
        }
        if ( mSubActorKeeper )
                alSubActorFunction::trySyncClippingStart( mSubActorKeeper );
}

void LiveActor::makeActorDead()
{
        if ( mActorPoseKeeper )
                al::setVelocityZero( this );
        mLiveActorFlag.isDead = true;
        if ( mHitSensorKeeper )
                mHitSensorKeeper->invalidateBySystem();
        fn_001DC038( this );
        if ( mCollider )
                fn_0024C9EC( mCollider );
        if ( mCollisionParts )
                fn_001C96B8( this );
        if ( mModelKeeper )
                fn_00129360( mModelKeeper );
        if ( getEffectKeeper() )
                getEffectKeeper()->deleteAndClearEffectAll();
        if ( getAudioKeeper() )
                fn_0024AD94( getAudioKeeper() );
        if ( mShadowKeeper )
                fn_001C1F50( this );
        if ( mActorExecuteInfo )
        {
                fn_00252B04( this );
                if ( mActorExecuteInfo->_18 )
                        fn_00250F94( this );
        }
        if ( mSubActorKeeper )
                alSubActorFunction::trySyncDead( mSubActorKeeper );
}

void LiveActor::initNerveKeeper( NerveKeeper* nk )
{
        mNerveKeeper = nk;
}

void LiveActor::initPoseKeeper( ActorPoseKeeperBase* pPoseKeeper )
{
        mActorPoseKeeper = pPoseKeeper;
}

void LiveActor::initRailKeeper( const ActorInitInfo& info )
{
        mRailKeeper = al::tryCreateRailKeeper( al::getPlacementInfo( info ) );
}

} // namespace al
