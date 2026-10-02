#include <Collision/alCollider.h>
#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alActorExecuteInfo.h>
#include <LiveActor/alLiveActor.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alLiveActorGroup.h>
#include <LiveActor/alLiveActorKit.h>
#include <Rail/alRailKeeper.h>
#include <Model/alModelKeeper.h>
#include <Collision/alCollisionUtil.h>
#include <Execute/alExecuteTableHolder.h>
#include <Functor/alFunctorV0M.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alHitSensorKeeper.h>
#include <LiveActor/alSubActorFunction.h>

extern "C" void fn_00268E80( al::AudioKeeper* keeper );
extern "C" const sead::Matrix34f* fn_0025C754( const alModelCtr* model );

extern "C" void executeLiveActorActionKeeper( al::ActorActionKeeper* keeper );
extern "C" void executeLiveActorShadowKeeper( al::LiveActor* actor );
extern "C" void fn_00129414( al::ModelKeeper* keeper );
extern "C" const void* fn_001E78B4( const al::LiveActor* actor );
extern "C" void fn_001BFB98( al::IUseEffectKeeper* actor, const void* material );
extern "C" const void* fn_0018B9B8( const al::LiveActor* actor );
extern "C" void fn_001BF2E4( al::AudioKeeper* keeper, const void* material );
extern "C" void fn_002DDC68( sead::Matrix34f* matrix, const sead::Vector3f* scale );
extern "C" void fn_001D2D44( al::LiveActor* actor, const sead::Matrix34f* matrix );
extern "C" void fn_001D2F90( al::ActorActionKeeper* keeper );
extern "C" void fn_00128DE8( alModelCtr* model );
extern "C" void fn_001CA998( al::ActorLightCtrl* controller, void* executionField );
extern "C" void fn_0018BAE8( al::SubActorKeeper* keeper );

extern "C" signed char readLiveActorClippingFlag( const al::LiveActor* actor );
extern "C" void fn_0027C05C( al::LiveActor* actor );
extern "C" void fn_0026FB1C( al::LiveActor* actor );
extern "C" void fn_001DBEFC( al::LiveActor* actor );
extern "C" void fn_001CA88C( al::ActorLightCtrl* controller, void* executionField );

extern "C" signed char readLiveActorModelHiddenFlag( const al::LiveActor* actor );
extern "C" signed char readLiveActorAuxiliaryFlag( const al::LiveActor* actor );

extern "C" void fn_0026E13C( al::LiveActor* actor );
extern "C" void fn_001BFAA4( al::EffectKeeper* keeper );
extern "C" void fn_001BF134( al::AudioKeeper* keeper );
extern "C" void fn_0012939C( al::ModelKeeper* keeper );
extern "C" void fn_00250F7C( al::LiveActor* actor );
extern "C" void fn_0025F890( al::LiveActor* actor );

extern "C" void fn_001DC038( al::LiveActor* actor );
extern "C" void fn_001C96B8( al::LiveActor* actor );

extern "C" void fn_00129360( al::ModelKeeper* keeper );
extern "C" void fn_0024ADCC( al::EffectKeeper* keeper );
extern "C" void fn_0024AD94( al::AudioKeeper* keeper );
extern "C" void fn_001C1F50( al::LiveActor* actor );
extern "C" void fn_00252B04( al::LiveActor* actor );
extern "C" void fn_00250F94( al::LiveActor* actor );

namespace al
{

template <>
void FunctorV0M<LiveActor*, void ( LiveActor::* )()>::operator()() const
{
        ( mParent->*mFuncPtr )();
}

template <>
FunctorV0M<LiveActor*, void ( LiveActor::* )()>*
FunctorV0M<LiveActor*, void ( LiveActor::* )()>::clone() const
{
        return new FunctorV0M<LiveActor*, void ( LiveActor::* )()>( *this );
}

class ActorLightCtrl
{
public:
        unsigned char _0[ 8 ];
        int _8;
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
                if ( mActorExecuteInfo->getPointerAtOffset18() )
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
                mCollider->onInvalidate();
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
                if ( mActorExecuteInfo->getPointerAtOffset18() )
                        fn_00250F94( this );
        }
        if ( mSubActorKeeper )
                alSubActorFunction::trySyncDead( mSubActorKeeper );
}

void LiveActor::endClipped()
{
        mLiveActorFlag.isClipped = false;
        if ( !mLiveActorFlag.isDrawClipping )
        {
                if ( mHitSensorKeeper )
                {
                        mHitSensorKeeper->validateBySystem();
                        fn_0026E13C( this );
                }
                if ( getEffectKeeper() )
                        fn_001BFAA4( getEffectKeeper() );
                if ( getAudioKeeper() )
                        fn_001BF134( getAudioKeeper() );
        }
        if ( mActorExecuteInfo )
        {
                if ( !mLiveActorFlag.isDrawClipping )
                        alActorSystemFunction::addToExecutorMovement( this );
                if ( mActorExecuteInfo->getPointerAtOffset18() && !readLiveActorModelHiddenFlag( this ) )
                        fn_00250F7C( this );
        }
        if ( !readLiveActorModelHiddenFlag( this ) && mModelKeeper )
                fn_0012939C( mModelKeeper );
        if ( mShadowKeeper && !readLiveActorModelHiddenFlag( this ) && !readLiveActorAuxiliaryFlag( this ) )
                fn_0025F890( this );
        if ( mSubActorKeeper )
                alSubActorFunction::trySyncClippingEnd( mSubActorKeeper );
}

void LiveActor::makeActorAppeared()
{
        if ( mHitSensorKeeper )
                mHitSensorKeeper->validateBySystem();
        mLiveActorFlag.isDead = false;
        if ( readLiveActorClippingFlag( this ) )
                endClipped();
        if ( !readLiveActorModelHiddenFlag( this ) && mModelKeeper )
                fn_0012939C( mModelKeeper );
        fn_0027C05C( this );
        if ( mCollisionParts )
                fn_0026FB1C( this );
        if ( mActorLightCtrl && ( mActorLightCtrl->_8 == 1 || mActorLightCtrl->_8 == 0 ) )
                fn_001CA88C( mActorLightCtrl, mActorExecuteInfo->getPointerAtOffset18() );
        if ( mHitSensorKeeper )
                mHitSensorKeeper->update();
        fn_001DBEFC( this );
        if ( mActorExecuteInfo )
        {
                alActorSystemFunction::addToExecutorMovement( this );
                if ( !readLiveActorModelHiddenFlag( this ) && mActorExecuteInfo->getPointerAtOffset18() )
                        fn_00250F7C( this );
        }
        if ( getAudioKeeper() )
                fn_001BF134( getAudioKeeper() );
        if ( mShadowKeeper && !readLiveActorModelHiddenFlag( this ) && !readLiveActorAuxiliaryFlag( this ) )
                fn_0025F890( this );
        if ( mSubActorKeeper )
                alSubActorFunction::trySyncAlive( mSubActorKeeper );
}

void LiveActor::movement()
{
        if ( mLiveActorFlag.isDead || ( mLiveActorFlag.isClipped && !mLiveActorFlag.isDrawClipping ) )
                return;
        if ( mActorActionKeeper )
                executeLiveActorActionKeeper( mActorActionKeeper );
        if ( mModelKeeper )
                fn_00129414( mModelKeeper );
        if ( mLiveActorFlag.isValidMaterialCode && al::isCollidedGround( this ) )
        {
                if ( mEffectKeeper )
                        fn_001BFB98( this, fn_001E78B4( this ) );
                if ( mAudioKeeper )
                        fn_001BF2E4( mAudioKeeper, fn_0018B9B8( this ) );
        }
        if ( mHitSensorKeeper )
        {
                mHitSensorKeeper->attackSensor();
                if ( mLiveActorFlag.isDead )
                        return;
        }
        if ( mNerveKeeper )
        {
                mNerveKeeper->update();
                if ( mLiveActorFlag.isDead )
                        return;
        }
        control();
        if ( mLiveActorFlag.isDead )
                return;
        updateCollider();
        if ( !mModelKeeper )
        {
                if ( mEffectKeeper )
                        mEffectKeeper->update();
                if ( mAudioKeeper )
                        fn_00268E80( mAudioKeeper );
                if ( mCollisionParts )
                {
                        sead::Matrix34f baseMatrix;
                        mActorPoseKeeper->calcBaseMtx( &baseMatrix );
                        fn_002DDC68( &baseMatrix, &mActorPoseKeeper->getScale() );
                        fn_001D2D44( this, &baseMatrix );
                }
        }
        if ( mActorActionKeeper )
                fn_001D2F90( mActorActionKeeper );
        if ( mShadowKeeper )
                executeLiveActorShadowKeeper( this );
        if ( mHitSensorKeeper )
                mHitSensorKeeper->update();
        if ( mModelKeeper )
                fn_00128DE8( mModelKeeper->getModel() );
        if ( mActorLightCtrl && mActorLightCtrl->_8 == 0 )
                fn_001CA998( mActorLightCtrl, mActorExecuteInfo->getPointerAtOffset18() );
        if ( mSubActorKeeper )
                fn_0018BAE8( mSubActorKeeper );
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
