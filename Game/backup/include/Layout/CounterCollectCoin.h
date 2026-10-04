#pragma once

#include <Layout/alLayoutActor.h>

class CounterCollectCoin : public al::LayoutActor {
public:
    bool* collected;
    void collect(int index);
};
