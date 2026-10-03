#include <MapObj/BalanceTruck.h>

#include <LiveActor/alLiveActorFunction.h>

namespace al
{
void startHitReactionStart( const LiveActor* actor );
void startHitReactionEnd( const LiveActor* actor );
}

void BalanceTruck::updateMovePower()
{
        if ( mDirection > 0 )
        {
                mMovePower += 2.0f;
                mMovePower = mMovePower > 4.0f ? 4.0f : mMovePower;
                mAnimationPower = mMovePower;
                if ( mPreviousDirection == 0 || mHeldFrames < 0 )
                        mHeldFrames = 0;
                ++mHeldFrames;
                mHeldFrames = mHeldFrames > 40 ? 40 : mHeldFrames;
        }
        else if ( mDirection < 0 )
        {
                mMovePower -= 2.0f;
                mMovePower = mMovePower > -4.0f ? mMovePower : -4.0f;
                mAnimationPower = mMovePower;
                if ( mPreviousDirection == 0 || mHeldFrames > 0 )
                        mHeldFrames = 0;
                --mHeldFrames;
                mHeldFrames = mHeldFrames > -40 ? mHeldFrames : -40;
        }
        else if ( mMovePower >= 0.0f )
        {
                float brake = 4.0f;
                if ( mHeldFrames > 0 )
                        brake = 8.0f / mHeldFrames;
                mMovePower -= brake;
                mMovePower = mMovePower > 0.0f ? mMovePower : 0.0f;
                mAnimationPower -= 2.0f;
                mAnimationPower = mAnimationPower > 0.0f ? mAnimationPower : 0.0f;
        }
        else
        {
                float brake = 4.0f;
                if ( mHeldFrames < 0 )
                        brake = -8.0f / mHeldFrames;
                mMovePower += brake;
                mMovePower = mMovePower > 0.0f ? 0.0f : mMovePower;
                mAnimationPower += 2.0f;
                mAnimationPower = mAnimationPower > 0.0f ? 0.0f : mAnimationPower;
        }

        if ( mPreviousDirection != mDirection )
        {
                if ( mDirection > 0 )
                {
                        al::startAction( this, "Front" );
                        al::startHitReactionStart( this );
                }
                else if ( mDirection < 0 )
                {
                        al::startAction( this, "Back" );
                        al::startHitReactionStart( this );
                }
                else
                {
                        al::startHitReactionEnd( this );
                        al::startAction( this, "Wait" );
                }
        }
}
