#pragma once

#include <Scene/alISceneObj.h>

class CounterCollectCoin;

// Scene object 9; the intervening fields remain unidentified.
class CoinCollectInfoKeeper : public al::ISceneObj {
public:
    unsigned char _04[0x0C];
    CounterCollectCoin* mCounterCollectCoin;
};
