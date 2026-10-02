#include <Clipping/alClippingActorHolder.h>
#include <Clipping/alClippingDirector.h>
#include <Collision/alCollider.h>
#include <Collision/alCollisionUtil.h>
#include <Execute/alExecuteTableHolder.h>
#include <LiveActor/alActorActionKeeper.h>
#include <LiveActor/alActorPoseFunction.h>
#include <LiveActor/alHitSensorKeeper.h>
#include <LiveActor/alLiveActor.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alLiveActorKit.h>
#include <LiveActor/alSensorFunction.h>
#include <LiveActor/alSubActorFunction.h>
#include <Math/alMtxUtil.h>
#include <Model/alAnimPlayerSimple.h>
#include <Model/alModelCtr.h>
#include <Model/alModelKeeper.h>
#include <Nerve/alNerveActionCtrl.h>
#include <Nerve/alNerveFunction.h>

extern "C" const sead::Matrix34f* fn_002519B0( al::ModelKeeper* keeper, const char* jointName );

extern "C" void fn_00129074( al::LiveActor* actor, const sead::Matrix34f& matrix, const sead::Vector3f& scale );
extern "C" void fn_002DDC68( sead::Matrix34f* matrix, const sead::Vector3f& scale );
extern "C" void fn_001D2D44( al::LiveActor* actor, const sead::Matrix34f* matrix );
extern "C" void fn_001D30C4( al::ActorActionKeeper* keeper, const char* actionName );

void alLiveActorFunction::calcAnimDirect( al::LiveActor* actor )
{
        sead::Matrix34f baseMtx;
        actor->mActorPoseKeeper->calcBaseMtx( &baseMtx ); /* alActorPoseFunction::calcBaseMtx */
        if ( actor->mModelKeeper )
                ::fn_00129074( actor, baseMtx, actor->mActorPoseKeeper->getScale() /* al::getScale */ );
        if ( actor->mCollisionParts )
        {
                ::fn_002DDC68( &baseMtx, actor->mActorPoseKeeper->getScale() /* al::getScale */ );
                ::fn_001D2D44( actor, &baseMtx );
        }
        if ( actor->getEffectKeeper() )
                actor->getEffectKeeper()->update();
        if ( actor->mSubActorKeeper )
                alSubActorFunction::tryCalcAnim( actor->mSubActorKeeper );
}

namespace al
{

// ??
bool isClipped( const LiveActor* actor )
{
        return actor->getLiveActorFlag().isClipped;
}

bool isInvalidClipping( const LiveActor* actor )
{
        return actor->getLiveActorFlag().isInvalidClipping;
}

bool isDead( const LiveActor* actor )
{
        return actor->getLiveActorFlag().isDead;
}

bool isAlive( const LiveActor* actor )
{
        return !actor->getLiveActorFlag().isDead;
}

void onCollide( LiveActor* actor )
{
        Collider* collider                     = actor->getCollider();
        actor->getLiveActorFlag().isOffCollide = false;
        if ( collider )
                collider->onInvalidate();
}

void offCollide( LiveActor* actor )
{
        actor->getLiveActorFlag().isOffCollide = true;
}

void onDrawClipping( LiveActor* actor )
{
        actor->getLiveActorFlag().isDrawClipping = true;
        if ( isClipped( actor ) )
        {
                alActorSystemFunction::addToExecutorMovement( actor );
                if ( actor->getHitSensorKeeper() )
                {
                        actor->getHitSensorKeeper()->validateBySystem();
                        alSensorFunction::updateHitSensorsAll( actor );
                }
        }
}

void invalidateClipping( LiveActor* actor )
{
        if ( isClipped( actor ) )
                actor->endClipped();

        if ( !isInvalidClipping( actor ) )
                getLiveActorKit()->getClippingDirector()->getClippingActorHolder()->invalidateClipping(
                        actor );
}

void validateClipping( LiveActor* actor )
{
        if ( isInvalidClipping( actor ) )
                getLiveActorKit()->getClippingDirector()->getClippingActorHolder()->validateClipping( actor );
}

// ModelKeeper

#ifdef NON_MATCHING

// optimization is too smart
void hideModel( LiveActor* actor )
{
        if ( isAlive( actor ) && !isClipped( actor ) )
        {
                if ( actor->getModelKeeper() )
                        actor->getModelKeeper()->hide();
                if ( actor->getShadowKeeper() )
                        hideShadow( actor );
                alActorSystemFunction::removeFromExecutorDraw( actor );
        }
        actor->getLiveActorFlag().isHideModel = true;
}
#endif

#ifdef NON_MATCHING
// register swap, maybe inlined
__attribute__((noinline)) bool tryStartMclAnimIfExist( LiveActor* actor, const char* animName )
{
        AnimPlayerSimple* animPlayer = actor->getModelKeeper()->getModel()->getMclAnimPlayer();
        if ( animPlayer && animPlayer->isAnimExist( animName ) )
        {
                actor->getModelKeeper()->getModel()->getMclAnimPlayer()->startAnim( animName );
                return true;
        }
        return false;
}
#endif

struct JointMatrixRequest
{
        ModelKeeper* keeper;
        const char* jointName;
};

#pragma push
#pragma no_inline
static __value_in_regs JointMatrixRequest makeJointMatrixRequest( const LiveActor* actor, const char* jointName )
{
        JointMatrixRequest request = { actor->getModelKeeper(), jointName };
        return request;
}
#pragma pop

void calcJointPos( sead::Vector3f* out, const LiveActor* actor, const char* jointName )
{
        const JointMatrixRequest request = makeJointMatrixRequest( actor, jointName );
        const sead::Matrix34f* jointMtx = ::fn_002519B0( request.keeper, request.jointName );
        out->x                          = jointMtx->m[ 0 ][ 3 ];
        out->y                          = jointMtx->m[ 1 ][ 3 ];
        out->z                          = jointMtx->m[ 2 ][ 3 ];
}

// HitSensorKeeper

HitSensor* getHitSensor( const LiveActor* actor, const char* name )
{
        return actor->getHitSensorKeeper()->getSensor( name );
}

float getSensorRadius( const LiveActor* actor, const char* sensorName )
{
        return getHitSensor( actor, sensorName )->getRadius();
}

// NerveKeeper
#pragma no_inline

void startNerveAction( LiveActor* actor, const char* actionName )
{
        if ( actor->getActorActionKeeper() )
                ::fn_001D30C4( actor->getActorActionKeeper(), actionName );
        alNerveFunction::setNerveAction( actor, actionName );
}

#ifdef NON_MATCHING
// registers
void initNerve( LiveActor* actor, const Nerve* nerve, int maxNerveStates )
{
        actor->initNerveKeeper( new NerveKeeper( actor, nerve, maxNerveStates ) );
}
#endif

#ifdef NON_MATCHING
// registers, too big for tail-reorder
void initNerveAction( LiveActor* actor, const char* name, alNerveFunction::NerveActionCollector* collector, int maxNerveStates )
{
        NerveActionCtrl* nerveActionCtrl = new NerveActionCtrl( collector );
        NerveAction*     nerve           = nerveActionCtrl->findNerve( name );
        NerveKeeper*     nk              = new NerveKeeper( actor, nerve, maxNerveStates );
        actor->initNerveKeeper( nk );
        actor->getNerveKeeper()->initNerveAction( nerveActionCtrl );
        startNerveAction( actor, name );
}
#endif

} // namespace al

// This source-local prefix follows the original animation-provider loads.
struct ActionAnimationController
{
        void* reserved[ 8 ];
        void* animationChannels[ 6 ];
};

extern "C" bool fn_0024FDB8( al::ActorActionKeeper* keeper, const char* actionName );
extern "C" bool fn_0024FD4C( al::LiveActor* actor, const char* actionName, int index );
extern "C" bool fn_0024FD08( al::LiveActor* actor, const char* actionName, int index );
extern "C" bool fn_00265128( al::LiveActor* actor, const char* actionName );
extern "C" bool fn_0024FCB8( al::LiveActor* actor, const char* actionName );
extern "C" bool fn_0024FC68( al::LiveActor* actor, const char* actionName );

extern "C" bool fn_002636E4( const al::LiveActor* actor )
{
        const ActionAnimationController* controller =
                reinterpret_cast<const ActionAnimationController*>( actor->getModelKeeper()->getModel() );
        return controller->animationChannels[ 0 ] != 0;
}

extern "C" bool fn_00257754( const al::LiveActor* actor )
{
        const ActionAnimationController* controller =
                reinterpret_cast<const ActionAnimationController*>( actor->getModelKeeper()->getModel() );
        return controller->animationChannels[ 2 ] != 0;
}

extern "C" bool fn_0025776C( const al::LiveActor* actor )
{
        const ActionAnimationController* controller =
                reinterpret_cast<const ActionAnimationController*>( actor->getModelKeeper()->getModel() );
        return controller->animationChannels[ 3 ] != 0;
}

extern "C" bool fn_001CBEB8( const al::LiveActor* actor )
{
        const ActionAnimationController* controller =
                reinterpret_cast<const ActionAnimationController*>( actor->getModelKeeper()->getModel() );
        return controller->animationChannels[ 4 ] != 0;
}

extern "C" bool fn_001CBEA0( const al::LiveActor* actor )
{
        const ActionAnimationController* controller =
                reinterpret_cast<const ActionAnimationController*>( actor->getModelKeeper()->getModel() );
        return controller->animationChannels[ 1 ] != 0;
}

extern "C" bool fn_001CBF08( const al::LiveActor* actor )
{
        const ActionAnimationController* controller =
                reinterpret_cast<const ActionAnimationController*>( actor->getModelKeeper()->getModel() );
        return controller->animationChannels[ 5 ] != 0;
}

namespace al
{

void startAction( LiveActor* actor, const char* actionName )
{
        if ( actor->getActorActionKeeper() && ::fn_0024FDB8( actor->getActorActionKeeper(), actionName ) )
                return;
        ::fn_0024FD4C( actor, actionName, 0 );
        ::fn_0024FD08( actor, actionName, 0 );
        tryStartMclAnimIfExist( actor, actionName );
        ::fn_00265128( actor, actionName );
        ::fn_0024FCB8( actor, actionName );
        ::fn_0024FC68( actor, actionName );
}


} // namespace al
