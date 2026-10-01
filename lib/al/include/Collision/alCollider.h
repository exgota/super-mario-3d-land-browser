#pragma once

#include <math/seadVector.h>

namespace al
{

class Collider
{
private:
        u8             _0[ 0x14 ];
        float          mRadius;
        u8             _18[ 0xc ];
        u32            mCollisionCount;
        u8             _28[ 4 ];
        sead::Vector3f mCollisionDisplacement;
        u8             _38[ 0x88 ];
        float          mGroundDistance;
        u8             _C4[ 0x88 ];
        float          mResultDistance1;
        u8             _150[ 0x88 ];
        float          mResultDistance2;
        int            mCollisionIndex;
        u8             _1E0[ 0x10 ];
        sead::Vector3f mPreviousPosition;
        float          mPreviousRadius;

        void clearCollisionResults()
        {
                mCollisionCount       = 0;
                mGroundDistance       = -99999.0f;
                mResultDistance1      = -99999.0f;
                mResultDistance2      = -99999.0f;
                mCollisionDisplacement = sead::Vector3f( 0.0f, 0.0f, 0.0f );
        }

public:
        float getGroundDistance()
        {
                return mGroundDistance;
        }

        void onInvalidate();
};

static_assert( sizeof( Collider ) == 0x200, "" );

} // namespace al
