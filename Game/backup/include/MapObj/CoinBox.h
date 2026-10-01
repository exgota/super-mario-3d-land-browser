#pragma once

#include <MapObj/alMapObjActor.h>
#include <stddef.h>

// Minimal recovered CoinBox layout. The archive, actor factory, constructor,
// and virtual table independently establish the class identity. Unrecovered
// methods are deliberately not declared here.
class CoinBox : public al::MapObjActor
{
public:
        al::HitSensor* mAttachedSensor;     // 0x60
        bool mUsesMiniWaitAnimation;        // 0x64
        float mDistanceSinceCoin;          // 0x68
        sead::Vector3f mPreviousTranslation;// 0x6C
        int mCoinsRemaining;               // 0x78; nonpositive means unlimited
        bool mUnrecovered7C;               // set by the detach routine
};

typedef char CoinBoxAttachedSensorOffset[ offsetof( CoinBox, mAttachedSensor ) == 0x60 ? 1 : -1 ];
typedef char CoinBoxMiniAnimationOffset[ offsetof( CoinBox, mUsesMiniWaitAnimation ) == 0x64 ? 1 : -1 ];
typedef char CoinBoxDistanceOffset[ offsetof( CoinBox, mDistanceSinceCoin ) == 0x68 ? 1 : -1 ];
typedef char CoinBoxTranslationOffset[ offsetof( CoinBox, mPreviousTranslation ) == 0x6C ? 1 : -1 ];
typedef char CoinBoxCoinCountOffset[ offsetof( CoinBox, mCoinsRemaining ) == 0x78 ? 1 : -1 ];
typedef char CoinBoxDetachFlagOffset[ offsetof( CoinBox, mUnrecovered7C ) == 0x7C ? 1 : -1 ];
typedef char CoinBoxSize[ sizeof( CoinBox ) == 0x80 ? 1 : -1 ];

// The state name is not established. Preserve the address-based identity.
extern "C" void fn_0030FABC( CoinBox* actor );
