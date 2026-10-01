#include "Sequence/ProductSequence.h"

#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>

#include "Sequence/ProductStateCourseSelect.h"
#include "Sequence/ProductStateStage.h"

namespace NrvProductSequence
{

NERVE_DEF( ProductSequence, Title )
NERVE_DEF( ProductSequence, Opening )
NERVE_DEF( ProductSequence, CourseSelect )
NERVE_DEF( ProductSequence, Stage )
NERVE_DEF( ProductSequence, KinopioHouse )
NERVE_DEF( ProductSequence, MysteryBox )
NERVE_DEF( ProductSequence, Ending )
NERVE_DEF( ProductSequence, GameOverRoom )
NERVE_DEF( ProductSequence, Unk1 )

} // namespace NrvProductSequence

ProductSequence::ProductSequence( const char* name )
    : Sequence( name ), mStageStartParam( nullptr ), _14C( nullptr ), _150( nullptr ), mWipeKeeper( nullptr ),
      _158( nullptr ), _15C( nullptr ), mStateTitle( nullptr ), mStateOpening( nullptr ),
      mStateCourseSelect( nullptr ), mStateStage( nullptr ), mStateKinopioHouse( nullptr ),
      mStateMysteryBox( nullptr ), mStateEnding( nullptr ), mStateGameOverRoom( nullptr ), _180( nullptr ),
      _184( nullptr ), _188( nullptr ), _18C( nullptr ), _190( nullptr )
{
}

#ifdef NON_MATCHING
void ProductSequence::init()
{
} // needed for vtable

#endif

extern "C" bool fn_0025ba7c( const char* );
extern "C" const char dat_003a8e64[];
extern "C" const NrvProductSequence::ProductSequenceNrvTitle dat_003ef538;
extern "C" const NrvProductSequence::ProductSequenceNrvOpening dat_003ef53c;
extern "C" const NrvProductSequence::ProductSequenceNrvCourseSelect dat_003ef540;
extern "C" const NrvProductSequence::ProductSequenceNrvUnk1 dat_003ef55c;

void ProductSequence::exeTitle()
{
        if ( al::updateNerveState( this ) )
        {
                if ( fn_0025ba7c( dat_003a8e64 ) )
                        al::setNerve( this, &dat_003ef540 );
                else
                        al::setNerve( this, &dat_003ef53c );
        }
}

void ProductSequence::exeOpening()
{
        if ( al::updateNerveState( this ) )
                al::setNerve( this, &dat_003ef540 );
}

void ProductSequence::exeKinopioHouse()
{
        if ( al::updateNerveState( this ) )
        {
                mStateCourseSelect->_10 = 4;
                al::setNerve( this, &dat_003ef540 );
        }
}

extern "C" bool fn_0025ddd0();

void ProductSequence::exeEnding()
{
        if ( al::updateNerveState( this ) )
        {
                if ( fn_0025ddd0() )
                        al::setNerve( this, &dat_003ef55c );
                else
                        al::setNerve( this, &dat_003ef538 );
        }
}
