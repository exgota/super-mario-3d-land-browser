#pragma once

#include <prim/seadSafeString.h>

class WalkerStateWanderParam
{
private:
        int                       _0, _4;
        float                     _8, _C, _10;
        sead::FixedSafeString<32>  mWalkAction;
        sead::FixedSafeString<32>  mWaitAction;

public:
        WalkerStateWanderParam( int a, int b, float c, float d, float e, const char* f, const char* g );
};

static_assert_( sizeof( WalkerStateWanderParam ) == 0x6C );
