#include "MapObj/ItemRouletteState.h"

#include <LiveActor/alLiveActorFunction.h>

// Every address-derived import below starts an unchanged existing map interval.
// Names are deliberately withheld where only the ABI and operation are known.
extern "C" bool fn_002730F8( int*, const al::ActorInitInfo& );
extern "C" bool fn_00266148( int*, const al::ActorInitInfo& );
extern "C" bool fn_002794F8( int*, const al::ActorInitInfo& );
extern "C" void fn_0027EFB0( al::LiveActor*, const al::ActorInitInfo&, sead::Vector3f* );
extern "C" void fn_002742E0( al::LiveActor*, const al::ActorInitInfo&, const char*, int );
extern "C" void fn_0011BFDC( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_00226C98( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_00227988( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_0022735C( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_00225CE8( al::LiveActor*, const al::ActorInitInfo& );
extern "C" bool fn_0016BB58();
extern "C" al::LiveActor* fn_00256ACC( al::LiveActor*, const al::ActorInitInfo&,
        const char*, const char*, const char*, const char* );
extern "C" void fn_0027ED20( al::LiveActor*, al::LiveActor* );
extern "C" void fn_0027E9EC( al::LiveActor*, float );
extern "C" void fn_0026A9FC( al::LiveActor* );
extern "C" int fn_0026B000( int, int );
extern "C" const char dat_003C0F7C[]; // shared "DummyModel" string, entire 12-byte row

#ifdef NON_MATCHING
// Recoverable source proposal only. No behavioral or exact-match claim.
ItemRouletteState::ItemRouletteState( al::LiveActor* host,
        const al::ActorInitInfo& info, bool omitPlacementOffset )
    : NerveStateBase( "State[\203\213\201[\203\214\203b\203g]" ), mHost( host ), mSelectedItem( 1 ), mPlacementArg( -1 ),
      mHasArg7( false ), mCounter( 0 ), mPlacementOffset( 0.0f, 0.0f, 0.0f )
{
        mItems[0] = nullptr;
        mItems[1] = nullptr;
        mItems[2] = nullptr;
        mItems[3] = nullptr;
        mItems[4] = nullptr;
        if ( fn_002730F8( &mPlacementArg, info ) )
                mHasArg7 = true;
        fn_00266148( &mPlacementArg, info );
        fn_0027EFB0( host, info, omitPlacementOffset ? nullptr : &mPlacementOffset );

        int itemSet = 0;
        fn_002794F8( &itemSet, info );
        switch ( itemSet )
        {
        case 0:
        case 3:
                fn_002742E0( mHost, info, "DummyModel", 4 );
                break;
        case 1:
                fn_002742E0( mHost, info, "DummyModel", 3 );
                break;
        case 2:
                fn_002742E0( mHost, info, "DummyModel", 5 );
                break;
        }

        if ( itemSet != 3 )
        {
                fn_0011BFDC( host, info );
                {
                        al::LiveActor* display = fn_00256ACC( mHost, info, "\203_\203~\201[\203\202\203f\203\213[\203X\201[\203p\201[\203X\203^\201[]", "SuperStar", dat_003C0F7C, nullptr );
                        fn_0027ED20( mHost, display );
                        al::hideModel( display );
                        mItems[0] = display;
                        fn_0027E9EC( display, 0.7f );
                }
        }
        fn_00226C98( host, info );
        {
                al::LiveActor* display = fn_00256ACC( mHost, info, "\203_\203~\201[\203\202\203f\203\213[\203X\201[\203p\201[\203L\203m\203R]", "KinokoSuper", dat_003C0F7C, nullptr );
                fn_0027ED20( mHost, display );
                al::hideModel( display );
                mItems[1] = display;
                fn_0027E9EC( display, 0.7f );
        }

        const char* leafModel = fn_0016BB58() ? "SuperLeafSpecial" : "SuperLeaf";
        fn_00227988( host, info );
        {
                al::LiveActor* display = fn_00256ACC( mHost, info, "\203_\203~\201[\203\202\203f\203\213[\203X\201[\203p\201[\202\261\202\314\202\315]", leafModel, dat_003C0F7C, nullptr );
                fn_0027ED20( mHost, display );
                al::hideModel( display );
                mItems[2] = display;
                fn_0027E9EC( display, 0.7f );
        }

        if ( itemSet != 1 )
        {
                fn_0022735C( host, info );
                {
                        al::LiveActor* display = fn_00256ACC( mHost, info, "\203_\203~\201[\203\202\203f\203\213[\203t\203@\203C\203A\203t\203\211\203\217\201[]", "FireFlower", dat_003C0F7C, nullptr );
                        fn_0027ED20( mHost, display );
                        al::hideModel( display );
                        mItems[3] = display;
                        fn_0027E9EC( display, 0.7f );
                }
                if ( itemSet == 2 || itemSet == 3 )
                {
                        fn_00225CE8( host, info );
                        {
                                al::LiveActor* display = fn_00256ACC( mHost, info, "\203_\203~\201[\203\202\203f\203\213[\203u\201[\203\201\203\211\203\223\203t\203\211\203\217\201[]", "BoomerangFlower", dat_003C0F7C, nullptr );
                                fn_0027ED20( mHost, display );
                                al::hideModel( display );
                                mItems[4] = display;
                                fn_0027E9EC( display, 0.7f );
                        }
                }
        }

        int next = mSelectedItem;
        int attempts = 0;
        for ( ; attempts < 5; ++attempts )
        {
                next = fn_0026B000( next + 6, 5 );
                if ( mItems[next] )
                        break;
        }
        if ( attempts == 5 )
                next = 0;
        fn_0026A9FC( mItems[next] );
}
#endif
