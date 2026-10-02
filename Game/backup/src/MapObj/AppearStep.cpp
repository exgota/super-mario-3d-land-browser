#include "MapObj/AppearStep.h"

#include <Functor/alFunctorV0M.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>
#include <Stage/alStageSwitchKeeper.h>

extern "C" bool fn_0027063c( al::LiveActor*, const char* );
extern "C" const char dat_003BCDA8[];
extern "C" const char dat_003BCDB0[];

static_assert_( sizeof( AppearStep ) == 0x60 );

namespace al
{

template <>
FunctorV0M<AppearStep*, void ( AppearStep::* )()>*
FunctorV0M<AppearStep*, void ( AppearStep::* )()>::clone() const
{
        return new FunctorV0M<AppearStep*, void ( AppearStep::* )()>( *this );
}

} // namespace al

namespace NrvAppearStep
{

NERVE_DEF( AppearStep, Appear )
NERVE_DEF( AppearStep, Wait )
NERVE_DEF( AppearStep, Disappear )
NERVE_DEF( AppearStep, End )

} // namespace NrvAppearStep

AppearStep::AppearStep( const sead::SafeString& name ) : MapObjActor( name )
{
}

#ifdef NON_MATCHING

void AppearStep::init( const al::ActorInitInfo& info )
{
        al::initActorPoseTQSV( this );
        al::initMapPartsActor( this, info );
        al::initNerve( this, &NrvAppearStep::End );
        al::initStageSwitchAppear( this, info );
        al::listenStageSwitchOnAppear(
                this, al::FunctorV0M<AppearStep*, void ( AppearStep::* )()>( this, &AppearStep::startAppear ), al::FunctorV0M<AppearStep*, void ( AppearStep::* )()>( this, &AppearStep::startDisappear ) )
                ? makeActorDead()
                : makeActorAppeared();
}

#endif

void AppearStep::startAppear()
{
        if ( al::isNerve( this, &NrvAppearStep::Disappear ) || al::isNerve( this, &NrvAppearStep::End ) )
        {
                al::invalidateClipping( this );
                al::setNerve( this, &NrvAppearStep::Appear );
                makeActorAppeared();
        }
}

void AppearStep::startDisappear()
{
        if ( al::isNerve( this, &NrvAppearStep::Appear ) || al::isNerve( this, &NrvAppearStep::Wait ) )
        {
                al::invalidateClipping( this );
                al::setNerve( this, &NrvAppearStep::Disappear );
        }
}

void AppearStep::exeAppear()
{
        if ( al::isFirstStep( this ) )
                fn_0027063c( this, dat_003BCDA8 );

        if ( al::isActionEnd( this ) )
                al::setNerve( this, &NrvAppearStep::Wait );
}

void AppearStep::exeWait()
{
        if ( al::isFirstStep( this ) )
                al::validateClipping( this );
}

void AppearStep::exeDisappear()
{
        if ( al::isFirstStep( this ) )
                fn_0027063c( this, dat_003BCDB0 );

        if ( al::isActionEnd( this ) )
                al::setNerve( this, &NrvAppearStep::End );
}

void AppearStep::exeEnd()
{
        if ( al::isFirstStep( this ) )
        {
                al::validateClipping( this );
                kill();
        }
}
