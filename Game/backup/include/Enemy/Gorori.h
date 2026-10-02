#pragma once

#include <MapObj/alMapObjActor.h>

// Layout recovered from the retail constructor at 0x0026AB84. The secondary
// interface at 0x60 and members unused by init retain address-based names.
class Gorori : public al::MapObjActor
{
private:
        void* mInterface60;
        float mMoveSpeed;
        al::LiveActor* mBreakActor;
        void* mState6C;
        void* mState70;
        void* mState74;
        int mMoveFrames;
        bool mFlag7C;
        bool mFlag7D;
        float mState80;
        sead::Matrix34f mMoveEffectMtx;
        sead::Matrix34f mLandEffectMtx;
        bool mForceBig;
        void* mStateE8;
        bool mFlagEC;
        bool mFlagED;
        void* mStateF0;
        void* mStateF4;

        void setMoveSpeed( int speed );
        void initCommon( const al::ActorInitInfo& info );

public:
        virtual void init( const al::ActorInitInfo& info );
};

static_assert( sizeof( Gorori ) == 0xF8, "" );
