#include "MapObj/SwingNeedleRoller.h"

#include <Functor/alFunctorV0M.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alLiveActorGroup.h>
#include <Nerve/alNerve.h>
#include <Placement/alPlacementFunction.h>
#include <Yaml/alByamlIter.h>
#include <math.h>

// These local type names describe independently observed constructor contracts.
// They are not assertions about the original source names.
namespace swing_needle_roller
{
class ChainModel : public al::LiveActor
{
        const char* mArchive;
        const char* mResource;
        u8 mFlag;
public:
        ChainModel( const sead::SafeString& name, const char* archive,
                const char* resource ); // 0x002741D8
};

class SwingState
{
        u8 mStorage[0x1C];
public:
        SwingState( const al::ActorInitInfo& info ); // 0x00274218
};

class RollerModel : public al::LiveActor
{
        al::LiveActor* mOwner;
public:
        u8 mFlag64;
        u8 mFlag65;
        RollerModel( const char* name, al::LiveActor* owner ); // 0x001D1178
};

class ChainGroup : public al::LiveActorGroup
{
public:
        ChainGroup( const char* name, int capacity )
                : al::LiveActorGroup( name, capacity ) {}
};

class ObservedNerve : public al::Nerve
{
public:
        virtual void execute( al::NerveKeeper* ) const;
};

static_assert( sizeof( ChainModel ) == 0x6C, "" );
static_assert( sizeof( SwingState ) == 0x1C, "" );
static_assert( sizeof( RollerModel ) == 0x68, "" );
static_assert( sizeof( ChainGroup ) == 0x14, "" );
}

extern "C"
{
bool fn_00277B58( float*, const al::ActorInitInfo& );
bool fn_00270ED8( float*, const al::ActorInitInfo& );
bool fn_0026ACC8( float*, const al::ActorInitInfo& );
void fn_002742E0( al::LiveActor*, const al::ActorInitInfo&, int, int );
void fn_0027ED20( al::LiveActor*, al::LiveActor* );
void fn_0027A488( sead::Vector3f*, const sead::Quatf& );
void fn_0027894C( sead::Vector3f*, const sead::Quatf& );
const u8* fn_00278320( const sead::SafeString&, const sead::SafeString& );
void fn_00267F2C( al::LiveActor*, const al::ActorInitInfo&,
        const sead::SafeString&, const char* );
void fn_0026D5E0( al::LiveActor*, float*, const char* );
void fn_001EB9D0( al::LiveActor*, const char*, const sead::Vector3f* );
void fn_00277B38( al::LiveActor*, const char*, float );
void fn_0028065C( al::LiveActor* );
bool fn_0027FC08( al::IUseStageSwitch*, const al::FunctorBase& );
void fn_00280610( al::LiveActor*, const al::Nerve* );
void fn_00256C50( SwingNeedleRoller* );
void fn_0027EE34( al::LiveActor*, const al::ActorInitInfo&, int );
void fn_0027FD10( al::LiveActor*, const sead::Vector3f*, float );
void fn_0027FAB8( al::LiveActor* );

extern const swing_needle_roller::ObservedNerve dat_003F2BF8;
extern const swing_needle_roller::ObservedNerve dat_003F2BFC;
extern void ( SwingNeedleRoller::*const dat_003BF4A4 )();
}

// Unaccepted reconstruction. No functional NonMatching or byte-exact claim.
void SwingNeedleRoller::init( const al::ActorInitInfo& info )
{
        fn_00277B58( &mChainLength, info );
        int linksPerSide = static_cast<int>( ( mChainLength + 30.0f ) / 60.0f ) - 1;
        if ( linksPerSide <= 0 )
                linksPerSide = 1;
        int chainCount = linksPerSide * 2;
        fn_002742E0( this, info, 0, chainCount + 1 );
        al::initActorWithArchiveName( this, info, "SwingNeedleRoller" );
        al::initNerve( this, &dat_003F2BFC );

        mChains = new swing_needle_roller::ChainGroup( "\x8d\xbd\x83\x8a\x83\x58\x83\x67", chainCount );
        for ( int i = 0; i < chainCount; ++i )
        {
                swing_needle_roller::ChainModel* chain = new swing_needle_roller::ChainModel(
                        "\x8d\xbd", "WanwanChain", "SwingNeedleRoller" );
                al::initCreateActorNoPlacementInfo( chain, info );
                mChains->registerActor( chain );
                fn_0027ED20( this, chain );
        }

        mInitialQuat = al::getQuat( this );
        mSwingState = new swing_needle_roller::SwingState( info );
        mLowestY = al::getTrans( this ).y - mChainLength;
        if ( fn_00270ED8( &mLowestY, info ) )
        {
                if ( mLowestY <= 0.0f )
                        mLowestY = al::getTrans( this ).y - mChainLength;
                else
                {
                        mLowestY = al::getTrans( this ).y - mLowestY;
                        mFindGround = false;
                }
        }
        fn_0026ACC8( &mChainSideOffset, info );
        fn_0027A488( &mFront, mInitialQuat );
        fn_0027894C( &mSide, mInitialQuat );

        const char* objectName = "SwingNeedleRoller";
        al::tryGetObjectName( &objectName, info );
        const char* rollerArchive = "NeedleRoller";
        mRollerLength = 400.0f;
        al::ByamlIter params( fn_00278320( "SwingNeedleRoller", "RollerParam" ) );
        al::ByamlIter entry;
        if ( params.tryGetIterByKey( &entry, objectName ) )
        {
                entry.tryGetStringByKey( &rollerArchive, "ArcName" );
                entry.tryGetFloatByKey( &mRollerLength, "Length" );
        }

        mLeftHead = new al::LiveActor( "\x8d\xaa\x96\x7b\x83\x82\x83\x66\x83\x8b[\x8d\xb6]" );
        al::initActorWithArchiveName( mLeftHead, info, "ChainHead" );
        mLeftHead->appear();
        mRightHead = new al::LiveActor( "\x8d\xaa\x96\x7b\x83\x82\x83\x66\x83\x8b[\x89\x45]" );
        al::initActorWithArchiveName( mRightHead, info, "ChainHead" );
        mRightHead->appear();

        mRoller = new swing_needle_roller::RollerModel(
                "\x83\x67\x83\x51\x83\x8d\x81\x5b\x83\x8b\x95\x94\x95\xaa", this );
        mRoller->mFlag64 = false;
        fn_00267F2C( mRoller, info, rollerArchive, nullptr );
        fn_0026D5E0( mRoller, &mRollAngle, rollerArchive );
        al::setQuat( mRoller, mInitialQuat );

        fn_001EB9D0( this, "Damage", al::getTransPtr( mRoller ) );
        fn_00277B38( this, "Damage", mRollerLength * 0.5f - 25.0f );
        fn_0028065C( mRoller );
        fn_0027ED20( this, mRoller );
        al::FunctorV0M<SwingNeedleRoller*, void ( SwingNeedleRoller::* )()> onSwitch(
                this, dat_003BF4A4 );
        if ( fn_0027FC08( this, onSwitch ) )
                fn_00280610( this, &dat_003F2BF8 );
        fn_00256C50( this );
        mRoller->appear();
        fn_0027EE34( this, info, 1 );
        fn_0027FD10( this, nullptr,
                sqrtf( mRollerLength * mRollerLength * 0.25f + mChainLength * mChainLength ) );
        fn_0027FAB8( this );
}
