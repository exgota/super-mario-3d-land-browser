#include <Util/alStringUtil.h>

// Retail item-name mapping. Unknown names use the same zero value as empty.
// The original public API spelling is not independently established.
#ifdef NON_MATCHING
extern "C" int fn_00277EC4( const char* name )
{
        if ( al::isEqualString( name, "" ) )
                return 0;
        if ( al::isEqualString( name, "Coin" ) )
                return 1;
        if ( al::isEqualString( name, "Coin10" ) )
                return 2;
        if ( al::isEqualString( name, "CoinRandom10" ) )
                return 3;
        if ( al::isEqualString( name, "CoinInfinity" ) )
                return 4;
        if ( al::isEqualString( name, "PopCoin" ) )
                return 5;
        if ( al::isEqualString( name, "PopCoin3" ) )
                return 6;
        if ( al::isEqualString( name, "PopCoin5" ) )
                return 7;
        if ( al::isEqualString( name, "PopCoin5High" ) )
                return 8;
        if ( al::isEqualString( name, "PopCoin10" ) )
                return 9;
        if ( al::isEqualString( name, "PopCoinRandom5" ) )
                return 10;
        if ( al::isEqualString( name, "OneUp" ) )
                return 11;
        if ( al::isEqualString( name, "OneUpFast" ) )
                return 12;
        if ( al::isEqualString( name, "KinokoSuper" ) )
                return 13;
        if ( al::isEqualString( name, "KinokoSuperFast" ) )
                return 14;
        if ( al::isEqualString( name, "FireFlower" ) )
                return 15;
        if ( al::isEqualString( name, "FireFlowerForce" ) )
                return 16;
        if ( al::isEqualString( name, "SuperLeaf" ) )
                return 17;
        if ( al::isEqualString( name, "SuperLeafForce" ) )
                return 18;
        if ( al::isEqualString( name, "SuperLeafNormal" ) )
                return 19;
        if ( al::isEqualString( name, "SuperLeafSpecial" ) )
                return 20;
        if ( al::isEqualString( name, "BoomerangFlower" ) )
                return 21;
        if ( al::isEqualString( name, "BoomerangFlowerForce" ) )
                return 22;
        if ( al::isEqualString( name, "SuperStar" ) )
                return 23;
        if ( al::isEqualString( name, "Poison" ) )
                return 24;
        if ( al::isEqualString( name, "PoisonFast" ) )
                return 25;
        if ( al::isEqualString( name, "PatapataWing" ) )
                return 26;
        if ( al::isEqualString( name, "AssistItem" ) )
                return 27;
        if ( al::isEqualString( name, "Propeller" ) )
                return 28;
        if ( al::isEqualString( name, "KickKoura" ) )
                return 29;
        return 0;
}
#endif
