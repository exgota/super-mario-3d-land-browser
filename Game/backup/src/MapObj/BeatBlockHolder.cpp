#include "MapObj/BeatBlockHolder.h"

extern "C" void fn_00268144( al::LiveActor*, const sead::SafeString&, int );
extern "C" bool fn_002794F8( int*, const al::ActorInitInfo& );
extern "C" bool fn_0027D1DC( int*, const al::ActorInitInfo& );
extern "C" bool fn_0027D180( int*, const al::ActorInitInfo& );
extern "C" void fn_0027A9D4( al::LiveActor*, const al::ActorInitInfo&, int );
extern "C" void fn_00280538( al::IUseStageSwitch*, const al::ActorInitInfo& );
extern "C" void fn_0027FAB8( al::LiveActor* );
extern "C" void fn_00277de0( al::LiveActor*, const al::ActorInitInfo& );
struct BeatBlockHolderNerve : al::Nerve { virtual void execute( al::NerveKeeper* ) const; };
extern "C" const BeatBlockHolderNerve dat_003F2558;
extern "C" const BeatBlockHolderNerve dat_003F255C;
extern "C" const BeatBlockHolderNerve dat_003F2560;

// Retained source reconstruction; the packet describes the unresolved exact-layout questions.
#ifdef NON_MATCHING
void BeatBlockHolder::init( const al::ActorInitInfo& info )
{
        fn_00277de0( this, info );
        fn_00268144( this, "BeatBlockHolder", 11 );
        interval = 20;
        fn_002794F8( &interval, info );
        int enableArgument = -1;
        fn_0027D1DC( &enableArgument, info );
        if ( enableArgument > 0 )
                enabled = true;
        else
                enabled = false;
        int nerveArgument = -1;
        fn_0027D180( &nerveArgument, info );
        if ( nerveArgument == 0 )
                al::initNerve( this, &dat_003F255C, 0 );
        else if ( nerveArgument == 1 )
                al::initNerve( this, &dat_003F2560, 0 );
        else
                al::initNerve( this, &dat_003F2558, 0 );
        int childCount = al::calcLinkChildNum( info );
        if ( childCount <= 0 )
        {
                makeActorDead();
                return;
        }
        blocks = new BeatBlockGroup( childCount );
        maximumBeatIndex = 0;
        for ( int childIndex = 0; childIndex < childCount; ++childIndex )
        {
                BeatBlock* block = new BeatBlock( al::getLinksActorObjectName( info, childIndex ) );
                fn_0027A9D4( block, info, childIndex );
                if ( block->beatIndex > maximumBeatIndex )
                {
                        int& maximum = maximumBeatIndex;
                        maximum = block->beatIndex;
                }
                blocks->registerActor( block );
        }
        fn_00280538( this, info );
        fn_0027FAB8( this );
}
#endif
