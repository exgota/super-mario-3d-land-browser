#include <Enemy/BunbunStateSpinAttack.h>
#include <Enemy/SpinStateSensorContracts.h>
#include <LiveActor/alActorActionParameters.h>
#include <Nerve/alNerve.h>
#include <Util/alStringUtil.h>

extern "C" al::LiveActor* fn_0027D120( al::LiveActor* actor, const char* name );
extern "C" int* fn_0026C9A8( al::LiveActor* actor, const char* name );

struct BunbunSpinAttackParameters
{
        int* mAttackDuration;
        const float* mAnticipation;
        const float* mNear;
        const float* mFar;
        const float* mStop;

        BunbunSpinAttackParameters( al::LiveActor* host, const char* variant )
        {
                mAttackDuration = fn_0026C9A8( host, al::StringTmp<128>( "\x5b\x89\xf1\x93\x5d\x83\x41\x83\x5e\x83\x62\x83\x4e\x5d\x8d\x55\x8c\x82\x8e\x9e\x8a\xd4\x3c\x25\x73\x3e", variant ).cstr() );
                mAnticipation = fn_0026C984( host, "\x5b\x89\xf1\x93\x5d\x83\x41\x83\x5e\x83\x62\x83\x4e\x5d\x97\x5c\x92\x9b" );
                mNear = fn_0026C984( host, al::StringTmp<128>( "\x5b\x89\xf1\x93\x5d\x83\x41\x83\x5e\x83\x62\x83\x4e\x5d\x8b\xdf\x8b\x97\x97\xa3\x3c\x25\x73\x3e", variant ).cstr() );
                mFar = fn_0026C984( host, al::StringTmp<128>( "\x5b\x89\xf1\x93\x5d\x83\x41\x83\x5e\x83\x62\x83\x4e\x5d\x89\x93\x8b\x97\x97\xa3\x3c\x25\x73\x3e", variant ).cstr() );
                mStop = fn_0026C984( host, "\x5b\x91\x53\x91\xcc\x5d\x92\xe2\x8e\x7e" );
        }
};

static_assert( sizeof( BunbunSpinAttackParameters ) == 0x14, "Five action parameter pointers" );

BunbunStateSpinAttack::BunbunStateSpinAttack( al::LiveActor* host, al::LiveActorGroup* trailActors,
                                           sead::Matrix34f* firstMatrix, sead::Matrix34f* secondMatrix,
                                           const char* variant )
        : al::ActorStateBase( "\x83\x75\x83\x93\x83\x75\x83\x93\x53\x74\x61\x74\x65\x20\x5b\x83\x58\x83\x73\x83\x93\x83\x41\x83\x5e\x83\x62\x83\x4e\x5d" , host ), mArm( nullptr ),
          mTrailActors( trailActors ), mParameters( nullptr ),
          mFirstMatrix( firstMatrix ), mSecondMatrix( secondMatrix )
{
        mArm = fn_0027D120( host, "Arm" );
        mParameters = new BunbunSpinAttackParameters( host, variant );
        initNerve( &dat_003F2E90, 0 );
}
