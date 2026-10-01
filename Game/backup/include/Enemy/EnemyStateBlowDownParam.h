#pragma once

class EnemyStateBlowDownParam
{
private:
        float _0;
        float _4;
        float _8;
        float _C;
        float _10;
        int   _14;
        int   _18;

public:
        EnemyStateBlowDownParam( float a, float b, float c, float d, float e, int f, int g )
            : _0( a ), _4( b ), _8( c ), _C( d ), _10( e ), _14( f ), _18( g )
        {
        }
};

static_assert( sizeof( EnemyStateBlowDownParam ) == 0x1C, "" );
