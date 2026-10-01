#include "Enemy/BlockDragonGenerator.h"

#include <Functor/alFunctorV0M.h>
#include <LiveActor/alActorInitInfo.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alActorPoseKeeper.h>
#include <LiveActor/alLiveActorFunction.h>
#include <LiveActor/alLiveActorGroup.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>
#include <Placement/alPlacementFunction.h>

// The four concrete constructors are independently identified by their retail
// stores and the allocation sites in init. These local class names
// describe the observed roles; their unimplemented virtual methods are omitted.
class BlockDragonSegment : public al::MapObjActor
{
public:
        BlockDragonGenerator* mGenerator;
        void* _64;
        BlockDragonSegment( const sead::SafeString& name );
};
class BlockDragonHead : public BlockDragonSegment
{
public:
        BlockDragonHead( const sead::SafeString& name );
};
class BlockDragonTail : public BlockDragonSegment
{
public:
        BlockDragonTail( const sead::SafeString& name );
};
class BlockDragonBody : public BlockDragonSegment
{
        void* _68;
public:
        BlockDragonBody( const sead::SafeString& name );
};
class BlockDragonQuestionBlock : public BlockDragonSegment
{
        int mType;
        void* _6C;
        void* _70;
public:
        BlockDragonQuestionBlock( const sead::SafeString& name, int type );
};

static_assert( sizeof( BlockDragonHead ) == 0x68, "retail allocation" );
static_assert( sizeof( BlockDragonTail ) == 0x68, "retail allocation" );
static_assert( sizeof( BlockDragonBody ) == 0x6C, "retail allocation" );
static_assert( sizeof( BlockDragonQuestionBlock ) == 0x74, "retail allocation" );

// Retail installs a distinct one-slot table after LiveActorGroup construction.
// It inherits the established LiveActorGroup::registerActor implementation.
class BlockDragonGroup : public al::LiveActorGroup
{
public:
        BlockDragonGroup( const char* name, int capacity )
            : al::LiveActorGroup( name, capacity ) {}
};

struct BlockDragonNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* ) const;
};
extern "C" {
extern const BlockDragonNerve dat_003F1E50;
extern const BlockDragonNerve dat_003F1E54;
void fn_0026F5CC( al::LiveActor*, const al::ActorInitInfo& );
void fn_00270AB0( al::LiveActor*, const al::ActorInitInfo& );
void fn_0026F5A8( al::LiveActor*, const al::ActorInitInfo& );
void fn_0026F56C( al::LiveActor*, const al::ActorInitInfo&, int );
void fn_0027FCBC( al::IUseStageSwitch*, const al::ActorInitInfo& );
bool fn_0027FC08( al::IUseStageSwitch*, const al::FunctorBase& );
bool fn_0027D1DC( int*, const al::ActorInitInfo& );
bool fn_0027D180( int*, const al::ActorInitInfo& );
bool fn_0027AFF8( int*, const al::ActorInitInfo& );
bool fn_002693D8( int*, const al::ActorInitInfo& );
bool fn_002671D4( int*, const al::ActorInitInfo& );
bool fn_00266148( int*, const al::ActorInitInfo& );
bool fn_002730F8( int*, const al::ActorInitInfo& );
bool fn_00277AB8( float*, const al::ActorInitInfo& );
bool fn_00250D28( sead::Quatf*, const al::ActorInitInfo& );
void fn_0027A488( sead::Vector3f*, const sead::Quatf& );
void fn_00253FE4( al::LiveActor*, float );
const sead::Vector3f& fn_00277AA4( const al::LiveActor*, const char* );
void fn_00277A7C( al::LiveActor*, const char*, const sead::Vector3f& );
void fn_0027CC64( sead::Vector3f&, const sead::Vector3f&, float );
void fn_0027D4D8( sead::Quatf*, const sead::Vector3f&, const sead::Vector3f& );
void fn_00260F3C( sead::Vector3f*, const al::LiveActor*, float );
void fn_0025CAE0( al::LiveActor*, const sead::Vector3f& );
void fn_0027C05C( al::LiveActor* );
void fn_0027CF6C( sead::Vector3f*, al::LiveActor*, float, float );
void fn_0027CF20( al::LiveActor*, const al::ActorInitInfo&, int );
}

typedef void ( BlockDragonGenerator::*BlockDragonCallback )();

static inline sead::Vector3f scaleVector( const sead::Vector3f& value, float scale )
{
        sead::Vector3f result;
        fn_0027CC64( result, value, scale );
        return result;
}

// NonMatching proposal. See project/dot_reports/block-dragon-init.md.
#ifdef NON_MATCHING
void BlockDragonGenerator::init( const al::ActorInitInfo& info )
{
        fn_0026F5CC( this, info );
        al::initNerve( this, &dat_003F1E50, 0 );
        al::initActorPoseTFSV( this );
        fn_00270AB0( this, info );
        fn_0026F5A8( this, info );
        fn_0026F56C( this, info, 64 );
        fn_0027FCBC( this, info );
        if ( !fn_0027FC08( this,
                     al::FunctorV0M<BlockDragonGenerator*, BlockDragonCallback>( this, &BlockDragonGenerator::startAppear ) )
             && al::isNerve( this, &dat_003F1E50 ) )
                al::setNerve( this, &dat_003F1E54 );

        al::tryGetArg0( &mMoveSpeed, info );
        fn_0027D1DC( &mBodyTypes[0], info );
        fn_0027D180( &mBodyTypes[1], info );
        fn_0027AFF8( &mBodyTypes[2], info );
        fn_002693D8( &mBodyTypes[3], info );
        fn_002671D4( &mBodyTypes[4], info );
        fn_00266148( &mBodyTypes[5], info );
        fn_002730F8( &mBodyTypes[6], info );
        fn_00277AB8( &mShadowLength, info );
        al::tryGetTrans( &mPlacementTrans, info );
        initRailKeeper( info );

        for ( int i = 0; i < 7; ++i )
        {
                if ( mBodyTypes[i] <= 0 )
                        break;
                mBodyCount = i + 1;
        }

        mSegments = new BlockDragonGroup( "BlockDragonGroup", mBodyCount + 2 );
        for ( int i = 0; i < mSegments->getArray<BlockDragonSegment>().capacity(); ++i )
        {
                BlockDragonSegment* segment = 0;
                if ( i == 0 )
                {
                        segment = new BlockDragonHead( "BlockDragonHead" );
                        mHead = segment;
                }
                else if ( i == mSegments->getArray<BlockDragonSegment>().capacity() - 1 )
                {
                        segment = new BlockDragonTail( "BlockDragonTail" );
                        mTail = segment;
                }
                else
                {
                        int type = mBodyTypes[i - 1];
                        if ( type == 1 )
                                segment = new BlockDragonBody( "BlockDragonBody" );
                        else if ( type <= 7 )
                                segment = new BlockDragonQuestionBlock( "BlockDragonQuestionBlock", type );
                }
                segment->mGenerator = this;
                al::initCreateActorNoPlacementInfo( segment, info );
                sead::Quatf placementQuat;
                fn_00250D28( &placementQuat, info );
                sead::Vector3f front;
                fn_0027A488( &front, placementQuat );
                fn_00253FE4( segment, mShadowLength );
                sead::Vector3f shadowOffset = fn_00277AA4( segment, "Body" );
                fn_00277A7C( segment, "Body", scaleVector( shadowOffset, mShadowScale * 0.01f ) );
                const sead::Vector3f& gravity = al::getGravity( segment );
                sead::Vector3f up( -gravity.x, -gravity.y, -gravity.z );
                fn_0027D4D8( al::getQuatPtr( segment ), front, up );
                mSegments->registerActor( segment );
        }

        mLeadingActor = mSegments->getArray<al::LiveActor>()[0];
        for ( int i = 0; i < mSegments->getArray<al::LiveActor>().size(); ++i )
        {
                al::LiveActor* segment = mSegments->getArray<al::LiveActor>()[i];
                segment->initRailKeeper( info );
                float distance = 0.0f;
                for ( int j = 0; j < mSegments->getArray<al::LiveActor>().size() - i - 1; ++j )
                        distance += mSegmentSpacing;
                sead::Vector3f railPosition;
                fn_00260F3C( &railPosition, segment, distance );
                fn_0025CAE0( segment, railPosition );
                fn_0027C05C( segment );
        }
        fn_0027CF6C( &mClippingCenter, this, 100.0f, 100.0f );
        fn_0027CF20( this, info, mSegments->getArray<al::LiveActor>().size() );
        makeActorAppeared();
}

#endif

void BlockDragonGenerator::startAppear()
{
        if ( al::isNerve( this, &dat_003F1E50 ) )
                al::setNerve( this, &dat_003F1E54 );
}
