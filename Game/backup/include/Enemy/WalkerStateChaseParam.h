#pragma once

#include <prim/seadSafeString.h>

class WalkerStateChaseParam
{
private:
        float                     _0;
        int                       _4;
        int                       _8;
        float                     _C;
        int                       _10;
        bool                      _14;
        bool                      _15;
        sead::FixedSafeString<32>  mRunAction;
        sead::FixedSafeString<32>  mWaitAction;

public:
        WalkerStateChaseParam( bool, bool, float, float, float, float, float, const char*, const char* );
};

static_assert_( sizeof( WalkerStateChaseParam ) == 0x70 );
