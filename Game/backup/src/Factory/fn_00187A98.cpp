#include <Layout/CounterCollectCoin.h>
#include <Scene/CoinCollectInfoKeeper.h>
#include <Scene/SceneObjFactory.h>

extern "C" void fn_00187A98(int index) {
    CoinCollectInfoKeeper* keeper = static_cast<CoinCollectInfoKeeper*>(
        al::getSceneObj(SceneObjType_CoinCollectInfoKeeper));
    return keeper->mCounterCollectCoin->collect(index);
}
