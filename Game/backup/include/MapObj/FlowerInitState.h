#pragma once

#include <Nerve/alNerveStateBase.h>

namespace al { class LiveActor; }

// Descriptive reconstruction identities approved for this import family.
// The observed base interface does not recover the original inheritance chain.
namespace FlowerInit
{

class Param
{
        float mValues[8];
public:
        Param();
};

class Appear : public al::NerveStateBase
{
        al::LiveActor* mHost;
public:
        Appear( al::LiveActor* );
        virtual void appear();
};

// The original formal types of the two settings are unresolved. All twenty
// observed direct calls supply zero or one; the bodies store their low bytes.
class Forward : public al::NerveStateBase
{
        al::LiveActor* mHost;
        unsigned char mFirstSetting;
        unsigned char mSecondSetting;
        unsigned char _12[2];
        unsigned int mStateWords[3];
        Param* mParam;
public:
        Forward( al::LiveActor*, Param*, bool, int );
        virtual void appear();
};

class Vertical : public al::NerveStateBase
{
        al::LiveActor* mHost;
        unsigned char mFirstSetting;
        unsigned char mSecondSetting;
        unsigned char _12[2];
        Param* mParam;
public:
        Vertical( al::LiveActor*, Param*, bool, int );
        virtual void appear();
};

class Release : public al::NerveStateBase
{
        al::LiveActor* mHost;
        Param* mParam;
        unsigned int mStateWords[3];
        unsigned char mFlag;
public:
        Release( al::LiveActor*, Param* );
        virtual void appear();
};

class Ground : public al::NerveStateBase
{
        al::LiveActor* mHost;
        Param* mParam;
        unsigned int _14;
        void* mAuxiliaryState;
        unsigned int mStateWords[3];
public:
        Ground( al::LiveActor*, Param* );
        virtual void appear();
};

// State words preserve observed storage without assigning unproved semantics.
static_assert( sizeof( Param ) == 0x20, "Observed parameter allocation" );
static_assert( sizeof( Appear ) == 0x10, "Observed appearance-state allocation" );
static_assert( sizeof( Forward ) == 0x24, "Observed forward-state allocation" );
static_assert( sizeof( Vertical ) == 0x18, "Observed vertical-state allocation" );
static_assert( sizeof( Release ) == 0x24, "Observed release-state allocation" );
static_assert( sizeof( Ground ) == 0x28, "Observed ground-state allocation" );

} // namespace FlowerInit
