#include "Enemy/PackunFlower.h"
#include "Enemy/EnemyStateBlowDown.h"

#include <LiveActor/alActorInitializationImports.h>
#include <LiveActor/alActorInitUtil.h>
#include <LiveActor/alLiveActorFunction.h>
#include <Nerve/alNerve.h>
#include <Nerve/alNerveFunction.h>
#include <Placement/alPlacementFunction.h>
#include <Util/alStringUtil.h>

// Neutral class name: 0x00279228 copies a Matrix34f, then clears the pointer
// at 0x30. Independent user 0x002792A8 updates the matrix and that pointer.
class PackunFlowerTransform
{
        sead::Matrix34f mMatrix;
        void* mSource;
public:
        PackunFlowerTransform();
};

// 0x0027924C calls MapObjActor's constructor, installs mapped dispatch at
// 0x003CD798, and initializes an extra pointer plus three floats.
class PackunFlowerTrace : public al::MapObjActor
{
        void* mUnknown60;
        sead::Vector3f mUnknown64;
public:
        explicit PackunFlowerTrace( const sead::SafeString& name );
};

static_assert( sizeof( PackunFlowerTransform ) == 0x34, "Transform allocation" );
static_assert( sizeof( PackunFlowerTrace ) == 0x70, "Trace allocation" );

struct PackunFlowerNerve : al::Nerve
{
        virtual void execute( al::NerveKeeper* keeper ) const;
};
extern "C" const PackunFlowerNerve dat_003F22B4;
extern "C" const PackunFlowerNerve dat_003F22D0;

namespace al
{
bool tryGetArg0( bool*, const ActorInitInfo& );
}
// Preserve the public pointer contract already used by Factory/fn_0018938C.
extern "C" void fn_0027D1DC( int*, const al::ActorInitInfo* );
extern "C" void fn_001E5E18( al::LiveActor* );
extern "C" void fn_001E5E28( al::LiveActor* );
extern "C" void fn_0026FC64( al::IUseStageSwitch*, const al::ActorInitInfo& );
extern "C" void fn_0027CF20( al::LiveActor*, const al::ActorInitInfo&, int );

void PackunFlower::init( const al::ActorInitInfo& info )
{
        const char* objectName;
        al::tryGetObjectName( &objectName, info );
        if ( al::isEqualString( objectName, "PackunFlowerDokan" ) )
        {
                mIsDokan = true;
                mDistance = 500.0f;
                al::initActorWithArchiveName( this, info, "PackunFlowerDokan" );
        }
        else
                al::initActorWithArchiveName( this, info, "PackunFlower" );

        mTransform = new PackunFlowerTransform();
        al::initNerve( this, &dat_003F22B4, 1 );
        al::tryGetArg0( &mIsTrace, info );
        if ( mIsTrace )
        {
                mTrace = new PackunFlowerTrace( "PackunFlowerTrace" );
                mTrace->init( info );
        }
        int arg = 0;
        fn_0027D1DC( &arg, &info );
        if ( arg == 1 )
                fn_001E5E18( this );
        else
                fn_001E5E28( this );

        mBlowDown = new EnemyStateBlowDown( this, nullptr, nullptr, 0 );
        al::initNerveState( this, mBlowDown, &dat_003F22D0, "state:Blow" );
        fn_00280538( this, info );
        fn_0026FC64( this, info );
        fn_0027CF20( this, info, 1 );
        al::offCollide( this );
        fn_0027FAB8( this );
}
