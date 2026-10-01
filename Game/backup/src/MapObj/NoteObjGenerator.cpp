#include "MapObj/NoteObjGenerator.h"

#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveActionCtrl.h>
#include <Nerve/alNerveFunction.h>
#include <Stage/alStageSwitchKeeper.h>

#include "MapObj/NoteObj.h"

static alNerveFunction::NerveActionCollector sCollector;

namespace NrvNoteObjGenerator
{

NERVEACTION_DEF( NoteObjGenerator, Wait )
NERVEACTION_DEF( NoteObjGenerator, Move )
NERVEACTION_DEF( NoteObjGenerator, Disappear )
NERVEACTION_DEF( NoteObjGenerator, Success )

} // namespace NrvNoteObjGenerator

NoteObjGenerator::NoteObjGenerator( const sead::SafeString& name )
    : MapObjActor( name ), mNoteObjGroup( nullptr ), _64( nullptr ), _68( 20 ), _6C( 200 ), _70( nullptr ),
      _74( 180 ), _78( false ), _7C( 0 ), _80( 0 ), _84( 0 )
{
}

extern "C" void fn_00270fc4( al::LiveActor*, float, int ); // MtxConnector (?)
extern "C" void fn_001d581c();

void NoteObjGenerator::exeWait()
{
        if ( al::isOnSwitchA( this ) )
        {
                al::invalidateClipping( this );
                al::showModel( this );
                fn_00270fc4( this, 70.0, 1 );
                al::startAction( this, "Wait" );
                fn_001d581c();
                al::startNerveAction( this, "Move" );
        }
}

void NoteObjGenerator::exeSuccess()
{
        if ( al::isGreaterEqualStep( this, 12 ) )
                kill();
}

#include <LiveActor/alLiveActorGroup.h>
#include <LiveActor/alActorInitUtil.h>
#include <Placement/alPlacementFunction.h>
#include <Rail/alRailFunction.h>

struct NoteSequenceDefinition
{
        const char* name;
        int count;
};
extern "C" const NoteSequenceDefinition dat_003BEAC4[6];

class NoteObjGroup : public al::LiveActorGroup
{
public:
        NoteObjGroup( const char* name, int count ) : LiveActorGroup( name, count ) {}
};

struct ActorMatrixConnector
{
        void* virtualTable;
        al::LiveActor* host;
        u8 enabled;
        u8 active;
        u8 connected;
        u8 padding;
        void* matrix;
        void* connection;
        u8 updated;
        u8 mode;
        u8 paddingTail[2];
        ActorMatrixConnector( al::LiveActor*, bool, bool );
};
static_assert_( sizeof( ActorMatrixConnector ) == 0x18 );

extern "C" void fn_0027FCBC( al::IUseStageSwitch*, const al::ActorInitInfo& );
extern "C" bool fn_0027D180( int*, const al::ActorInitInfo& );
extern "C" bool fn_0027AFF8( int*, const al::ActorInitInfo& );
extern "C" bool fn_002693D8( int*, const al::ActorInitInfo& );
extern "C" void fn_00227988( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_0022735C( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_00225CE8( al::LiveActor*, const al::ActorInitInfo& );
extern "C" void fn_00277E5C( al::LiveActor*, const al::ActorInitInfo&, int );
extern "C" float fn_00260F9C( const al::LiveActor* );
extern "C" ActorMatrixConnector* fn_00270F00( ActorMatrixConnector*, al::LiveActor*, bool, bool );

// Retained source reconstruction. Canonical equality and whole-game behavior remain unproven.
#ifdef NON_MATCHING
void NoteObjGenerator::init( const al::ActorInitInfo& info )
{
        al::initNerveAction( this, "Wait", &sCollector, 0 );
        al::initActor( this, info );
        fn_0027FCBC( this, info );
        if ( al::isExistRail( info ) )
        {
                initRailKeeper( info );
                al::setSyncRailToStart( this );
        }
        al::tryGetArg0( &_68, info );
        int duration = -1;
        fn_0027D180( &duration, info );
        if ( duration > 0 )
                _74 = duration;
        fn_0027AFF8( &_80, info );
        int count = dat_003BEAC4[_80].count;
        int mode = -1;
        fn_002693D8( &mode, info );
        if ( mode == -1 )
                _84 = 0;
        else if ( mode == 1 )
                _84 = 1;
        else if ( mode == 2 )
                _84 = 2;
        switch ( _84 )
        {
        case 0: fn_00227988( this, info ); break;
        case 1: fn_0022735C( this, info ); break;
        case 2: fn_00225CE8( this, info ); break;
        }
        fn_00277E5C( this, info, 1 );
        _6C = fn_00260F9C( this ) / ( count - 1 );
        _64 = new ActorMatrixConnector*[count];
        mNoteObjGroup = new NoteObjGroup( "NoteObjGroup", count );
        for ( int index = 0; index < count; index++ )
        {
                al::LiveActor* note = new NoteObj( this );
                note->init( info );
                mNoteObjGroup->registerActor( note );
                ActorMatrixConnector* connector = new ActorMatrixConnector( note, true, false );
                static_cast<ActorMatrixConnector**>( _64 )[index] = connector;
        }
        mNoteObjGroup->makeActorDeadAll();
        makeActorAppeared();
        al::hideModel( this );
}
#endif
