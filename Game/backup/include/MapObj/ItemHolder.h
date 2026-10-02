#pragma once

#include <Scene/alISceneObj.h>

namespace al
{
class LiveActor;
class LayoutInitInfo;
}
class CoinCharger;

class ItemHolder : public al::ISceneObj
{
private:
        void* mCoins;
        void* mCountUpCoins;
        void* mFireFlowers;
        void* mKinokoOneUps;
        void* mFastKinokoOneUps;
        void* mKinokoPoisons;
        void* mFastKinokoPoisons;
        void* mKinokos;
        void* mFastKinokos;
        void* mBoomerangFlowers;
        void* mPatapataWings;
        void* mAssistItems;
        void* mSuperLeaves;
        void* mSpecialSuperLeaves;
        void* mSuperStars;
        void* mClocks;
        void* mCollectCoins;
        void* mKickKouras;
        CoinCharger*                   mCoinCharger;
        bool mEnabled;
        bool mOption51;
        bool mOption52;
        unsigned char mUntouched53;
        int mOption54;
        int mLimit;

public:
        void initCoinCharger( const al::LayoutInitInfo& info );

        virtual const char* getSceneObjName() const
        {
                return "ItemHolder";
        }

public:
        ItemHolder( bool, bool, int );
};
