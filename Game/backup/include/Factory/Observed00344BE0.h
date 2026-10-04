#pragma once

#include <LiveActor/alLiveActor.h>
#include <math/seadQuat.h>
#include <math/seadVector.h>

// Actor extent observed by the Nerve callback at 0x00344BE0.
class ObservedActor00344BE0 : public al::LiveActor
{
public:
    unsigned char unknown60[0x1c];
    sead::Vector3f vector7c;
};

static_assert( sizeof(ObservedActor00344BE0) == 0x88, "Observed actor extent" );

extern "C" {
bool fn_00256B58( const al::IUseNerve*, int );
bool fn_00263BB8( int, int );
const sead::Vector3f* fn_0026CCD0();
void fn_0027306C( sead::Vector3f*, const sead::Vector3f*, const sead::Vector3f* );
void fn_0026AB58( sead::Vector3f* );
void fn_0027D4D8( sead::Quatf*, const sead::Vector3f*, const sead::Vector3f* );
bool fn_0027D5C4( sead::Vector3f* );
extern const al::Nerve dat_003F256C;
}
